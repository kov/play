CXX ?= g++
NUM_JOBS=$(filter -j%,$(MAKEFLAGS))

build/veccpp: Makefile build/benchmark/src/libbenchmark.a veccpp.cc
	$(CXX) -c -O3 -std=c++11 -isystem benchmark/include -o build/veccpp.o veccpp.cc
	$(CXX) -o build/veccpp build/veccpp.o build/benchmark/src/libbenchmark.a

build/benchmark/CMakeCache.txt:
	cmake -B build/benchmark -DBENCHMARK_DOWNLOAD_DEPENDENCIES=on -DCMAKE_BUILD_TYPE=Release benchmark/

build/benchmark/src/libbenchmark.a: Makefile build/benchmark/CMakeCache.txt
	make $(NUM_JOBS) -C build/benchmark

clean:
	rm -rf build