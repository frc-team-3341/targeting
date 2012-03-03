# Header
all: rectangledetector
# Variable Declarations
#CFLAGS=-march=native -O2 -pipe
CFLAGS=-ggdb -pipe
CXXFLAGS=$(CFLAGS)
CXX_INCLUDE=/usr/include/c++/4.6.2
FLAGS=-Wall -std=gnu++0x
INCLUDE=-I$(CXX_INCLUDE) `pkg-config --cflags opencv`
LINK_INCLUDE=-I$(CXX_INCLUDE) `pkg-config --cflags --libs opencv`
COMPILE=$(CXX) $(FLAGS) $(CXXFLAGS) $(INCLUDE)
LINK=$(CXX) $(FLAGS) $(CXXFLAGS) $(LINK_INCLUDE)

OBJECTS=main.o Constants.o Rectangle.o RectangleDetector.o CRIOLink.o HSVImage.o VideoDevice.o
ALL_OBJECTS=$(OBJECTS) rectangledetector
# End Variable Declarations
# End Header

rectangledetector: $(OBJECTS)
	$(LINK) $(OBJECTS) -o rectangledetector
main.o: main.cpp Constants.hpp
	$(COMPILE) -c main.cpp -o main.o
Constants.o: Constants.cpp Constants.hpp
	$(COMPILE) -c Constants.cpp -o Constants.o
Rectangle.o: Rectangle.cpp Rectangle.hpp Constants.hpp
	$(COMPILE) -c Rectangle.cpp -o Rectangle.o
RectangleDetector.o: RectangleDetector.cpp RectangleDetector.hpp Constants.hpp
	$(COMPILE) -c RectangleDetector.cpp -o RectangleDetector.o
CRIOLink.o: CRIOLink.cpp CRIOLink.hpp
	$(COMPILE) -c CRIOLink.cpp -o CRIOLink.o
HSVImage.o: HSVImage.cpp HSVImage.hpp
	$(COMPILE) -c HSVImage.cpp -o HSVImage.o
VideoDevice.o: VideoDevice.cpp VideoDevice.hpp
	$(COMPILE) -c VideoDevice.cpp -o VideoDevice.o
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
