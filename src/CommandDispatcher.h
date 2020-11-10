#ifndef COMMAND_DISPATCHER_H
#define COMMAND_DISPATCHER_H

#include <vector>
#include <memory>
#include "Button.h"
#include "InputType.h"
#include "CommandCode.h"



namespace sqs {

class CommandDispatcher {
    public:
        CommandDispatcher() = default;
        virtual ~CommandDispatcher(){}
        void SetButtonList(std::vector<std::shared_ptr<Button>>* _list);
        CommandCode Dispatch(const InputType& input, int mouseX, int mouseY);

    private:
        std::vector<std::shared_ptr<Button>>* m_ButtonList;
        //list of puzzleset
        //list of puzzles
        //list of fractals
};

}

#endif //COMMAND_DISPATCHER_H
