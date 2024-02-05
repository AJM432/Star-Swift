#  all:
# 	g++ -std=c++11 src/*.cpp -o main -I include/SDL2 -I include/imgui -L lib -l SDL2-2.0.0

INCLUDE_DIRS = -I include/SDL2 -I include/imgui
LIB_DIRS = -L lib -l SDL2-2.0.0

SRC = $(wildcard src/*.cpp) $(wildcard imgui/*.cpp)

default:
	g++ -std=c++11 $(SRC) -o StarSwift $(INCLUDE_DIRS) $(LIB_DIRS)