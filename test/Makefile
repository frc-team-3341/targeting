squares: squares.o Rectangle.o
	g++ squares.o Rectangle.o `pkg-config --cflags --libs opencv` -g -o squares
squares.o: squares.cpp
	g++ -c squares.cpp `pkg-config --cflags --libs opencv` -g -o squares.o
Rectangle.o: Rectangle.cpp Rectangle.hpp
	g++ -c Rectangle.cpp `pkg-config --cflags --libs opencv` -g -o Rectangle.o
