
#include "result.h"
#include "jbig2comparator.h"

float Result::getDistance(Result *result) {
  return 1 - areEquivalent(this->getPix(), result->getPix());
}


