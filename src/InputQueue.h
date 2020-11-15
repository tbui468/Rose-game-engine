#ifndef INPUT_QUEUE_H
#define INPUT_QUEUE_H

#include <queue>

#include <glm/glm.hpp>

#include "InputType.h"
#include "Utility.h"
#include "Window.h"

namespace rose {


class InputQueue {
    public:
        InputQueue(std::shared_ptr<Window> window) : m_Window(window) {}
        virtual ~InputQueue(){}
        void PollEvents();
        InputType NextInput();
        size_t Size() const {return m_Inputs.size(); }
        bool Empty() const {return m_Inputs.empty(); }
        void Clear() {while(!m_Inputs.empty()) m_Inputs.pop();}
        glm::ivec2 GetMouseCoords();
    private:
    private:
        std::queue<InputType> m_Inputs;
        uint32_t m_Decay;
        std::shared_ptr<Window> m_Window;
};

}

#endif //INPUT_QUEUE_H
