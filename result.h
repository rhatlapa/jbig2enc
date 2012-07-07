#ifndef _JBIG2_RESULT_H_
#define _JBIG2_RESULT_H_

#include <allheaders.h>
#include <pix.h>


/**
 * Structure used as reference (fallback) point, which allows to count distance of two PIXes without using OCR
 */
class Result {
  protected:
    PIX * pix;

  public:
    Result() {}
    Result(PIX *pix) {
      this->pix = pix;
    }

    ~Result() {
      this->pix = NULL;
    }

    PIX * getPix() {
      return pix;
    }

    void setPix(PIX * pix) {
      this->pix = pix;
    }

    virtual float getDistance(Result *result);

};

#endif
