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

all	:	qcdf-app \
		qcdf-test


src/program.cpp: src/kernel_program.cl
	ruby tools/embed_program.rb $< $@

qcdf-app	:	src/*.cpp src/*.hpp digest_program.o
	clang++ -O1 -faddress-sanitizer -o qcdf-app -std=c++11 -Xclang "-stdlib=libc++" -Xlinker -lc++ src/*.cpp -framework opencl digest_program.o

digest_program.o: src/digest_program.c
	clang -c src/digest_program.c

qcdf-test	:	test/*.cpp src/*.c src/*.hpp digest_program.o
	clang++ -I $(GTEST_INCLUDE) -o qcdf-test -std=c++11 test/*.cpp digest_program.o $(GTEST_LIBS)
	qcdf-test

clean:
	rm -f qcdf-app
	rm -f qcdf-test
	rm -f digest_program.o
