WARNINGS = -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wformat=2 -Winit-self -Wmissing-declarations -Wredundant-decls -Wshadow -Wstrict-overflow=5 -Wswitch-default -Wundef
FLAGS = $(WARNINGS) -std=c++17 -pthread -finput-charset=UTF-8

SRC = ./test.cpp

CC = mpic++

test:
	$(CC) $(FLAGS) -DTESTS -Ofast $(SRC) -I include -o main -lgtest

debug:
	$(CC) $(FLAGS) -DDEBUG $(SRC) -o main
	./main