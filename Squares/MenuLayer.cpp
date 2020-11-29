#include <cmath>
#include <fstream>

#include "MenuLayer.h"
#include "Button.h"
#include "PuzzleSet.h"
#include "Puzzle.h"
#include "PuzzleIcon.h"
#include "Fractal.h"

using json = nlohmann::json;

namespace sqs {

    MenuLayer::MenuLayer(): Layer() {
        
        m_Sound = new rose::Sound("sound/pluck.wav");

        Fractal<int> f0(1, glm::ivec2(0, 0), glm::vec2(0.0f, 0.0f), 0);
        Fractal<glm::imat2> f1(glm::imat2(0), glm::ivec2(0, 0), glm::vec2(0.0f, 0.0f), 0);
        Fractal<glm::imat4> f2(glm::imat4(0), glm::ivec2(0, 0), glm::vec2(0.0f, 0.0f), 0);
        const rose::Sprite startSprite = {{0, 96}, {64, 32}, rose::TextureType::Default};
        const rose::Sprite quitSprite = {{64, 0}, {64, 32}, rose::TextureType::Default};
        const rose::Sprite closeSprite = {{96, 96}, {32, 32}, rose::TextureType::Default};

        const glm::vec2 size = glm::vec2(64.0f, 32.0f);
        const glm::vec2 tinysize = glm::vec2(16.0f, 16.0f);
        const glm::vec4 boundingBox = glm::vec4(0.0f, 0.0f, 64.0f, 32.0f);
        const glm::vec4 tinyboundingBox = glm::vec4(0.0f, 0.0f, 16.0f, 16.0f);

        m_StartButton = new Button(startSprite, size, boundingBox, glm::vec2(0.0f, 32.0f));
        m_StartButton->SetAnimationCoords(glm::vec2(0.0f, 32.0f), glm::vec2(m_RightEdge + 96.0f, 32.0f));
        m_QuitButton = new Button(quitSprite, size, boundingBox, glm::vec2(0.0f, -32.0f));
        m_QuitButton->SetAnimationCoords(glm::vec2(0.0f, -32.0f), glm::vec2(m_LeftEdge - 96.0f, -32.0f));
        m_CloseButton = new Button(closeSprite, tinysize, tinyboundingBox, glm::vec2(m_RightEdge + 16.0f, m_TopEdge - 16.0f));
        m_CloseButton->SetAnimationCoords(glm::vec2(m_RightEdge - 16.0f, m_TopEdge - 16.0f), glm::vec2(m_RightEdge + 16.0f, m_TopEdge - 16.0f));

        m_Entities.push_back((rose::Entity*)m_StartButton);
        m_Entities.push_back((rose::Entity*)m_QuitButton);
        m_Entities.push_back((rose::Entity*)m_CloseButton);

        ///////////////////////////////example writing to output file in user app directory////////////////////////
        char* prefPath = NULL;
        prefPath = SDL_GetPrefPath("Squares", "profile");
        if(prefPath) {
            std::cout << prefPath << std::endl;
        }else{
            std::cout << "Error getting pref path" << std::endl;
        }

        std::string outputPath = std::string(prefPath) + "/test.json";

        SDL_free(prefPath);

        std::ofstream output(outputPath.c_str());
        FractalElement r = FractalElement::Red;
        FractalElement g = FractalElement::Green;
        FractalElement b = FractalElement::Blue;
        FractalElement e = FractalElement::Empty;
        output << (int)r << (int)g << (int)b << (int)e;
        output.close();
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        /////////////////////////////////example reading in default data//////////////////////////////////////////////
        char* dataPath;
        char* basePath = SDL_GetBasePath();
        if(basePath) {
            dataPath = basePath;
        }else{
            dataPath = SDL_strdup("./");
        }
        SDL_free(basePath);
        std::string path = std::string(dataPath) + "../../resources/default.json";
        std::vector<PuzzleSetData> puzzleSetList;
        LoadPuzzleData(path, &puzzleSetList);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////
       

        float topEdge = 135.0f;
        m_PuzzleSets.emplace_back(new PuzzleSet(puzzleSetList.at(0), glm::vec2(-32.0f, topEdge + 32.0f)));
        m_PuzzleSets.emplace_back(new PuzzleSet(puzzleSetList.at(1), glm::vec2(0.0f, topEdge + 32.0f)));
        m_PuzzleSets.emplace_back(new PuzzleSet(puzzleSetList.at(2), glm::vec2(32.0f, topEdge + 32.0f)));
        for(PuzzleSet* ps: m_PuzzleSets) m_Entities.push_back(ps);

    }


    void MenuLayer::LoadPuzzleData(const std::string& path, std::vector<PuzzleSetData>* puzzleSetList) {
        std::ifstream inputFile(path);
        json j = json::parse(inputFile);

        int puzzleSetCount = j.begin().value().size();
        for(int i = 0; i < puzzleSetCount; ++i) { //iterator through puzzlesets
            PuzzleSetData puzzleSetData;
            json puzzleSet = j.begin().value().at(i);
            for(json::iterator it = puzzleSet.begin(); it != puzzleSet.end(); ++it) {
                if(it.value().is_array()) {
                    int puzzleCount = it.value().size();
                    for(int k = 0; k < puzzleCount; ++k) { //iterate through puzzles
                        PuzzleData puzzleData;
                        json puzzle = it.value().at(k);
                        for(json::iterator it2 = puzzle.begin(); it2 != puzzle.end(); ++it2) {
                            if(it2.value().is_array()) { //iterator through elements
                                int elementCount = it2.value().size();
                                for(int m = 0; m < elementCount; ++m) {
                                    puzzleData.Elements.push_back(it2.value().at(m));
                                }
                            }else{
                                if(it2.key() == "Width") puzzleData.Width = it2.value();
                                if(it2.key() == "Height") puzzleData.Height = it2.value();
                            }
                        }
                        puzzleSetData.Puzzles.push_back(puzzleData);
                    }
                }else{
                    //std::cout << it.key() << ": " << it.value() << std::endl;
                }
            }
            puzzleSetList->push_back(puzzleSetData);
            //std::cout << std::endl;
        }
    }

    MenuLayer::~MenuLayer() {
        if(m_QuitButton) delete m_QuitButton;
        if(m_StartButton) delete m_StartButton;
        if(m_CloseButton) delete m_CloseButton;

        for(rose::Entity* e: m_Entities) if(e) delete e;
    }

    void MenuLayer::SetAnimationStart() {
        m_Parameter = 0.0f;
        m_Start = true;
    }

    void MenuLayer::OpenPuzzleSetMenu() {
        m_StartButton->GoAway();
        m_StartButton->ScaleTo(glm::vec2(2.0f, 1.0f));
        m_QuitButton->GoAway();
        m_QuitButton->ScaleTo(glm::vec2(1.0f, 2.0f));
        m_CloseButton->ComeBack();
        for(PuzzleSet* ps : m_PuzzleSets) ps->MoveTo(glm::vec2(ps->x(), 0.0f));
        SetAnimationStart();
    }

    void MenuLayer::OpenMainMenu() {
        m_StartButton->ComeBack();
        m_StartButton->ScaleTo(glm::vec2(1.0f, 1.0f));
        m_QuitButton->ComeBack();
        m_QuitButton->ScaleTo(glm::vec2(1.0f, 1.0f));
        m_CloseButton->GoAway();
        for(PuzzleSet* ps : m_PuzzleSets) ps->MoveTo(glm::vec2(ps->x(), m_TopEdge + 32.0f));
        SetAnimationStart();
    }

    void MenuLayer::ClosePuzzleSet(PuzzleSet* openPuzzleSet) {
        for(PuzzleSet* ps : m_PuzzleSets) ps->MoveTo(glm::vec2(ps->x(), 0.0f));
        openPuzzleSet->Close();
        SetAnimationStart();
    }

    void MenuLayer::OpenPuzzleSet(PuzzleSet* puzzleSet) {
        puzzleSet->Open();
        for(PuzzleSet* ps : m_PuzzleSets) ps->MoveTo(glm::vec2(ps->x(), m_TopEdge + 32.0f));
        SetAnimationStart();
    }

    void MenuLayer::OpenPuzzle(Puzzle* puzzle) {
        PuzzleSet* puzzleSet = GetOpenPuzzleSet();
        puzzleSet->OpenPuzzle(puzzle);
        SetAnimationStart();
    }

    void MenuLayer::SplitFractal(BaseFractal* fractal) {
        Puzzle* puzzle = GetOpenPuzzle();
        puzzle->SplitFractal(fractal);
        SetAnimationStart();
    }

    bool MenuLayer::Update(double deltaTime, const std::array<bool, rose::g_MaxKeys>& keys, const std::array<bool, rose::g_MaxMouseButtons>& mouseKeys , 
                           const glm::vec2& mouse) {

        if(m_Parameter < 1.0f && m_Start) {
            m_Parameter += static_cast<float>(deltaTime) * .0010f;
            if(m_Parameter >= 1.0f) {
                for(rose::Entity* e: m_Entities) e->OnAnimationEnd();
            }else{
                for(rose::Entity* e: m_Entities) e->OnAnimationUpdate(Sigmoid(m_Parameter));
                return false; //skip remainder of Update function if animation is still playing
            }
        }

        if(m_QuitButton->PointCollision(mouse.x, mouse.y) && mouseKeys.at(rose::MouseEvents::LeftButton)) return true;

        if(m_StartButton->PointCollision(mouse.x, mouse.y) && mouseKeys.at(rose::MouseEvents::LeftButton)) {
            OpenPuzzleSetMenu();
        }

        if(m_CloseButton->PointCollision(mouse.x, mouse.y) && mouseKeys.at(rose::MouseEvents::LeftButton)) {
            PuzzleSet* openPuzzleSet = GetOpenPuzzleSet();

            if(openPuzzleSet) {
                ClosePuzzleSet(openPuzzleSet);
            }else{
                OpenMainMenu();
            }
        }

        for(PuzzleSet* ps : m_PuzzleSets) {
            if(ps->PointCollision(mouse.x, mouse.y) && mouseKeys.at(rose::MouseEvents::LeftButton)) {
                OpenPuzzleSet(ps);
                break;
            }
            for(PuzzleIcon* icon: ps->GetPuzzleIcons()) {
                if(icon->PointCollision(mouse.x, mouse.y) && mouseKeys.at(rose::MouseEvents::LeftButton)) {
                    OpenPuzzle(icon->GetPuzzle());
                    break;
                }
            }
        }

        
        if(Puzzle* puzzle = GetOpenPuzzle()) {

            if(mouseKeys.at(rose::MouseEvents::RightButton)) {
                FractalCorners fc = puzzle->FindFractalCorners(mouse.x, mouse.y); //fourFractals should be a pointer to a struct of Fractal pointers
                if(fc.TopLeft && fc.TopRight && fc.BottomLeft && fc.BottomRight) {
                    puzzle->FormFractal(fc); 
                    SetAnimationStart();
                }
            }

            for(BaseFractal* fractal: puzzle->GetFractals()) {
                if(fractal->PointCollision(mouse.x, mouse.y)) { 
                    glm::ivec2 index = fractal->GetIndex();
                    if(keys.at(SDLK_a)) { //translate left
                        BaseFractal* otherFractal = puzzle->GetFractal(glm::ivec2(index.x - GetFractalSize(fractal), index.y)); //to get fractal to the left of current
                        if(otherFractal && GetFractalSize(fractal) == GetFractalSize(otherFractal)) { //move all this into MenuLayer::TransformFractal() later
                            puzzle->SwapFractals(fractal, otherFractal);
                            SetAnimationStart();
                            break;
                        }
                    }else if(keys.at(SDLK_d)) { //translate right
                        BaseFractal* otherFractal = puzzle->GetFractal(glm::ivec2(index.x + GetFractalSize(fractal), index.y)); 
                        if(otherFractal && GetFractalSize(fractal) == GetFractalSize(otherFractal)) { 
                            puzzle->SwapFractals(fractal, otherFractal);
                            SetAnimationStart();
                            break;
                        }
                    }else if(keys.at(SDLK_w)) { //translate up
                        BaseFractal* otherFractal = puzzle->GetFractal(glm::ivec2(index.x, index.y - GetFractalSize(fractal)));
                        if(otherFractal && GetFractalSize(fractal) == GetFractalSize(otherFractal)) { 
//                            m_Sound->Play();
                            puzzle->SwapFractals(fractal, otherFractal);
                            SetAnimationStart();
                            break;
                        }
                    }else if(keys.at(SDLK_s)) { //translate down
                        BaseFractal* otherFractal = puzzle->GetFractal(glm::ivec2(index.x, index.y + GetFractalSize(fractal)));
                        if(otherFractal && GetFractalSize(fractal) == GetFractalSize(otherFractal)) { 
                            puzzle->SwapFractals(fractal, otherFractal);
                            SetAnimationStart();
                            break;
                        }
                    }else if(keys.at(SDLK_q)) { //rotate ccw
                        if(GetFractalSize(fractal) > 1) { 
                            puzzle->RotateFractalCCW(fractal);
                            SetAnimationStart();
                            break;
                        }
                    }else if(keys.at(SDLK_e)) { //rotate cw
                        if(GetFractalSize(fractal) > 1) { 
                            puzzle->RotateFractalCW(fractal);
                            SetAnimationStart();
                            break;
                        }
                    }else if(keys.at(SDLK_x)) { //reflect across x-axis
                        if(GetFractalSize(fractal) > 1) { 
                            puzzle->ReflectFractalX(fractal);
                            SetAnimationStart();
                            break;
                        }
                    }else if(keys.at(SDLK_z)) { //reflect across y-axis
                        if(GetFractalSize(fractal) > 1) { 
                            puzzle->ReflectFractalY(fractal);
                            SetAnimationStart();
                            break;
                        }
                    }else if(mouseKeys.at(rose::MouseEvents::LeftButton) && GetFractalSize(fractal) != 1) {
                        SplitFractal(fractal);
                        SetAnimationStart();
                        break;
                    }
                }
            } 


        }

        return false;

    }


    void MenuLayer::Draw() {
        for(rose::Entity* e: m_Entities) {
            e->Draw();
        }
    }


    Puzzle* MenuLayer::GetOpenPuzzle() const {
        PuzzleSet* puzzleSet = GetOpenPuzzleSet();
        if(puzzleSet) {
            for(Puzzle* puzzle: puzzleSet->GetPuzzles()) {
                if(puzzle && puzzle->IsOpen()) return puzzle;
            }
        }

        return nullptr;
    }


    PuzzleSet* MenuLayer::GetOpenPuzzleSet() const {
        for(PuzzleSet* ps: m_PuzzleSets) {
            if(ps && ps->IsOpen()) return ps;
        }
        return nullptr;
    }



    float MenuLayer::Sigmoid(float t) const {
        return 1.0f / (1.0f + exp(-15.0f * (t - 0.5f)));
    }


}
