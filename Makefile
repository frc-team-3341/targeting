# Header
all: rectangledetector
# Variable Declarations
CPP_INCLUDE=/usr/include/c++/4.6.2 # Defines the path for the C++ libraries
OBJECTS=main.o Rectangle.o
ALL_OBJECTS=$(OBJECTS) rectangledetector
# End Variable Declarations
# End Header

rectangledetector: $(OBJECTS)
	g++ $(OBJECTS) -Wall -I$(CPP_INCLUDE) `pkg-config --cflags --libs opencv` -g -o rectangledetector.bin
# strip rectangledetector # Enable to remove debugging symbols, making the binary smaller
main.o: main.cpp
	g++ -c main.cpp -Wall -I$(CPP_INCLUDE) `pkg-config --cflags opencv` -g -o main.o
Rectangle.o: Rectangle.cpp Rectangle.hpp
	g++ -c Rectangle.cpp -Wall -I$(CPP_INCLUDE) `pkg-config --cflags opencv` -g -o Rectangle.o
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
