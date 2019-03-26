LIBS=-pthread -ldl -lsfml-graphics -lsfml-window -lsfml-system -lGL

.PHONY: build run clean

build:
	@mkdir -p out
	@g++ -I include -o out/viz src/main.cpp src/glad.c $(LIBS)

run: build
	@out/viz

clean:
	@rm -r out/

.DEFAULT_GOAL := build
