CCC = g++
CCFLAGS = -std=c++11 -Wall -Wextra
LIBS = -lSDL2
FILES = src/*.cc
NAME = hobo

all: compile

run :
	./$(NAME)

compile :
	$(CCC) $(CCFLAGS) $(FILES) -o $(NAME) $(LIBS)

clean :
	rm $(NAME)
