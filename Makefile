CXX = g++
SRC = $(wildcard src/*.cpp)
OUT = trynd
INCLUDES = -Iinclude

build:
	$(CXX) $(INCLUDES) $(SRC) -o $(OUT)
