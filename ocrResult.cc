#include "ocrResult.h"
#include <cmath>
#include <string.h>

#include <allheaders.h>
#include <pix.h>


using namespace std;


void OcrResult::addRecognizedChar(char character, int confidence) {
  this->charConfidences.insert(pair<char, int>(character, confidence));
  this->numOfChars++;
}


/**
 *  *chars .......... array of recognized chars
 *  *confidences .... array of confidences of chars 
 *		      (connected by index number = position in the array)
 *      -- last item in array is -1
 */
void OcrResult::setCharsWithConfidences(char *chars, int *confidences) {
  int i = 0;
  while (confidences[i] != -1) {
    this->charConfidences.insert(pair<char, int>(chars[i], confidences[i]));
    i++;
  }
  this->numOfChars+=i;
  this->recognizedText = chars;
}

void OcrResult::setRecognizedTextWithMeanConfidence(char * chars, int meanConfidence) {
  this->recognizedText = chars;
  this->meanConfidence = meanConfidence;
}

/**
 * counts number of pixels in defined area using box struct
 */
l_float64 getNumOfPixelsInRegion(PIX * pix, BOX * box) {
  l_float64 psum;
  pixSumPixelValues(pix, box, &psum);
  return psum;
}


/**
 * counts distance between PIX int this OcrResult and another PIX
 * takes into account sizes of PIX
 */
float OcrResult::getPixDistance(PIX * otherPix) {
  float distance = 0.0;
  PIX *thisPix = this->pix;
  distance += fabs(thisPix->w - otherPix->w);
  distance += fabs(thisPix->h - otherPix->h);

  distance *= 0.3;

  PIX * pixd;
  pixd = pixXor(NULL, thisPix, otherPix);

  l_int32 init = 0;
  l_int32 *thisCount = &init;
  l_int32 *diffCount = &init;

  // counting number of ON pixels in thisPix
  if (pixCountPixels(thisPix, thisCount, NULL)) {
    fprintf(stderr, "Unable to count pixels\n");
    pixDestroy(&pixd);
    return 0;
  }

  // counting number of ON pixels in thisPix
  if (pixCountPixels(pixd, diffCount, NULL)) {
    fprintf(stderr, "Unable to count pixels\n");
    pixDestroy(&pixd);
    return 0;
  }

  l_int32 w, h, d;

  pixGetDimensions(pixd, &w, &h, &d);
  l_uint32 xParts = 3 * pixd->wpl;
  l_uint32 yParts = 3;

  l_uint32 segmentWidth = w / xParts;
  l_uint32 segmentHeight = h / yParts;

  l_uint32 localArea = segmentWidth * segmentHeight;

  for (l_uint32 i = 0; i < xParts; i++) {
    for (l_uint32 j = 0; j < yParts; j++) {
      BOX * box = boxCreate(i*segmentWidth,j*segmentHeight, segmentWidth, segmentHeight);
      int localDiffPixels = getNumOfPixelsInRegion(pixd, box);      
      float percentDiff = (float)localDiffPixels / localArea;
      if (percentDiff < 0.05) {
        distance += percentDiff * 0.1;
      } else if (percentDiff < 0.1) {
        distance += percentDiff * 0.3;
      } else if (percentDiff < 0.2) {
        distance += percentDiff * 1.2;
      } else if (percentDiff < 0.4) {
        distance += percentDiff * 3;
      } else if (percentDiff < 0.6) {
        distance += percentDiff * 5;
      } else {
        distance += percentDiff * 10;
      }
#ifdef OCR_DEBUGGING
      fprintf(stderr, "diffPixels found: %d\n", localDiffPixels);
#endif
      boxDestroy(&box);
    }
  }

//   distance += ((*diffCount)/(thisPix->w * thisPix->h));
  //distance += (*diffCount)/(*thisCount);

  return distance;
}



/**
 * counts distance of two ocr results (it should be checked before, that we compare only ocr results
 * with same lettersRecognized
 */
float OcrResult::getDistance(OcrResult * ocrResult) {
    float distance = 0.0;
    distance += strcmp(this->getRecognizedText(),ocrResult->getRecognizedText());
    distance += (getPixDistance(ocrResult->pix)*0.6);
    float confDiff = fabs(this->getConfidence()-ocrResult->getConfidence());

    float uncertainity = 0.0;
    if (this->getConfidence() > ocrResult->getConfidence()) {
      uncertainity = 100 - ocrResult->getConfidence();
    } else {
      uncertainity = 100 - this->getConfidence();
    }
    distance += (((confDiff/3.0)+1) * (uncertainity + 1))*0.3;
    distance *= (uncertainity/50);

    return distance;
}
