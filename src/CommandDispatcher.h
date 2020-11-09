#ifndef COMMAND_DISPATCHER_H
#define COMMAND_DISPATCHER_H


namespace sqs {

class CommandDispatcher {
    public:
        CommandDispatcher() = default;
        virtual ~CommandDispatcher(){}
//        void Dispatch(InputType input);

    private:
        //will include shared pointers to lists of entities (Fractal, PuzzleSet, Buttons, etc)
};

}

#endif //COMMAND_DISPATCHER_H
