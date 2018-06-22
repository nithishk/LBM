CXX = g++
#CXXFLAGS = -O3 -Wall -Winline -Wshadow -ansi
CXXFLAGS = -O3 -Wall -ansi -pedantic

SRC = $(wildcard ./lbm.cpp)
BIN = $(SRC:.cpp=)

default: $(BIN)

clean:
	$(RM) $(BIN)
