lex:lexer.cpp source.txt
	g++ -std=c++11 -w lexer.cpp -o lexer 
	lexer >out

LL1:lexer.cpp source.txt LL1.cpp
	g++ -std=c++11 -w LL1.cpp -o LL1
	LL1 >out
	LL1
.PHONY :clean
clean:
	del *.exe out out2