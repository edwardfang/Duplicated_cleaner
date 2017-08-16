CC=gcc
CFLAGS=-Wall
LIBS=-lcrypto -lssl
EXECUTABLE = $(BUILDDIR)/duscanner
SOURCEDIR = .
BUILDDIR = ./build
SOURSCE := $(wildcard $(SOURCEDIR)/*.cpp $(SOURCEDIR)/*.c)
SOURCES := $(filter-out $(SOURCEDIR)/linkedlist_learn.c, $(SOURSCE))
OBJECTS = $(patsubst $(SOURCEDIR)/%.c,$(BUILDDIR)/%.o,$(SOURCES))
MKDIR_P = mkdir -p

OS := $(shell uname)
ifeq ($(OS),Darwin)
	# Fix build fail on MacOS
	CFLAGS += -I/usr/local/opt/openssl/include
endif

all: $(BUILDDIR) $(EXECUTABLE) 

.PHONY: debugvd
debugvd:all
	./build/cleaner -vd ~

.PHONY: debugv
debugv:all
	./build/cleaner -v ~

.PHONY: debugpd
debugpd:all
	./build/cleaner -vd ./testfile

.PHONY: debug
debug:all
	./build/cleaner ~

$(EXECUTABLE):$(OBJECTS)
	$(CC) $(CFLAGS)  -o $@ $^ $(LIBS)

$(OBJECTS): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.c
	$(CC) $(CFLAGS)  -o $@ -c $^

.PHONY: $BUILDDIR
$(BUILDDIR): 
	$(MKDIR_P) $(BUILDDIR)

.PHONY : clean
clean : $(BUILDDIR)
	find $(BUILDDIR) -name "*.o" | xargs rm -f
	rm -f $(BUILDDIR)/cleaner
.PHONY : test
test:
	@echo $(OBJECTS) -- $(SOURCES)
