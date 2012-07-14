#ifndef _OCRRESULT_H_
#define _OCRRESULT_H_

#include <allheaders.h>
#include <pix.h>
#include <map>
#include "result.h"

/**
 * Standard structure for holding results of running OCR
 */
class OcrResult : public Result {
  private:
    std::map<char, int> charConfidences; // confidences for each recognized character
    char * recognizedText; // the recognized text
    int numOfChars; // the number of characters recognized
    int meanConfidence; // the mean confidence of charConfidences

  protected:
    // counts similarity distance between two PIXes without using results of OCR
    float getPixDistance(PIX * pix);  
  public:
    OcrResult() {
      this->numOfChars = 0;
    }

    OcrResult(PIX *pix) {
      this->pix = pix;
      this->numOfChars = 0;
    }

    ~OcrResult() {
      charConfidences.clear();
    }

    std::map<char, int> getCharConfidences() {
      return this->charConfidences;
    }
    
	/*int getConfidence() {*/
	/*if (charConfidences.size()==0) {*/
	/*return -1;*/
	/*}*/
	/*std::map<char, int>::iterator it;*/
	/*int sum = 0;*/
	/*for (it = this->charConfidences.begin(); it != this->charConfidences.end(); it++) {*/
	/*sum += it->second;*/
	/*}*/
	/*return sum / charConfidences.size();*/
	/*}*/

    /**
     * Returns mean confidence of recognized text
     */
    int getConfidence() {
      return this->meanConfidence;
    }

    void setConfidence(int meanConfidence) {
      this->meanConfidence = meanConfidence;
    }

    /**
     * Returns recognized text by OCR engine
     */
    char * getRecognizedText() {
      return recognizedText;
    }

    /**
     * Returns number of recognized characters
     */
    int getNumOfChars() {
      return numOfChars;
    }

    /**
     * Adds new character with its recognition confidence to set of recognized symbols from provided PIX
     */
    void addRecognizedChar(char symbol, int confidence);

    /**
     *  *chars .......... array of recognized chars
     *  *confidences .... array of confidences of chars 
			  (connected by index number = position in the array)
     *  last item in array has value -1
     */
    void setCharsWithConfidences(char *chars, int *confidences);

    /**
     * Sets recognized text and mean confidence, confidence is only stored as whole for the entire recognized text
     */
    void setRecognizedTextWithMeanConfidence(char * chars, int meanConfidence);

    /**
     * Function counting similarity distance of two ocrResults
     *
     * If the result is 0, then they are exactly the same,
     * Higher number (distance) means that they are more different than lower number (distance)
     *
     * If their difference is lower or equal to 0.285, symbols are considered equivalent
     *
     */
    virtual float getDistance(OcrResult * ocrResults);
  
};
#endif
