#ifndef INPUT_QUEUE_H
#define INPUT_QUEUE_H

#include <queue>
#include "InputType.h"
#include "SquaresCommon.h"

namespace sqs {


class InputQueue {
    public:
        InputQueue() = default;
        virtual ~InputQueue(){}
        void PollEvents();
        InputType NextInput();
        size_t Size() const {return m_Inputs.size(); }
        bool Empty() const {return m_Inputs.empty(); }
        void Clear() {while(!m_Inputs.empty()) m_Inputs.pop();}
        CartCoordsi GetMouseCoords();
    private:
    private:
        std::queue<InputType> m_Inputs;
        uint32_t m_Decay;
};

}

#endif //INPUT_QUEUE_H
