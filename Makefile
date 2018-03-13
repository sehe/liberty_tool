CXXFLAGS := -std=c++14 -Wall -Wextra -O2
BOOST_DIR=/home/sehe/custom/boost

CXXFLAGS += -isystem $(BOOST_DIR)

ifneq ($(LINK),STATIC)
LDFLAGS  := -L$(BOOST_DIR)/stage/lib \
			-Wl,-rpath,$(BOOST_DIR)/stage/lib \
	        -lboost_system \
            -lboost_filesystem
else
LDFLAGS  := -static \
            $(BOOST_DIR)/stage/lib/libboost_system.a \
            $(BOOST_DIR)/stage/lib/libboost_filesystem.a
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
