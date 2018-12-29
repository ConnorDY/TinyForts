STATIC_RESOURCES=false
# Fast, but potentially produces larger binaries containing unused resources
# because it bypasses LTO for the resource definitions
STATIC_RESOURCES_FAST=false

CC=gcc
CXX=g++

LIBS=-lsfml-graphics -lsfml-network -lsfml-audio -lsfml-window -lsfml-system

LDFLAGS=-Wall -Wextra -pedantic -fwhole-program
CPPFLAGS=-Wall -Wextra -pedantic -Wno-reorder -I.
CXXFLAGS=-std=c++14 -Og -g3 -flto -pipe -fstack-protector-strong
CFLAGS=-std=c11 -Og -g3 -flto -pipe -fstack-protector-strong

TOOL_CFLAGS=-std=c11 -Ofast -march=native -pipe
TOOL_LDFLAGS=-s
TOOL_CPPFLAGS=-Wall -Wextra -pedantic
TOOL_OBJS=tools/make_res.o
TOOL_FILES=$(TOOL_OBJS) $(TOOL_OBJS:.o=.d) tools/make_res

DIRECTORIES=./
SRCS=$(wildcard $(addsuffix *.cpp,$(DIRECTORIES)))
OBJS=$(SRCS:.cpp=.o)
OUTPUT=platformer

ifeq ($(STATIC_RESOURCES), true)
	# Put static_resource at the start because it takes some time to compile the tools and generate it.
	OBJS := static_resource.o $(OBJS)
	CPPFLAGS += -DSTATIC_RESOURCES

	ifeq ($(STATIC_RESOURCES_FAST), true)
		TOOL_CPPFLAGS += -DBUILD_S_FILE
		TOOL_BUILDS = static_resource.s
	else
		TOOL_CPPFLAGS += -DBUILD_C_FILE
		TOOL_BUILDS = static_resource.c
	endif
endif


all: $(OUTPUT)

ifeq ($(STATIC_RESOURCES), true)
main.o menu_state.o: static_resource.h
endif

$(OUTPUT): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

%.o: %.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $< -MMD -MQ $@

$(TOOL_BUILDS): static_resource.h
static_resource.h: tools/make_res res
	tools/make_res res

tools/make_res: tools/make_res.o

tools/%.o: tools/%.c
	$(CC) -c $(TOOL_CPPFLAGS) $(TOOL_CFLAGS) -o $@ $< -MMD -MQ $@

clean:
	rm -f $(OBJS) $(OUTPUT) $(OBJS:.o=.d) $(TOOL_FILES) static_resource.[hsc]

-include $(addsuffix *.d,$(DIRECTORIES))
