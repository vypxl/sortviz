.PHONY: build embuild run emrun clean

build:
	@cmake -B build/native .
	@make -C build/native

embuild:
	@emcmake cmake -B build/web .
	@emmake make -C build/web

run: build
	@build/native/out/sortviz

emrun: embuild
	@emrun build/native/out/sortviz.html

clean:
	@rm -rf build/

.DEFAULT_GOAL := build
