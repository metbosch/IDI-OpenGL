all: main exec

main: main.cpp model.o
	g++ main.cpp model.o -o program -lglut -lGL -lGLU

exec: program
ifdef MODEL
	./program ${MODEL} &
else
	./program legoman.obj &
endif		

model.o: model.cpp
	g++ -c model.cpp -o model.o

clean:
	rm -rf *.out program *.exe *.o
