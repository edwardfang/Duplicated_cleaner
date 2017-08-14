CC=gcc
CFLAGS=-Wall
EXECUTABLE = $(BUILDDIR)/cleaner
SOURCEDIR = .
BUILDDIR = ./build
SOURCES = $(wildcard $(SOURCEDIR)/*.cpp $(SOURCEDIR)/*.c)
OBJECTS = $(patsubst $(SOURCEDIR)/%.c,$(BUILDDIR)/%.o,$(SOURCES))
MKDIR_P = mkdir -p

all: $(BUILDDIR) $(EXECUTABLE) 

.PHONY: debug
debug:all
	./build/cleaner -vd ./build/

$(EXECUTABLE):$(OBJECTS)
	$(CC) $(CFLAGS)  -o $@ $^

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
