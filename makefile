all:	main

main: main.o
	g++  -lSDL2 -lSDL2_image -lSDL2_ttf main.o -o main

main.o: main.cpp
	g++ -c main.cpp -lSDL2 -lSDL2_image -lSDL2_ttf

clean:
	rm -f *.o main