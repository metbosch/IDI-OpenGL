ifdef MODEL
all: main exec
else
all: main
endif

main: main.cpp model.o
	g++ main.cpp model.o -o program -lglut -lGL

tetera: tetera.cpp
	g++ tetera-full.cpp -o program -lglut -lGL

2triangles: 2triangles.cpp
	g++ 2triangles.cpp -o program -lglut -lGL

snowman: snowman.cpp
	g++ snowman.cpp -o program -lglut -lGL

loadModel: load-dinamic-model.cpp model.o
	g++ oad-dinamic-model.cpp model.o -o program -lglut -lGL

exec: program
	./program ${MODEL} &

model.o: model.cpp
	g++ -c model.cpp -o model.o

clean:
	rm -rf *.out program *.exe *.o