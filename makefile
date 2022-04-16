build:
	g++ -std=c++17 src/*.cpp -I include -o main

run:
	valgrind --leak-check=full ./main
