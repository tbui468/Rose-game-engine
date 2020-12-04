#include "Fractal.h"


namespace sqs {
    int GetFractalSize(BaseFractal* f) {
        if(dynamic_cast<Fractal<int>*>(f)) return 1;
        else if(dynamic_cast<Fractal<glm::imat2>*>(f)) return 2;
        else if(dynamic_cast<Fractal<glm::imat4>*>(f)) return 4;
        assert(false);
        return 0;
    }

    bool Contains(BaseFractal* f, const glm::ivec2& index) {
        if(dynamic_cast<Fractal<int>*>(f)) return dynamic_cast<Fractal<int>*>(f)->Contains(index);
        if(dynamic_cast<Fractal<glm::imat2>*>(f)) return dynamic_cast<Fractal<glm::imat2>*>(f)->Contains(index);
        if(dynamic_cast<Fractal<glm::imat4>*>(f)) return dynamic_cast<Fractal<glm::imat4>*>(f)->Contains(index);
        assert(false);
        return false;
    }


    int GetSubElementsI(BaseFractal* f, const glm::ivec2& index) {
        if(dynamic_cast<Fractal<int>*>(f)) return dynamic_cast<Fractal<int>*>(f)->GetSubElementsI(index);
        if(dynamic_cast<Fractal<glm::imat2>*>(f)) return dynamic_cast<Fractal<glm::imat2>*>(f)->GetSubElementsI(index);
        if(dynamic_cast<Fractal<glm::imat4>*>(f)) return dynamic_cast<Fractal<glm::imat4>*>(f)->GetSubElementsI(index);
        assert(false);
        return false;
    }
}
