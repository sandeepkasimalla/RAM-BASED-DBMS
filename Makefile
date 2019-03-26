parser:lex.yy.c y.tab.c y.tab.h
	g++ lex.yy.c y.tab.c dbms.cpp -ll -ly -o parser
lex.yy.c:dbms.l
	lex dbms.l
y.tab.c:dbms.y
	yacc -v -d dbms.y -Wconflicts-rr	
clean:
	rm -f parser lex.yy.c y.tab.c y.tab.h y.output
