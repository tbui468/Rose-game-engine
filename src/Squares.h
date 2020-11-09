#ifndef SQUARES_H
#define SQUARES_H


#include <iostream>
#include <SDL.h>

#include "Utility.h"
#include "SquaresCommon.h"
#include "Quad.h"
#include "Button.h"
#include "InputQueue.h"
#include "CommandDispatcher.h"
#include "AnimationTimer.h"

namespace sqs {

static int Init() {
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL_Init error!!!" << SDL_GetError() << std::endl;
        return 1;
    }
    return 0;
}


}


#endif //SQUARES_H
