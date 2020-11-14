#include "Rose.h" //by any other name would have just as many memory leaks, and break as many cups

class MainLayer: public rose::Layer{
    public:
        MainLayer() {
            //m_StartButton = new Ref<Button>();
            //
            //create a bunch of instances
            //m_EntityList->Push(instances);
        }
        virtual ~MainLayer() {
        }
        virtual void Update() override {
            //if InputQueue.Next() == Input::LeftClick
                //startButton->MoveTo(....)
            //if InputQueue.Next() == Input::RightClick
                //for(Entity& e: m_EntityList)
                    //e->MoveTo(offscreen)
        }
        virtual void Draw() override {
            //m_startButton.Draw(); //Draw calls should call the renderer in application (how to do this???)
            //
            //for(Entity& e: m_EntityList)
                //e->Draw();
        }
    private:
        //Ref<Button> m_StartButton;
        //Ref<EntityList> m_EntityList;
};



int main(int, char**) {
    std::unique_ptr<rose::Application> squaresApp = std::make_unique<rose::Application>();
    std::shared_ptr<rose::Layer> layer = std::make_shared<MainLayer>();

    squaresApp->SetLayer(layer);
    squaresApp->Run();
    squaresApp->Shutdown();

    return 0;
}


