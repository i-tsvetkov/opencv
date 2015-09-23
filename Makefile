CXX = g++
CXXFLAGS += -Wall -std=c++11
OPENCVFLAGS = $(shell pkg-config --cflags --libs opencv)

all: detect

detect: detect.cpp
	$(CXX) $(CXXFLAGS) detect.cpp -o detect $(OPENCVFLAGS)

clean:
	rm -f detect

