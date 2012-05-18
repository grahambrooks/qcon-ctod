
all	:	qcdf qcdf-test
	hello-test

src/program.cpp: src/kernel_program.cl
	ruby tools/embed_program.rb $< $@

qcdf	:	src/*.cpp	src/program.cpp
	clang++ -o qcdf -std=c++11 -Xclang "-stdlib=libc++" -Xlinker -lc++ src/*.cpp -framework opencl /usr/local/lib/libboost_filesystem-mt.dylib /usr/local/lib/libboost_system-mt.dylib /usr/local/lib/libboost_timer-mt.dylib

qcdf-test	:	test/*.cpp
	clang++ -o qcdf-test -std=c++11 test/*.cpp /usr/local/lib/libboost_unit_test_framework-mt.a

clean:
	rm -f qcdf
	rm -f qcdf-test

