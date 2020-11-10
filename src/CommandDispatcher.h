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
        void SetEntityList(std::vector<std::shared_ptr<Entity>>* entityList);
        CommandCode Dispatch(const InputType& input, int mouseX, int mouseY);

    private:
        std::vector<std::shared_ptr<Entity>>* m_EntityList;
};

}

#endif //COMMAND_DISPATCHER_H
