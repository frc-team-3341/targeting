# Header
all: rectangledetector
# Variable Declarations
#CFLAGS=-march=native -O2 -pipe
CFLAGS=-ggdb
CXXFLAGS=$(CFLAGS)
CPP_INCLUDE=/usr/include/c++/4.6.2 # Defines the path for the C++ libraries
OBJECTS=main.o Constants.o Rectangle.o RectangleDetector.o
ALL_OBJECTS=$(OBJECTS) rectangledetector
# End Variable Declarations
# End Header

rectangledetector: $(OBJECTS)
	g++ $(OBJECTS) -Wall -I$(CPP_INCLUDE) `pkg-config --cflags --libs opencv` $(CXXFLAGS) -o rectangledetector.bin
# strip rectangledetector # Enable to remove debugging symbols, making the binary smaller
main.o: main.cpp Constants.hpp
	g++ -c main.cpp -Wall -I$(CPP_INCLUDE) `pkg-config --cflags opencv` $(CXXFLAGS) -o main.o
Constants.o: Constants.cpp Constants.hpp
	g++ -c Constants.cpp -Wall -I$(CPP_INCLUDE) `pkg-config --cflags opencv` $(CXXFLAGS) -o Constants.o
Rectangle.o: Rectangle.cpp Rectangle.hpp Constants.hpp
	g++ -c Rectangle.cpp -Wall -I$(CPP_INCLUDE) `pkg-config --cflags opencv` $(CXXFLAGS) -o Rectangle.o
RectangleDetector.o: RectangleDetector.cpp RectangleDetector.hpp Constants.hpp
	g++ -c RectangleDetector.cpp -Wall -I$(CPP_INCLUDE) `pkg-config --cflags opencv` $(CXXFLAGS) -o RectangleDetector.o
clean:
	touch $(OBJECTS) o~
	rm $(OBJECTS) *~
cleanall:
	touch $(ALL_OBJECTS) o~
	rm $(ALL_OBJECTS) *~
install: rectangledetector
	cp rectangledetector /usr/local/bin/rectangledetector
	chown root\: /usr/local/bin/rectangledetector
	chgrp root /usr/local/bin/rectangledetector
	chmod 755 /usr/local/bin/rectangledetector
