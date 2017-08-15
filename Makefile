CC=gcc
CFLAGS=-Wall
LIBS=-lcrypto -lssl
EXECUTABLE = $(BUILDDIR)/cleaner
SOURCEDIR = .
BUILDDIR = ./build
SOURCES = $(wildcard $(SOURCEDIR)/*.cpp $(SOURCEDIR)/*.c)
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
	./build/cleaner -vd ./build/

.PHONY: debugpd
debugpd:all
	./build/cleaner -vd ./testfile

.PHONY: debug
debug:all
	./build/cleaner ./build/

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
