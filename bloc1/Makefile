all: program exec

program: prova.cpp
	g++ prova.cpp -o program -lglut -lGL

exec: program
	./program

clean:
	rm -rf *.out program

