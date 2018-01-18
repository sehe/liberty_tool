CXXFLAGS := -std=c++14 -Wall

ifneq ($(LINK),STATIC)
LDFLAGS  := -lboost_system \
            -lboost_filesystem
else
LDFLAGS  := -static \
            /usr/lib/x86_64-linux-gnu/libboost_system.a \
            /usr/lib/x86_64-linux-gnu/libboost_filesystem.a
endif

HEADERS  := $(wildcard src/*.hpp)
SOURCES  := $(wildcard src/*.cpp)
OBJECTS  := $(patsubst src/%.cpp,build/%.o,$(SOURCES))

BUILDDIR ?= build
BINDIR   ?= bin
PROGRAM  ?= liberty_tool

.PHONY: all
all: $(BINDIR)/$(PROGRAM)

$(BINDIR)/$(PROGRAM): $(OBJECTS)
	@test -d $(BINDIR) || mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILDDIR)/%.o: src/%.cpp
	@test -d $(BUILDDIR) || mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $^

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)

# Clang Format - Settings in file .clang-format
.PHONY: format
format:
	clang-format -i -style=file $(HEADERS) $(SOURCES)