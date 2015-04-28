all:	main2

main: main2.o
	g++  -lSDL2 -lSDL2_image -lSDL2_ttf main2.o -o main2

main.o: main.cpp
	g++ -c main2.cpp -lSDL2 -lSDL2_image -lSDL2_ttf

clean:
	rm -f *.o main2