LIBS=-pthread -ldl -lsfml-graphics -lsfml-window -lsfml-system -lGL

FILES=$(wildcard src/*.cpp) $(wildcard src/sorts/*.cpp)

.PHONY: build run clean

build:
	@mkdir -p out
	@g++ -I include -o out/sortviz $(FILES) src/glad.c $(LIBS)

run: build
	@out/sortviz

clean:
	@rm -r out/

.DEFAULT_GOAL := build
