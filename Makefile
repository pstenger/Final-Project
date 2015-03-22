all:	main

main: main.o car.o
	g++ main.o car.o -o main

main.o: main.cpp
	g++ -c main.cpp
car.o:  car.cpp
	g++ -c car.cpp

clean:
	rm -f *.o main
