CC      = gcc
DEBUG   = -g
CFLAGS  = -Wall -ansi -pedantic
BIN = AddressBook
OBJS    = addressbook.o addressbook_array.o addressbook_list.o commands.o helpers.o


# Force using GCC in macOS because clang toolchain does not recognize the "-ansi" flag and throws annoying warnings
# on every time it compiles. GCC can be retrieved by using Homebrew, by running command "brew install gcc".
ifeq ($(shell uname -s), Darwin)
	CC  = gcc-7
endif


all: $(OBJS)
	@echo Linking...
	$(CC) $(CFLAGS) -o $(BIN) $(OBJS)

debug:  $(OBJS)
	@echo Linking...
	$(CC) $(CFLAGS) $(DEBUG) -o $(BIN) $(OBJS)

%.o: %.c
	@echo Compiling $@ ...
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -f $(OBJS) $(BIN)

archive:
	zip $(USER)-a2 $(SOURCES) $(HEADERS) Makefile
