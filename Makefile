CC=g++
LEPTONICA=../../leptonica-1.69
# For example, a fink MacOSX install:
# EXTRA=-I/sw/include/ -I/sw/include/libpng -I/sw/include/libjpeg -L/sw/lib
CFLAGS=-I${LEPTONICA}/src -Wall -I/usr/include -L/usr/lib -O3 ${EXTRA}

jbig2: libjbig2enc.a jbig2.cc
	$(CC) -o jbig2 jbig2.cc -L. -ljbig2enc ${LEPTONICA}/src/.libs/liblept.a $(CFLAGS) -lpng -ljpeg -ltiff -lgif -lwebp -lm -lz

libjbig2enc.a: jbig2enc.o jbig2arith.o jbig2sym.o result.o jbig2comparator.o
	ar -rcv libjbig2enc.a jbig2enc.o jbig2arith.o jbig2sym.o result.o jbig2comparator.o

jbig2enc.o: jbig2enc.cc jbig2arith.h jbig2sym.h jbig2structs.h jbig2segments.h jbig2comparator.h
	$(CC) -c jbig2enc.cc $(CFLAGS)
jbig2comparator.o: jbig2comparator.cc jbig2comparator.h
	$(CC) -c jbig2comparator.cc $(CFLAGS)
jbig2arith.o: jbig2arith.cc jbig2arith.h
	$(CC) -c jbig2arith.cc $(CFLAGS)
jbig2sym.o: jbig2sym.cc jbig2arith.h
	$(CC) -c jbig2sym.cc -DUSE_EXT $(CFLAGS)
result.o: result.h result.cc
	$(CC) -c result.cc $(CFLAGS)

clean:
	rm -f *.o jbig2 libjbig2enc.a
