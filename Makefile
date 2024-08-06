.PHONY: build setup clean

build: setup 
	make -C build

setup: src/CMakeLists.txt
	mkdir -p build
	cmake -B build src/

clean:
	rm -rf build
