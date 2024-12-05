main.cpp: main.cpp.rl
	ragel main.cpp.rl -o main.cpp

main:	main.cpp main.cpp.rl
	g++ main.cpp -o main

run:	main
	./main
