.PHONY: deps build embuild run emrun clean

deps:
	@mkdir -p build
	@cd build && conan install .. -s compiler.version=10

build: deps
	@cmake -B build -DCMAKE_MODULE_PATH=./build
	@cmake --build ./build

embuild:
	@emcmake cmake -B build/web .
	@emmake make -C build/web

run: build
	@build/out/sortviz

emrun: embuild
	@emrun build/web/out/sortviz.html

clean:
	@rm -rf build/

.DEFAULT_GOAL := build
