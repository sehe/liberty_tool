CXXFLAGS := -std=c++14 -Wall -Wextra -O2

ifneq ($(LINK),STATIC)
LDFLAGS  := -lboost_system \
            -lboost_filesystem
else
LDFLAGS  := -static \
            /usr/lib/x86_64-linux-gnu/libboost_system.a \
            /usr/lib/x86_64-linux-gnu/libboost_filesystem.a
endif

BUILDDIR ?= build
BINDIR   ?= bin
PROGRAM  ?= liberty_tool

HEADERS  := $(wildcard src/*.hpp)
SOURCES  := $(wildcard src/*.cpp)
OBJECTS  := $(patsubst src/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))
DEPENDS  := $(patsubst src/%.cpp,$(BUILDDIR)/%.d,$(SOURCES))

.PHONY: all
all: $(BINDIR)/$(PROGRAM)

$(BINDIR)/$(PROGRAM): $(OBJECTS)
	@test -d $(BINDIR) || mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

-include $(DEPENDS)
$(BUILDDIR)/%.o: src/%.cpp
	@test -d $(BUILDDIR) || mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -MMD -c -o $@ $<

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)

# Clang Format - Settings in file .clang-format
.PHONY: format
format:
	clang-format -i -style=file $(HEADERS) $(SOURCES)

# Clang Tidy
.PHONY: tidy
tidy:
	clang-tidy -checks=cppcoreguidelines-*,modernize-*,readability-*,-readability-braces-around-statements \
	           -header-filter=src/ $(SOURCES) -- $(CXXFLAGS)
