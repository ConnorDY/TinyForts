#define _FILE_OFFSET_BITS 64
#define _POSIX_C_SOURCE 200809
#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <err.h>
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define HEADER_DEFINE "STATIC_RESOURCE_H"
#define RESOURCES_FILE "static_resource"
#define ALIGN_TO "64"

#if !defined(BUILD_C_FILE) && !defined(BUILD_S_FILE)
#warn "You should probably define BUILD_C_FILE or BUILD_S_FILE"
#endif

void mangle_path(char *restrict path)
{
    // Replace all non-alphanumeric chars with an underscore
    // The first character additionally cannot be a number, so will be
    // replaced with an underscore in that case too.
    if (!*path) return;
    if (!isalpha(*path)) *path = '_';

    while (*++path) {
        if (!isalpha(*path) && !isdigit(*path))
            *path = '_';
    }
}

void f_s_dump(FILE *file, unsigned char *data, size_t data_length)
{
    size_t i = 0;
    while (i < data_length) {
        fputs("	.byte ", file);
        // 512 here is a constant which doesn't matter. It is the maximum
        // bytes per row. This just happened to make 'as' compile much faster.
        for (size_t j = 0; j < 512 && i < data_length; i++, j++) {
            if (j) fputc(',', file);
            fprintf(file, "%d", data[i]);
        }
        fputc('\n', file);
    }
}

void f_c_dump(FILE *file, unsigned char *data, size_t data_length)
{
    size_t i = 0;
    while (i < data_length) {
        fputs("    ", file);
        for (size_t j = 0; j < 512 && i < data_length; i++, j++) {
            fprintf(file, "%d,", data[i]);
        }
        fputc('\n', file);
    }
}

// Adapted from rosetta code
int walk_dir(char *dname, void (*func)(void*, char const*), void *param)
{
    struct dirent *dent;
    DIR *dir;
    struct stat st;
    char fn[FILENAME_MAX];
    int res = 0;
    int len = strlen(dname);
    if (len >= FILENAME_MAX - 1)
        return EINVAL;

    strcpy(fn, dname);
    fn[len++] = '/';

    if (!(dir = opendir(dname))) {
        warn("can't open %s", dname);
        return errno;
    }

    errno = 0;
    while ((dent = readdir(dir))) {
        if (!strcmp(dent->d_name, ".") || !strcmp(dent->d_name, ".."))
            continue;

        strncpy(fn + len, dent->d_name, FILENAME_MAX - len);
        if (lstat(fn, &st) == -1) {
            warn("Can't stat %s", fn);
            res = errno;
            continue;
        }

        /* don't follow symlinks */
        if (S_ISLNK(st.st_mode))
            continue;

        if (S_ISDIR(st.st_mode)) {
            /* recursively follow dirs */
            walk_dir(fn, func, param);
        } else {
            func(param, fn);
        }
    }

    closedir(dir);
    return res;
}

struct file_list_t {
    char *filename;
    unsigned char *data;
    size_t filesize;
    struct file_list_t *next;
};

void add_fname_to_list(struct file_list_t **head, char const *fname)
{
    struct file_list_t *new_elem = malloc(sizeof *new_elem);
    new_elem->next = *head;
    new_elem->filename = strdup(fname);
    new_elem->data = NULL;

    *head = new_elem;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fputs("Need exactly 1 argument (name of directory to convert)\n", stderr);
        return 1;
    }

    struct file_list_t *head = NULL;
    walk_dir(argv[1], (void(*)(void*,char const*))add_fname_to_list, &head);

    // Get file contents and mangle filenames
    for (struct file_list_t *curr = head; curr; curr = curr->next) {
        FILE *f = fopen(curr->filename, "rb");
        if (f) {
            fseek(f, 0, SEEK_END);
            off_t size = ftello(f);
            if (size == -1 && errno != 0) {
                warn("Could not get size of %s", curr->filename);
                goto skip_file;
            }
            curr->filesize = (size_t) size;
            if ((ssize_t) size != size) {
                // If we get here, pointers aren't wide enough to store
                // the file. I doubt we will ever hit this, so I'll just
                // make it an unsupported case rather than changing the
                // focus of this program to being optimized for memory
                // usage.
                fprintf(stderr, "File too large at %s, skipping\n", curr->filename);
                goto skip_file;
            }
            fseek(f, 0, SEEK_SET);
            curr->data = malloc(curr->filesize);
            if (fread(curr->data, 1, curr->filesize, f) != curr->filesize) {
                if (ferror(f)) {
                    warn("Could not fully read %s", curr->filename);
skip_file:
                    free(curr->data);
                    curr->data = NULL;
                } else if (feof(f)) {
                    fprintf(stderr, "Reached end of file early reading %s; using it anyway\n", curr->filename);
                }
            }
            fclose(f);
        } else {
            warn("Could not open %s for reading", curr->filename);
        }
        mangle_path(curr->filename);
    }

    // Write header
    {
        FILE *header = fopen(RESOURCES_FILE ".h", "wt");
        fputs("#ifndef " HEADER_DEFINE "\n"
              "#define " HEADER_DEFINE "\n"
              "#ifdef __cplusplus\n"
              "extern \"C\" {\n"
              "#endif\n"
              "\n"
              "#define ARRAY_LENGTH(x) (sizeof(x) / sizeof *(x))\n"
              "#define ARRAY_WITH_LENGTH(x) (x), ARRAY_LENGTH(x)\n"
              "\n", header);
        // Write each filename into the header
        for (struct file_list_t *curr = head; curr; curr = curr->next) {
            fprintf(header, "extern unsigned char %s[0x%" PRIx64 "];\n", curr->filename, (uint64_t)curr->filesize);
        }
        fputs("\n"
              "#ifdef __cplusplus\n"
              "}\n"
              "#endif\n"
              "#endif\n", header);
        fclose(header);
    }

#ifdef BUILD_S_FILE
    // Write .s file
    {
        FILE *sfile = fopen(RESOURCES_FILE ".s", "wt");
        fputs(".data\n"
              ".align " ALIGN_TO "\n", sfile);
        // Write each file's contents into the .s file
        for (struct file_list_t *curr = head; curr; curr = curr->next) {
            fprintf(sfile,
                    ".globl %s\n"
                    ".type %s, @object\n"
                    ".size %s, %" PRIu64 "\n"
                    "%s:\n",
                    curr->filename, curr->filename, curr->filename, (uint64_t) curr->filesize, curr->filename);
            f_s_dump(sfile, curr->data, curr->filesize);
        }
        fclose(sfile);
    }
#endif

#ifdef BUILD_C_FILE
    // Write .c file
    {
        FILE *cfile = fopen(RESOURCES_FILE ".c", "wt");
        fputs("#include \"" RESOURCES_FILE ".h\"\n", cfile);
        // Write each file's contents into the .c file
        for (struct file_list_t *curr = head; curr; curr = curr->next) {
            fprintf(cfile,
                    "unsigned char %s[%" PRIu64 "] = {\n",
                    curr->filename, (uint64_t) curr->filesize);
            f_c_dump(cfile, curr->data, curr->filesize);
            fputs("};\n", cfile);
        }
        fclose(cfile);
    }
#endif

    // Don't bother freeing lists and stuff, OS does that for us
    return 0;
}