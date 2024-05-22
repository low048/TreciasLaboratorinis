.PHONY: all program clean

all: program

program:
	g++ main.cpp studentas.cpp funkcijos.cpp -o program.exe

test:
	g++ -std=c++20 -D_POSIX_C_SOURCE=200809L -D__cpp_lib_containers_ranges=201907L -I./googletest/include -I./googletest -I. -pthread \
        vector_unit_test.cpp \
        ./googletest/src/gtest-assertion-result.cc \
        ./googletest/src/gtest-death-test.cc \
        ./googletest/src/gtest-filepath.cc \
        ./googletest/src/gtest-matchers.cc \
        ./googletest/src/gtest-port.cc \
        ./googletest/src/gtest-printers.cc \
        ./googletest/src/gtest-test-part.cc \
        ./googletest/src/gtest-typed-test.cc \
        ./googletest/src/gtest.cc \
		./googletest/src/gtest_main.cc -o test_program.exe

clean:
	rm -f program.exe test_program.exe
