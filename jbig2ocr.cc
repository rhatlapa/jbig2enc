// Copyright 2006 Google Inc. All Rights Reserved.
// Author: agl@imperialviolet.org (Adam Langley)
//
// Copyright (C) 2006 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <map>
#include <vector>
#include <algorithm>

#include <stdio.h>
#include <string.h>

#include <allheaders.h>
#include <pix.h>

#include "ocrResult.h"

// from tesseract
#include <baseapi.h>
#include <ocrclass.h>
using namespace tesseract;

#include <math.h>
#if defined(sun)
#include <sys/types.h>
#else
#include <stdint.h>
#endif

#define u64 uint64_t
#define u32 uint32_t
#define u16 uint16_t
#define u8  uint8_t

#include "jbig2ocr.h"

/*  
 *  Initialization of tesseract api
 */
void TesseractOcr::init() {
  this->api.Init("tesseract_data", lang);
  tesseract::PageSegMode pagesegmode = static_cast<tesseract::PageSegMode>(10);
  this->api.SetPageSegMode(pagesegmode);
}


/* 
 *  box contains info about position and size of PIX
 */
//OcrResult * recognizeLetter(PIX * pix, BOX * box) {
OcrResult * TesseractOcr::recognizeLetter(PIX * pix) {
  api.SetImage(pix);
  if (sourceResolution > 0) {
    api.SetSourceResolution(sourceResolution);
  } 
  api.SetPageSegMode(tesseract::PSM_SINGLE_WORD);
//   fprintf(stderr, "Getting UTF8 text\n");
  char * recognizedText = api.GetUTF8Text();
//   fprintf(stderr, "Recognized text %s\n", recognizedText);
  int *confidences = api.AllWordConfidences();
  int confidence = api.MeanTextConf();

/*
  // no improvement achieved
  if (confidence < 75) {
    api.Clear();
    api.SetImage(pix);
    api.SetPageSegMode(tesseract::PSM_SINGLE_WORD);
    char * recognizedWord = api.GetUTF8Text();
    int wordConfidence = api.MeanTextConf();
    fprintf(stderr, "Recognized as char %s with confidence %d vs recognized as word (textline) %s with confidence %d\n",
                          recognizedText, confidence, recognizedWord, wordConfidence);
    if (wordConfidence > confidence) {
      fprintf(stderr, "OCR Result improved\n");
      confidences = api.AllWordConfidences();
      recognizedText = recognizedWord;
      confidence = wordConfidence;
    }
  } else {
*/
//     fprintf(stderr, "Recognized as char %s with confidence %d\n", recognizedText, confidence);
//   }
  OcrResult * result = new OcrResult(pix);
  result->setCharsWithConfidences(recognizedText, confidences);
  result->setRecognizedTextWithMeanConfidence(recognizedText, confidence);
  api.Clear();
  return result;
}

void TesseractOcr::recognizeLetterDetailInfo(PIX * pix) {
  api.SetImage(pix);
  ETEXT_DESC * tessOcrResult = new ETEXT_DESC();
  api.Recognize(tessOcrResult);
  EANYCODE_CHAR * charResult = tessOcrResult->text;
  fprintf(stderr, "Recognized char %d; Confidence %d\n", charResult->char_code, charResult->confidence);
}

