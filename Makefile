CC      = gcc
DEBUG   = -g
CFLAGS  = -Wall -ansi -pedantic
BIN     = AddressBook
OBJS    = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

# Prevent make to do some strange behaviour,
#   e.g. if a file called "all" or "debug" exists in the source path, it won't work as expected.
.PHONY: all debug clean archive

# Prevent make deleting file when it is interrupted
.PRECIOUS: $(BIN) $(OBJS)

# Force using GCC in macOS because clang toolchain does not recognize the "-ansi" flag and throws annoying warnings
# on every time it compiles. GCC can be retrieved by using Homebrew, by running command "brew install gcc".
ifeq ($(shell uname -s), Darwin)
	CC  = gcc-7
endif

%.o: %.c $(HEADERS)
	@echo Compiling $@ ...
	$(CC) $(CFLAGS) -c $< -o $@

all: $(OBJS)
	@echo Linking...
	$(CC) $(CFLAGS) -o $(BIN) $(OBJS)

debug:  $(OBJS)
	@echo Linking...
	$(CC) $(CFLAGS) $(DEBUG) -o $(BIN) $(OBJS)

clean:
	rm -f $(OBJS) $(BIN)

archive:
	zip $(USER)-a2 $(SOURCES) $(HEADERS) Makefile
