build:
	g++ -g -std=c++17 src/*.cpp -I include -o main

vrun:
	valgrind --leak-check=full ./main

fbuild:
	g++ -g -fsanitize=address -std=c++17 src/*.cpp -I include -o main

run:
	./main
