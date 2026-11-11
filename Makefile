CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g -Iinclude
SRC = $(wildcard src/*.cpp)

smti: $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o smti

asan: $(SRC)
	$(CXX) $(CXXFLAGS) -fsanitize=address $(SRC) -o smti_asan

clean:
	rm -f smti smti_asan
