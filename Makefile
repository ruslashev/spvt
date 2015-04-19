CXX = g++

OBJS = obj/main.o obj/errors.o obj/renderer/glutils.o obj/renderer/renderer.o \
	obj/renderer/textcacher.o obj/charmatrix.o

EXECNAME = spvt

default: objdir $(EXECNAME)
	./$(EXECNAME)

$(EXECNAME): $(OBJS)
	$(CXX) -o $@ $^ -lGL -lGLEW -lSDL2 -lfreetype

obj/%.o: source/%.cpp
	$(CXX) -c -o $@ $< -Wall -Wextra -g -std=c++0x `freetype-config --cflags`

objdir:
	mkdir -p obj
	mkdir -p obj/renderer

clean:
	-rm -f $(OBJS) $(EXECNAME)

