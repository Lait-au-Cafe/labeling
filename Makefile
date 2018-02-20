CXXFLAGS=-std=c++11 -Wall -Wextra -Werror
TARGET=labeling

$(TARGET): labeling.cpp
	g++ -o $@ $< `pkg-config opencv --libs --cflags`

.PHONY: clean
clean:
	rm -f $(TARGET)
