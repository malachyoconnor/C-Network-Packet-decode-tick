pcolmake: pcolparse.c extract.cc

	g++ -c -Wall pcolparse.c
	g++ -c -Wall extract.cc

	g++ -Wall --pedantic pcolparse.o extract.o -I. -o output