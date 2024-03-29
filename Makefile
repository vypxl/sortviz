.PHONY: deps build embuild run emrun dockerbuild clean

deps:
	@mkdir -p build/native
	@mkdir -p build/web
	@cd build/native && conan install ../.. -s compiler=gcc -s compiler.version=10

DEPSFAIL={ echo '\#\#\#\#\# Build failed, did you run `make deps`? \#\#\#\#\#'; false; }

CMAKE_OPTS=-DCMAKE_EXPORT_COMPILE_COMMANDS=1 -GNinja

build:
	@cmake -B build/native -DCMAKE_MODULE_PATH=./build/native $(CMAKE_OPTS) || $(DEPSFAIL)
	@ln -fs native/compile_commands.json build/compile_commands.json
	@cmake --build ./build/native || $(DEPSFAIL)
	@mkdir -p out
	@cp build/native/out/sortviz out/sortviz

embuild:
	@emcmake cmake -B build/web $(CMAKE_OPTS)
	@ln -fs web/compile_commands.json build/compile_commands.json
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
