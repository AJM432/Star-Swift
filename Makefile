 all:
	g++ -std=c++11 src/*.cpp -o main -I include -L lib -l SDL2-2.0.0

#2.28.5