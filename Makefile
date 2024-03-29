CC      = gcc
DEBUG   = -g
CFLAGS  = -Wall -ansi -pedantic
BIN     = AddressBook
SOURCES = $(wildcard *.c)
OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)
CSV     = $(wildcard *.txt)


# Force using GCC in macOS because clang toolchain does not recognize the "-ansi" flag and throws annoying warnings
# on every time it compiles. GCC can be retrieved by using Homebrew, by running command "brew install gcc".
ifeq ($(shell uname -s), Darwin)
	CC  = gcc-7
endif

# Prevent make to do some strange behaviour,
#   e.g. if a file called "all" or "debug" exists in the source path, it won't work as expected.
.PHONY: all debug clean archive

# Prevent make deleting file when it is interrupted
.PRECIOUS: $(BIN) $(OBJECTS)


%.o: %.c %.h
	@echo Compiling $@ ...
	$(CC) $(CFLAGS) -c $< -o $@

all: $(OBJECTS)
	@echo Linking...
	$(CC) $(CFLAGS) -o $(BIN) $(OBJECTS)

debug:  $(OBJECTS)
	@echo Linking...
	$(CC) $(CFLAGS) $(DEBUG) -o $(BIN) $(OBJECTS)

clean:
	rm -f $(OBJECTS) $(BIN) *.zip

archive:
	zip $(USER)-a2 $(SOURCES) $(HEADERS) $(CSV) Makefile
