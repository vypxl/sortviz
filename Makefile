.PHONY: deps build embuild run emrun dockerbuild clean

deps:
	@mkdir -p build/native
	@cd build/native && conan install ../.. -s compiler.version=10

DEPSFAIL={ echo '\#\#\#\#\# Build failed, did you run `make deps`? \#\#\#\#\#'; false; }

build:
	@cmake -B build/native -DCMAKE_MODULE_PATH=./build/native || $(DEPSFAIL)
	@cmake --build ./build/native || $(DEPSFAIL)

embuild:
	@emcmake cmake -B build/web .
	@emmake make -C build/web

run: build
	@build/native/out/sortviz

emrun: embuild
	@emrun build/web/out/sortviz.html

dockerbuild:
	docker build -t sortviz .

clean:
	@rm -rf build/

.DEFAULT_GOAL := build
