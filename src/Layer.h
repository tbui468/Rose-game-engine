#ifndef LAYER_H
#define LAYER_H

namespace rse {

class Layer {
    public:
        Layer(){}
        virtual ~Layer() {}
        virtual void Update() = 0;
        virtual void Draw() = 0;
};

}

#endif //LAYER_H
