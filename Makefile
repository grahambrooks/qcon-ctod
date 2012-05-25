BOOST_LIB_PATH = /usr/local/lib
LT = dylib
#LT = a
BOOST_LIBS = \
	$(BOOST_LIB_PATH)/libboost_system.$(LT) \
	$(BOOST_LIB_PATH)/libboost_timer.$(LT)

all	:	qcdf qcdf-test
	hello-test

src/program.cpp: src/kernel_program.cl
	ruby tools/embed_program.rb $< $@

qcdf	:	src/*.cpp	src/program.cpp
	clang++ -O0 -faddress-sanitizer -o qcdf -std=c++11 -Xclang "-stdlib=libc++" -Xlinker -lc++ src/*.cpp -framework opencl $(BOOST_LIBS)
#	qcdf .

qcdf-test	:	test/*.cpp
	clang++ -o qcdf-test -std=c++11 test/*.cpp /usr/local/lib/libboost_unit_test_framework-mt.a

clean:
	rm -f qcdf
	rm -f qcdf-test

