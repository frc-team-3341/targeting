# Header
all: rectangledetector
# Variable Declarations
CPP_INCLUDE=/usr/include/c++/4.4 # Defines the path for the C++ libraries
OBJECTS=main.o HSVImage.o
ALL_OBJECTS=$(OBJECTS) rectangledetector
# End Variable Declarations
# End Header

rectangledetector: $(OBJECTS)
	g++ $(OBJECTS) -Wall -I$(CPP_INCLUDE) `pkg-config --cflags --libs opencv` -g -o rectangledetector.bin
# strip rectangledetector # Enable to remove debugging symbols, making the binary smaller
main.o: main.cpp
	g++ -c main.cpp -Wall -I$(CPP_INCLUDE) `pkg-config --cflags opencv` -g -o main.o
HSVImage.o: HSVImage.cpp
	g++ -c HSVImage.cpp -Wall -I$(CPP_INCLUDE) `pkg-config --cflags opencv`
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
