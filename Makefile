.PHONY: all example run_example test run_test clean

example: src/CMakeLists.txt
	mkdir -p build
	cmake -B build src/
	make -C build

run_example: build/mini
	@build/mini

test: test/CMakeLists.txt
	mkdir -p btest
	cmake -B btest test/
	make -C btest

run_test: btest/miniTest
	@btest/miniTest

clean:
	rm -rf build
	rm -rf btest
