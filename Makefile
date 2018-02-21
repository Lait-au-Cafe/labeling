CXXFLAGS=-std=c++11 -Wall -Wextra -Werror -g
TARGET=labeling

$(TARGET): labeling.cpp
	g++ -o $@ $< $(CXXFLAGS) `pkg-config opencv --libs --cflags`

.PHONY: clean
clean:
	rm -f $(TARGET)
