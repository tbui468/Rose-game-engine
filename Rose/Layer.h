#ifndef LAYER_H
#define LAYER_H

namespace rose {

class Layer {
    public:
        Layer(){}
        virtual ~Layer() {}
        virtual bool Update(double deltaTime, rose::InputType input, const glm::ivec2& mousePos) = 0;
        virtual void Draw() = 0;
    private:
};

}

#endif //LAYER_H
