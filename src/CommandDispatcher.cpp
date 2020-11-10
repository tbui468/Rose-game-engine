#include "CommandDispatcher.h"

namespace sqs {

void CommandDispatcher::SetButtonList(std::vector<std::shared_ptr<Button>>* _list) {
    m_ButtonList = _list;
}

CommandCode CommandDispatcher::Dispatch(const InputType& input, int mouseX, int mouseY) {
    
    switch(input) {
        case InputType::Close: 
            return CommandCode::Quit;
        case InputType::LeftTap: 
            for(std::shared_ptr<Button> b: *m_ButtonList) {
                if(b->PointCollision(mouseX, mouseY))
                    b->MoveTo({200, 200});
            }
            return CommandCode::Success;
        case InputType::RightTap: 
            for(std::shared_ptr<Button> b: *m_ButtonList) b->MoveTo({0, 0});
            return CommandCode::Success;
    }
    return CommandCode::Failed;
}


}
