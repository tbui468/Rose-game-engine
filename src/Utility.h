#ifndef UTILITY_H
#define UTILITY_H

#include <math.h>

namespace sqs{

float Sigmoid(float _t) {
    return 1.0f / (1.0f + exp(-15.0f * (_t - 0.5f)));
}

}


#endif //UTILITY_H
