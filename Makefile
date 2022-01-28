.PHONY: deps build embuild run emrun dockerbuild clean

deps:
	@mkdir -p build/native
	@mkdir -p build/web
	@cd build/native && conan install ../.. -s compiler=gcc -s compiler.version=10

DEPSFAIL={ echo '\#\#\#\#\# Build failed, did you run `make deps`? \#\#\#\#\#'; false; }

build:
	@cmake -B build/native -DCMAKE_MODULE_PATH=./build/native -GNinja || $(DEPSFAIL)
	@cmake --build ./build/native || $(DEPSFAIL)
	@mkdir -p out
	@cp build/native/out/sortviz out/sortviz

embuild:
	@emcmake cmake -B build/web -GNinja
	@cmake --build ./build/web
	@mkdir -p out/web
	@cp -Tr build/web/out out/web

run: build
	@out/sortviz

emrun: embuild
	@emrun out/web/sortviz.html

dockerbuild:
	@DOCKER_BUILDKIT=1 docker build -o out .

clean:
	@rm -rf build/
	@rm -rf out/

.DEFAULT_GOAL := build
