#ifndef LAYER_H
#define LAYER_H

namespace rose {

class Layer {
    public:
        Layer(){}
        virtual ~Layer() {}
        virtual void Update() = 0;
        virtual void Draw() = 0;
};

}

#endif //LAYER_H
