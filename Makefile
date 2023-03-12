compiler = g++

main: minix.o lexer.o parser.o
	$(compiler) -o scell minix.o lexer.o parser.o
	./scell tables/example | column -t -s '|'

rm:
	rm *.o
	rm scell
