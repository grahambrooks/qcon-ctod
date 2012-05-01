

all	:	hello hello-test
	hello-test

hello	:	src/*.cpp
	clang++ -o hello -std=c++11 src/*.cpp -framework opencl

hello-test	:	test/*.cpp
	clang++ -o hello-test -std=c++11 test/*.cpp /usr/local/lib/libboost_unit_test_framework.a

clean:
	rm hello
	rm hello-test

