RazorWire: main.o Object.o Player.o Track.o
	g++ main.o Object.o Player.o Track.o -o RazorWire -lsfml-graphics -lsfml-window -lsfml-system -lGLU

main.o: main.cpp main.h
	g++ -c main.cpp

Object.o: Object.cpp Object.h
	g++ -c Object.cpp

Player.o: Player.cpp Player.h
	g++ -c Player.cpp

Track.o: Track.cpp Track.h
	g++ -c Track.cpp
