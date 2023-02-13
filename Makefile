compiler = g++

main: main.o lexer.o parser.o
	$(compiler) -o scell main.o lexer.o parser.o
	./scell tables/copies | column -t -s '|'

rm:
	rm *.o
	rm scell
