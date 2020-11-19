#ifndef INPUT_DISPATCHER_H
#define INPUT_DISPATCHER_H

namespace sqs {

enum class InputType { //the namespace will differentiate between this and rose InputType
    SingleTap,
    DoubleTap,
    FlickLeft,
    FlickRight,
    FlickUp,
    FlickDown
};

class InputDispatcher {
    public:
        InputDispatcher() = default;
        virtual ~InputDispatcher() {}
    private:
};


}


#endif //INPUT_DISPATCHER_H
