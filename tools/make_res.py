#!/usr/bin/env python3
# I was thinking of having it dump each resource into a separate .c file but I
# realized that this would make it very difficult for VC++ users
import os
import sys

define = 'STATIC_RESOURCE_H'
filename = 'static_resource'
align = 64

def mangle_path(path):
    def mangle_char(char):
        return char if char.isalnum() else '_'

    def mangle_char_first(char):
        return char if char.isalpha() else '_'

    return mangle_char_first(path[0]) + ''.join(map(mangle_char, path[1:]))

def s_dump(data):
    chunksize = 512
    return '\n'.join('	.byte ' + ','.join(map(str, data[x:x + chunksize])) for x in range(0, len(data), chunksize))

def c_dump(data):
    chunksize = 512
    return '\n'.join('    {},'.format(','.join(map(str, data[x:x + chunksize]))) for x in range(0, len(data), chunksize))

if len(sys.argv) != 2:
    print("Need exactly one argument (directory containing resources).", file=sys.stderr)
    exit(1)

arrays = []  # List of mangled filenames (and sizes) which will be used as array names (and lengths) in the .h

for root, dirs, files in os.walk(sys.argv[1]):
    for file in files:
        mangled = mangle_path(root + '_' + file)

        with open(os.path.join(root, file), 'rb') as fh:
            contents = fh.read()

        arrays.append((mangled, contents))

with open("{}.h".format(filename), "w") as header:
    arrays_str = '\n'.join('extern unsigned char {}[{}];'.format(mangled, hex(len(contents))) for mangled, contents in arrays)
    header.write("""\
#ifndef {define}
#define {define}
#ifdef __cplusplus
extern "C" {{
#endif
#define ARRAY_LENGTH(x) (sizeof(x) / sizeof *(x))
#define ARRAY_WITH_LENGTH(x) (x), ARRAY_LENGTH(x)
{arrays}
#ifdef __cplusplus
}}
#endif
#endif
""".format(define=define, arrays=arrays_str))

with open("{}.s".format(filename), "w") as sfile:
    sfile.write(".data\n")
    for mangled, contents in arrays:
        sfile.write("""\
.align {align}
.globl {name}
.type {name}, @object
.size {name}, {size}
{name}:
{data}
""".format(name=mangled, size=len(contents), data=s_dump(contents), align=align));

with open("{}.c".format(filename), "w") as cfile:
    cfile.write('#include "{filename}.h"\n'.format(filename=filename))
    for mangled, contents in arrays:
        cfile.write("""\
unsigned char {name}[{size}] = {{
{data}
}};
""".format(name=mangled, size=len(contents), data=c_dump(contents), align=align));