/*
 * =====================================================================================
 *
 *       Filename:  jbig2ocr.h
 *
 *    Description:  Class defining methods needed for running an OCR engine
 *
 *        Version:  1.0
 *        Created:  01/10/2012 03:11:54 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Radim Hatlapatka (radim), hata.radim@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef _JBIG2OCR_H_
#define _JBIG2OCR_H_

#include <pix.h>
#include "ocrResult.h"
#include <baseapi.h>

/**
 * Interface class for using an OCR engine in order to get text recognition results
 * This class shouldn't be used directly, but it should be used for defining children classes with specific OCR engine
 */
class OcrEngine {
  protected:
    const char * lang; // language used for text recognition, should be in format acceptable by used OCR engine
    int sourceResolution; // resolution of the source image, can be used to improve quality of the text recognition

    public:
		
	  /*
       * initializes language for OCR and sourceResolution in ppi
	   */
	  OcrEngine(const char * lang, int sourceResolution) {
	    this->lang = lang;
        this->sourceResolution = sourceResolution;
	  }

      void setResolution(int ppi) {
        this->sourceResolution = ppi;
      }

      ~OcrEngine() {};

      /**
       * Method used to initialized the OCR engine
       *
       * Given separately to minimize OCR engine initializations which is a computationally expensive operation
       */
      virtual void init() = 0;

      /**
       * Method used to recognize text and give additional text recognition information in specialized structure 
       * OcrResult
       */
      virtual OcrResult * recognizeLetter(PIX * pix) = 0; 
};

/**
 * Implementation of the OCR engine interface using Tesseract
 */
class TesseractOcr : public OcrEngine {
  private:
	tesseract::TessBaseAPI api; // API of the Tesseract OCR engine, should be initialized with init() function

  public:
	TesseractOcr(const char * lang) : OcrEngine(lang,0) { }
    TesseractOcr(const char * lang, int sourceResolution) : OcrEngine(lang, sourceResolution) {}

    ~TesseractOcr() {
      api.End();
	}

    // initializes Tesseract OCR engine API
    void init();

    /**
     * Returns recognition results given by Tesseract OCR
     */
    OcrResult * recognizeLetter(PIX * pix);

    // just for testing purposes, will be removed in the future
    void recognizeLetterDetailInfo(PIX * pix);
};

#endif
