# define the correct path and name
export PATH 		:= c:/tcc/:$(PATH)
PROJECT_NAME        ?= GAMENAME
OUTDIR				= out/
SOURCE				= src/
DEBUGGING           ?= FALSE
PLATFORM			?= PLATFORM_WEB

# Emscripten required variables
EMSDK_PATH          ?= C:/emsdk
EMSCRIPTEN_VERSION  ?= 1.38.32
CLANG_VERSION       = e$(EMSCRIPTEN_VERSION)_64bit
PYTHON_VERSION      = 2.7.13.1_64bit\python-2.7.13.amd64
NODE_VERSION        = 8.9.1_64bit
export PATH         = $(EMSDK_PATH);$(EMSDK_PATH)\clang\$(CLANG_VERSION);$(EMSDK_PATH)\node\$(NODE_VERSION)\bin;$(EMSDK_PATH)\python\$(PYTHON_VERSION);$(EMSDK_PATH)\emscripten\$(EMSCRIPTEN_VERSION)
EMSCRIPTEN          = $(EMSDK_PATH)\emscripten\$(EMSCRIPTEN_VERSION)

CC = emcc
MAKE = make

EXT = .html

CFLAGS += -Os -s USE_GLFW=3 -s TOTAL_MEMORY=134217728 -s USE_PTHREADS=1 --preload-file resources

ifeq ($(DEBUGGING), TRUE)
	CFLAGS += -s ASSERTIONS=1 --profiling
endif

# Define a custom shell .html and output extension
CFLAGS += --shell-file shell.html

INCLUDE_PATHS = -I.
LDFLAGS = -L.
LDLIBS = libraylib.bc

PROJECT_SOURCE_FILES ?= $(wildcard $(SOURCE)*.c $(SOURCE)**/*.c)

OBJS = $(patsubst %.c, %.o, $(PROJECT_SOURCE_FILES))

.PHONY: all clean

all:
	$(MAKE) $(PROJECT_NAME) -B

$(PROJECT_NAME): $(OBJS)
	$(CC) -o $(OUTDIR)$(PROJECT_NAME)$(EXT) $(OBJS) $(CFLAGS) $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS) -D$(PLATFORM)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE_PATHS) -D$(PLATFORM)

clean:
	del $(wildcard $(SOURCE)*.o $(SOURCE)**/*.o)
