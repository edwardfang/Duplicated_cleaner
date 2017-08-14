CC=gcc
CFLAGS=-Wall
TARGET = $(BUILDDIR)/cleaner
SOURCEDIR = .
BUILDDIR = ./build
SOURCES = $(wildcard $(SOURCEDIR)/*.cpp $(SOURCEDIR)/*.c)
OBJECTS = $(patsubst $(SOURCEDIR)/%.c,$(BUILDDIR)/%.o,$(SOURCES))
MKDIR_P = mkdir -p


$(TARGET):$(OBJECTS)
	$(CC) $(CFLAGS)  -o $@ $^

$(OBJECTS): $(BUILDDIR)/%.o : $(BUILDDIR) $(SOURCEDIR)/%.c
	$(CC) $(CFLAGS)  -o $@ -c $^

.PHONY: $BUILDDIR
$(BUILDDIR): 
	$(MKDIR_P) $(BUILDDIR)

.PHONY : clean
clean : $(BUILDDIR)
	find $(BUILDDIR) -name "*.o"  | xargs rm -f

.PHONY : test
test:
	@echo $(OBJECTS) -- $(SOURCES)
