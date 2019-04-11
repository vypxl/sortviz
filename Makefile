LIBS=-pthread -ldl -lSDL -lGL -lfreetype

FILES=$(wildcard src/*.cpp) $(wildcard src/sorts/*.cpp)

.PHONY: build run clean

build:
	@mkdir -p out
	@g++ -I include -o out/sortviz $(FILES) src/glad.c $(LIBS)

embuild:
	@mkdir -p out/web
	@rm -r out/web
	@mkdir -p out/web
	@em++ -std=c++11 -s USE_PTHREADS -s USE_SDL=1 -s USE_FREETYPE=1 -s FULL_ES3=1 -s USE_WEBGL2=1 -I include -o out/web/sortviz.html $(FILES) --preload-file ./res@/res --preload-file ./shaders@/shaders

embuild_asmjs:
	@mkdir -p out/web
	@rm -r out/web
	@mkdir -p out/web
	@em++ -std=c++11 -s WASM=0 -s USE_PTHREADS -s USE_SDL=1 -s USE_FREETYPE=1 -s FULL_ES3=1 -s USE_WEBGL2=1 -I include -o out/web/sortviz.html $(FILES) --preload-file ./res@/res --preload-file ./shaders@/shaders

run: build
	@out/sortviz

clean:
	@rm -r out/

.DEFAULT_GOAL := build
