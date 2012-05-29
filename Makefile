BOOST_LIB_PATH = /usr/local/lib
LT = dylib
#LT = a
BOOST_LIBS = \
	$(BOOST_LIB_PATH)/libboost_system.$(LT) \
	$(BOOST_LIB_PATH)/libboost_timer.$(LT)

GTEST_PATH	=	tools/gtest
GTEST_INCLUDE	=	$(GTEST_PATH)/include

GTEST_LIBS	=	$(GTEST_PATH)/lib/libgtest.a \
			$(GTEST_PATH)/lib/libgtest_main.a

all	:	qcdf \
		qcdf-test


src/program.cpp: src/kernel_program.cl
	ruby tools/embed_program.rb $< $@

qcdf	:	src/*.cpp	src/program.cpp
	clang++ -O1 -faddress-sanitizer -o qcdf -std=c++11 -Xclang "-stdlib=libc++" -Xlinker -lc++ src/*.cpp -framework opencl
#	qcdf .

qcdf-test	:	test/*.cpp
	clang++ -I $(GTEST_INCLUDE) -o qcdf-test -std=c++11 test/*.cpp $(GTEST_LIBS)
	qcdf-test

clean:
	rm -f qcdf
	rm -f qcdf-test

