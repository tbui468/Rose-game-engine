#ifndef INPUT_TYPE_H
#define INPUT_TYPE_H

namespace sqs {

enum class InputType {
    Close,
    LeftTap, 
    RightTap,
    DoubleTap, 
    FlickLeft,
    FlickRight,
    FlickUp,
    FlickDown
};

}

#endif //INPUT_TYPE
