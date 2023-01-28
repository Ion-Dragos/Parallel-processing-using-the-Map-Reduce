build:
	g++ -o tema1 tema1.cpp -lm -Wall -Werror -lpthread

build_debug:
	g++ -o tema1 tema1.cpp -lpthread -DDEBUG -g3 -O0 -Werror -Wall

clean:
	rm -rf tema1 *.txt