SRC = $(wildcard source/*.cpp) $(wildcard source/renderer/*.cpp)
OBJ = $(patsubst source/%.cpp, .objs/%.o, $(SRC))
DEP = $(OBJ:.o=.d)
EXECNAME = spvt
LDFLAGS = -lGL -lGLEW -lSDL2 -lfreetype
CXXFLAGS = -Wall -Wextra -g -std=c++0x `freetype-config --cflags`

default: objdir $(EXECNAME)
	./$(EXECNAME)

$(EXECNAME): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

.objs/%.o: source/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

objdir:
	mkdir -p .objs
	mkdir -p .objs/renderer

-include $(DEP)
.objs/%.d: source/%.cpp
	$(CPP) $(CXXFLAGS) $< -MM -MT $(@:.d=.o) > $@

.PHONY: clean
clean:
	-rm -f $(OBJ) $(EXECNAME)
	-rm -f $(DEP)

