#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iterator>

std::string text;

std::string& read_text() {
    auto f  = std::ifstream("input.txt");

    if (f.is_open()) {
	    std::copy(std::istream_iterator<char>(f), std::istream_iterator<char>(), std::back_inserter(text));
		f.close();
	}
	
	return text;
}

%%{
    machine parser;
	write data;

	a = digit+ >{ a = std::atoi(p); };
	b = digit+ >{ b = std::atoi(p); };

	do   = "do()"    @{ enabled = true;   };
	dont = "don't()" @{ enabled = false;  };

	mul = ("mul(" a "," b ")") @{
		if (enabled)
			std::cout << "mul " << a << "*" << b << std::endl;
		answer1 += a * b;
		answer2 += enabled? a * b : 0;
	};
	
	main := (dont | do | mul)+ $err { fgoto main; };
}%%

int main() {
	if (read_text().empty()) {
		std::cout << "unable to load input" << std::endl;
		return 0;
	}

	int answer1 = 0;
	int answer2 = 0;
	bool enabled = true;
	char* p = text.data();
	char* pe = text.data() + text.size();
	char* eof = 0;
	int a, b, cs;

    %% write init;
	%% write exec;
    
    std::cout << "Answer Part 1: " << answer1 << std::endl;
    std::cout << "Answer Part 2: " << answer2 << std::endl;
}