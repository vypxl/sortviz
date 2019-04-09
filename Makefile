LIBS=-pthread -ldl -lSDL -lGL -lfreetype

FILES=$(wildcard src/*.cpp) $(wildcard src/sorts/*.cpp)

.PHONY: build run clean

build:
	@mkdir -p out
	@g++ -I include -o out/sortviz $(FILES) src/glad.c $(LIBS)

# embuild:
# 	@mkdir -p out/web
# 	@em++ -std=c++11 -Wno-delete-non-virtual-dtor -I include -o out/web/sortviz.html $(FILES) src/glad.c $(LIBS)

run: build
	@out/sortviz

clean:
	@rm -r out/

.DEFAULT_GOAL := build
