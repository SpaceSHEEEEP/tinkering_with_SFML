CC = g++
MAIN_FILE = bouncing_balls.cpp
OUTPUT_NAME = bouncing_balls
LINKER = -lsfml-graphics -lsfml-window -lsfml-system

all : run

run : bouncing_balls.cpp
	clear
	$(CC) $(MAIN_FILE) -o $(OUTPUT_NAME) $(LINKER)
	./$(OUTPUT_NAME)

clean : 
	rm $(OUTPUT_NAME) 
	clear