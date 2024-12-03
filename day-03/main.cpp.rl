#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iterator>

std::string load_text() {
    std::ifstream f("input.txt");
    std::string text;

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
	op = "mul(" a "," b ")";
	main := op+ @{ answer += a * b; } $err { fgoto main; };
}%%

int main() {
	auto text = load_text();
	if (text.empty()) {
		std::cout << "unable to load input" << std::endl;
		return 0;
	}

	int answer = 0;
	char* p = text.data();
	char* pe = text.data() + text.size();
	char* eof = 0;
	int a, b, cs;

    %% write init;
	%% write exec;
    
    std::cout << "Answer: " << answer << std::endl;
}
