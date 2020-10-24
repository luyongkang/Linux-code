debug:main.o sleep.o
	g++ -o debug main.o sleep.o

main.o:main.cpp sleep.h
	g++ -c main.cpp

sleep.o:sleep.cpp sleep.h
	g++ -c sleep.cpp

clean:
	rm -rf main.o sleep.o debug