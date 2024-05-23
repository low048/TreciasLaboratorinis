.PHONY: all program test examples clean

all: program test examples

program: main.cpp studentas.cpp funkcijos.cpp
	g++ main.cpp studentas.cpp funkcijos.cpp -o program.exe

test:
	g++ -std=c++23 -D_POSIX_C_SOURCE=200809L -D__cpp_lib_containers_ranges=202202L -I./googletest/include -I./googletest -I. -pthread \
        vector_unit_tests.cpp \
        ./googletest/src/gtest-all.cc \
        ./googletest/src/gtest_main.cc -o vector_unit_tests.exe

examples:
	g++ -std=c++23 -D__cpp_lib_containers_ranges=202202L vector_examples.cpp -o vector_examples.exe

clean:
	rm -f program.exe vector_unit_tests.exe vector_examples.exe
