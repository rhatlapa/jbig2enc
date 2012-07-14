CC=g++
LEPTONICA=../../leptonica-1.69
TESSERACT=../../tesseract-ocr
TESSERACT_INCLUDES=-I${TESSERACT}/api -I${TESSERACT}/ccmain -I${TESSERACT}/ccutil -I${TESSERACT}/ccstruct -I${TESSERACT}/image -I${TESSERACT}/viewer -I${TESSERACT}/dict -I${TESSERACT}/classify -I${TESSERACT}/display -I${TESSERACT}/wordrec -I${TESSERACT}/cutil -I${TESSERACT}/textord
TESSERACT_LIBS=${TESSERACT}/api/.libs/libtesseract_api.a ${TESSERACT}/ccstruct/.libs/libtesseract_ccstruct.a ${TESSERACT}/ccutil/.libs/libtesseract_ccutil.a ${TESSERACT}/classify/.libs/libtesseract_classify.a ${TESSERACT}/cutil/.libs/libtesseract_cutil.a ${TESSERACT}/textord/.libs/libtesseract_textord.a ${TESSERACT}/dict/.libs/libtesseract_dict.a ${TESSERACT}/image/.libs/libtesseract_image.a ${TESSERACT}/viewer/.libs/libtesseract_viewer.a ${TESSERACT}/dict/.libs/libtesseract_dict.a ${TESSERACT}/cube/.libs/libtesseract_cube.a ${TESSERACT}/wordrec/.libs/libtesseract_wordrec.a ${TESSERACT}/ccmain/.libs/libtesseract_main.a   
#-Iccops

# For example, a fink MacOSX install:
# EXTRA=-I/sw/include/ -I/sw/include/libpng -I/sw/include/libjpeg -L/sw/lib
CFLAGS=-I${LEPTONICA}/src -Wall -I/usr/include -I/usr/local/include -I/usr/local/include/tesseract -L/usr/lib -L/usr/local/lib -O3 ${EXTRA} -fopenmp -D_REENTRANT
#CFLAGS=-I${LEPTONICA}/src -Wall -I/usr/include -I/usr/local/include -L/usr/lib -L/usr/local/lib -O3 ${EXTRA}

#-fopnmp -D_REENTRANT is used for parallelization
jbig2: libjbig2enc.a jbig2.cc
	$(CC) -o jbig2 jbig2.cc -L. -ljbig2enc -lpng -ljpeg -ltiff -lgif -lm -lz -lwebp ${LEPTONICA}/src/.libs/liblept.a -ltesseract $(CFLAGS) 
	#$(CC) -o jbig2 jbig2.cc -L. -ljbig2enc ${LEPTONICA}/src/.libs/liblept.a $(CFLAGS) -lpng -ljpeg -ltiff -lgif -lm 


libjbig2enc.a: jbig2enc.o jbig2arith.o jbig2sym.o jbig2comparator.o jbig2ocr.o ocrResult.o result.o
	ar -rcv libjbig2enc.a jbig2enc.o jbig2arith.o jbig2sym.o jbig2comparator.o jbig2ocr.o result.o ocrResult.o 

jbig2enc.o: jbig2enc.cc jbig2arith.h jbig2sym.h jbig2structs.h jbig2segments.h jbig2comparator.h
	$(CC) -c jbig2enc.cc $(CFLAGS)
jbig2comparator.o: jbig2comparator.cc jbig2comparator.h jbig2ocr.h
	$(CC) -c jbig2comparator.cc $(CFLAGS)
jbig2ocr.o: jbig2ocr.cc jbig2ocr.h ocrResult.h jbig2comparator.o
	$(CC) -c jbig2ocr.cc $(CFLAGS)
ocrResult.o: ocrResult.h ocrResult.cc
	$(CC) -c ocrResult.cc $(CFLAGS)
result.o: result.h result.cc
	$(CC) -c result.cc $(CFLAGS)
jbig2arith.o: jbig2arith.cc jbig2arith.h
	$(CC) -c jbig2arith.cc $(CFLAGS)
jbig2sym.o: jbig2sym.cc jbig2arith.h
	$(CC) -c jbig2sym.cc -DUSE_EXT $(CFLAGS)
clean:
	rm -f *.o jbig2 libjbig2enc.a
