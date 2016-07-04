CCC = g++
CCFLAGS = -std=c++11 -pthread -Wall -Wextra
LIBS = -lSDL2 -lSDL2_net -lSDL2_ttf -lSDL2_image
FILES = src/*.cc
NAME = hobo

all: compile

run :
	./$(NAME)

compile :
	$(CCC) $(CCFLAGS) $(FILES) -o $(NAME) $(LIBS)

clean :
	rm $(NAME)
