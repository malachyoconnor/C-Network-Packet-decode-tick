pcolmake: pcolparse.c extract.cpp

	g++ -c -Wall pcolparse.c
	g++ -c -Wall extract.cpp

	g++ -Wall --pedantic pcolparse.o extract.o -I. -o output