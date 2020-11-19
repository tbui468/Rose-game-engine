#ifndef LAYER_H
#define LAYER_H

namespace rose {

class Layer {
    public:
        Layer(){}
        virtual ~Layer() {}
        virtual bool Update(double deltaTime) = 0;
        virtual void Draw() = 0;
    private:
};

}

#endif //LAYER_H
