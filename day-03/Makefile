main:	main.cpp.rl
	ragel main.cpp.rl -o main.cpp
	g++ main.cpp -o main

run:	main
	./main

clean:
	rm -rf main main.cpp *.dot *.png