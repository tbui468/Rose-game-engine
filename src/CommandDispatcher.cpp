#include "CommandDispatcher.h"

extern const int32_t SCALE;
extern const int32_t WINDOW_WIDTH;
extern const int32_t WINDOW_HEIGHT;

namespace sqs {

void CommandDispatcher::SetEntityList(std::vector<std::shared_ptr<Entity>>* entityList) {
    m_EntityList = entityList;
}

CommandCode CommandDispatcher::Dispatch(const InputType& input, int mouseX, int mouseY) {

    switch(input) {
        case InputType::Close: 
            return CommandCode::Quit;
        case InputType::LeftTap: 
            for(std::shared_ptr<Entity> b: *m_EntityList) {
                if(b->x > static_cast<float>(WINDOW_WIDTH) * .5f)
                    b->MoveTo({0.0f, static_cast<float>(WINDOW_HEIGHT) * .5f});
                else
                    b->MoveTo({static_cast<float>(WINDOW_WIDTH) - 100, static_cast<float>(WINDOW_HEIGHT) * .5f});
            }
            return CommandCode::Success;
        case InputType::RightTap: 
            return CommandCode::Failed;
    }
    return CommandCode::Failed;
}


}
