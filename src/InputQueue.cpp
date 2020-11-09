#include "InputQueue.h"

#include <SDL_events.h>

namespace sqs {


void InputQueue::PollEvents() {
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        if(event.type == SDL_QUIT) m_Inputs.push(InputType::Close);
    }
}


InputType InputQueue::NextInput(){
    InputType input = m_Inputs.front();
    m_Inputs.pop();
    return input;
}



}
