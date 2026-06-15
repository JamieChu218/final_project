# =============================================================
#  Makefile — 商店庫存管理系統
# =============================================================
CXX      := g++
CXXFLAGS := -std=c++14 -Wall -Wextra -Isrc
TARGET   := inventory
SRCDIR   := src
OBJDIR   := build

SOURCES  := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS  := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

run: all
	./$(TARGET)

clean:
	rm -rf $(OBJDIR) $(TARGET)
