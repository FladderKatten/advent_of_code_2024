#include "../common/util.h"

%%{
    machine parser;
	write data;

	a = digit+ >{ a = std::atoi(p); };
	b = digit+ >{ b = std::atoi(p); };

	do   = "do()"    @{ enabled = true;   };
	dont = "don't()" @{ enabled = false;  };

	mul = ("mul(" a "," b ")") @{
		answer1 += a * b;
		answer2 += enabled? a * b : 0;
	};
	
	main := (dont | do | mul)+ $err { fgoto main; };
}%%

int main() {
	auto text = read_file_raw_text("input.txt");
	if (text.empty()) {
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