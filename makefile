CXXFLAGS := -I include/ -Wall -Wextra -pedantic -std=c++20 -fno-char8_t

all: samples

clean:
	rm samp1

samples: samp1

samp1: sample/sample.cpp $(wildcard include/*)
	$(CXX) $(CXXFLAGS) -o $@ $<
