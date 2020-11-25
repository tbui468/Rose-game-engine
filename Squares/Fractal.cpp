#include "Fractal.h"


namespace sqs {
    int GetFractalSize(BaseFractal* f) {
        if(dynamic_cast<Fractal<int>*>(f)) return 1;
        else if(dynamic_cast<Fractal<glm::imat2>*>(f)) return 2;
        else if(dynamic_cast<Fractal<glm::imat4>*>(f)) return 4;
        assert(false);
        return 0;
    }
}
