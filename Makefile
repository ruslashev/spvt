CXX = g++

OBJS = obj/main.o obj/errors.o obj/glutils.o obj/renderer/renderer.o \
	obj/renderer/textdrawer.o obj/renderer/widgets/texteditor.o

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
	mkdir -p obj/renderer/widgets

clean:
	-rm -f $(OBJS) $(EXECNAME)

