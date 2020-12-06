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




    enum class InputType {
        None,
        Tap,
        FlickLeft,
        FlickRight,
        FlickDown,
        FlickUp,
        PinchIn,
        PinchOut
    };



    MenuLayer::MenuLayer(): Layer() {
        m_Sound = new rose::Sound("sound/pluck.wav");

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
        //this is how I want to write out profile.json
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
        FractalElement r = 'r';
        FractalElement g = 'g';
        FractalElement b = 'b';
        FractalElement e = 'e';
        output << (int)r << (int)g << (int)b << (int)e;
        output.close();
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /////////////////////////////////example reading in default data//////////////////////////////////////////////
        //temp: this data should be saved inside the executable, not in an external file where it can be modified
        //reading in data should overwrite g_Data, and only occur when profile.json exists and is not corrupt
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

        for(int i = 0; i < g_Data.size(); ++i) {
            m_PuzzleSets.emplace_back(new PuzzleSet(i, glm::vec2(-32.0f + 32.0f * i, topEdge + 32.0f)));
        }

        for(PuzzleSet* ps: m_PuzzleSets) m_Entities.push_back(ps);

    }


    void MenuLayer::LoadPuzzleData(const std::string& path, std::vector<PuzzleSetData>* puzzleSetList) {
        /*
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
                                    puzzleData.elements.push_back(it2.value().at(m));
                                }
                            }else{
                                if(it2.key() == "Width") puzzleData.dimensions.x = it2.value();
                                if(it2.key() == "Height") puzzleData.dimensions.y = it2.value();
                            }
                        }
                        puzzleSetData.puzzlesData.push_back(puzzleData);
                    }
                }else{
                    //std::cout << it.key() << ": " << it.value() << std::endl;
                }
            }
            puzzleSetList->push_back(puzzleSetData);
            //std::cout << std::endl;
        }

        inputFile.close();*/
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

    void MenuLayer::AddCommand(CommandData data) {
        m_CommandQueue.push_back(data);
    }

    void MenuLayer::CallNextCommand() {
        CommandData command = m_CommandQueue.front();
    
        switch(command.type) {
            case CommandType::OpenPuzzleSetMenu:
                OpenPuzzleSetMenu();
                break;
            case CommandType::OpenMainMenu:
                OpenMainMenu();
                break;
            case CommandType::OpenPuzzleSet:
                OpenPuzzleSet(command.puzzleSet);
                break;
            case CommandType::ClosePuzzleSet:
                ClosePuzzleSet(command.puzzleSet);
                break;
            case CommandType::OpenPuzzle:
                OpenPuzzle(command.puzzle);
                break;
            case CommandType::SplitFractal:
                SplitFractal(command.puzzle, command.fractal);
                break;
                /*
            case CommandType::FormFractal: 
                FormFractal(command.puzzle, FractalCorners fc);
                break;*/
            case CommandType::UndoResizeFractals:
                UndoResizeFractals(command.puzzle);
                break;
            case CommandType::UndoTransformation:
                UndoTransformation(command.puzzle);
                break;
        }

        m_CommandQueue.erase(m_CommandQueue.begin());
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

    void MenuLayer::OpenPuzzleSet(PuzzleSet* puzzleSet) {
        puzzleSet->Open();
        for(PuzzleSet* ps : m_PuzzleSets) ps->MoveTo(glm::vec2(ps->x(), m_TopEdge + 32.0f));
        SetAnimationStart();
    }

    void MenuLayer::ClosePuzzleSet(PuzzleSet* puzzleSet) {
        for(PuzzleSet* ps : m_PuzzleSets) ps->MoveTo(glm::vec2(ps->x(), 0.0f));
        puzzleSet->Close();
        SetAnimationStart();
    }

    void MenuLayer::OpenPuzzle(Puzzle* puzzle) {
        PuzzleSet* puzzleSet = GetOpenPuzzleSet();
        puzzleSet->OpenPuzzle(puzzle);
        SetAnimationStart();
    }

    void MenuLayer::SplitFractal(Puzzle* puzzle, Fractal* fractal) {


        int subSize = fractal->GetSize() / 2;
        glm::ivec2 index = fractal->GetIndex();

        std::vector<FractalData> splitData;
        splitData.push_back({subSize, {index.x, index.y}});
        splitData.push_back({subSize, {index.x + subSize, index.y}});
        splitData.push_back({subSize, {index.x, index.y + subSize}});
        splitData.push_back({subSize, {index.x + subSize, index.y + subSize}});

        //can Puzzle::SplitFractal(splitData) just take in the split data and then find the fractal data from it?
        //could put an assert inside split to make sure that all the subfractals form a large fractal of correct dimenions (1, 2, or 4)
        std::vector<Fractal*> subFractalList = puzzle->SplitFractal(fractal, splitData); 

        for(Fractal* f: subFractalList) {
            glm::vec2 endCoords = Fractal::GetCoords(f->GetIndex(), subSize, puzzle->GetDimensions(), glm::vec2(puzzle->x(), puzzle->y()));
            f->MoveTo(endCoords);
        }

        SetAnimationStart();
    }

    void MenuLayer::FormFractal(Puzzle* puzzle, FractalCorners fc) {

        glm::ivec2 targetIndex = fc.TopLeft->GetIndex();
        int subFSize = fc.TopLeft->GetSize();
        int targetSize = subFSize * 2;
        glm::vec2 puzzlePos = {puzzle->x(), puzzle->y()};

        glm::vec2 TLCoords = Fractal::GetCoordsForTarget(fc.TopLeft->GetIndex(), subFSize, targetIndex, targetSize, puzzle->GetDimensions(), puzzlePos);
        fc.TopLeft->MoveTo(TLCoords);
        glm::vec2 TRCoords = Fractal::GetCoordsForTarget(fc.TopRight->GetIndex(), subFSize, targetIndex, targetSize, puzzle->GetDimensions(), puzzlePos);
        fc.TopRight->MoveTo(TRCoords);
        glm::vec2 BLCoords = Fractal::GetCoordsForTarget(fc.BottomLeft->GetIndex(), subFSize, targetIndex, targetSize, puzzle->GetDimensions(), puzzlePos);
        fc.BottomLeft->MoveTo(BLCoords);
        glm::vec2 BRCoords = Fractal::GetCoordsForTarget(fc.BottomRight->GetIndex(), subFSize, targetIndex, targetSize, puzzle->GetDimensions(), puzzlePos);
        fc.BottomRight->MoveTo(BRCoords);

        //could put assert into merge fractals to make sure they all form a large fractal of correct dimensions (1,2 or 4)
        puzzle->MergeFractals({fc.TopLeft, 
                fc.TopRight,
                fc.BottomLeft,
                fc.BottomRight});
        SetAnimationStart();
    }

    void MenuLayer::UndoResizeFractals(Puzzle* puzzle) {
        bool resized = false; //set this to true if any splits/merges are necessary
        //
        //1. Check if the undo transformation is translation or not
        //      if it's a translation, then may need to resize the fractal in undo data AND the swapped fractal
        //2. Will need a function to determine if a subfractal is a proper subfractal of a larger given fractal,
        //      if it overlaps partially, or if doesn't overlap at all (just need two of the three, since only one of the three conditions can every be true)
        //
        //check if resize conditions are met (note: need to check two fractals if the undotransformation type is a translation)
        //Call Puzzle::SplitFractal() if necessary (need to split first since MergeFractals() requires pointers to fractals to merge)
        //Call Puzzle::MergeFractals() if necessary (recall: MergeFractals() creates a list of fractals to merge when Puzzle::OnAnimationEnd() called)
        //Call Fractal::MoveTo() if necessary (will need a handle to all fractals, including those returned by SplitFractal() function call)
        if(resized) SetAnimationStart();
        AddCommand({CommandType::UndoTransformation, nullptr, puzzle, nullptr});
    }

    void MenuLayer::UndoTransformation(Puzzle* puzzle) {
        puzzle->UndoTransformation();
        SetAnimationStart();
    }

    bool MenuLayer::Update(double deltaTime, const std::array<bool, rose::g_MaxKeys>& keys, const std::array<bool, rose::g_MaxMouseButtons>& mouseKeys, 
            const glm::vec2& mouse) {


        InputType input = InputType::None;

        //checking scroll first so that any drags/taps will have higher precedence and override below
        if(mouseKeys.at(rose::MouseEvents::WheelUp)) {
            input = InputType::PinchIn;
        }else if(mouseKeys.at(rose::MouseEvents::WheelDown)) {
            input = InputType::PinchOut;
        }

        //detect start of tap/drag
        if(mouseKeys.at(rose::MouseEvents::LeftButtonDown)) {
            m_DownMouseCoords = mouse;
        }

        //detect end of tap/drag and process
        if(mouseKeys.at(rose::MouseEvents::LeftButtonUp)) {
            m_UpMouseCoords = mouse;

            float dragThreshold = 10.0f;
            if(rose::PointDistance(m_DownMouseCoords, m_UpMouseCoords) < dragThreshold) {
                input = InputType::Tap;
            }else{
                float deltaX = m_UpMouseCoords.x - m_DownMouseCoords.x;
                float deltaY = m_UpMouseCoords.y - m_DownMouseCoords.y;
                if(abs(deltaX) > abs(deltaY)) {
                    if(deltaX < 0) input = InputType::FlickLeft;
                    else input = InputType::FlickRight;
                }else{
                    if(deltaY < 0) input = InputType::FlickDown;
                    else input = InputType::FlickUp;
                }
            }
        }

         
        if(m_Parameter < 1.0f && m_Start) {
            m_Parameter += static_cast<float>(deltaTime) * .0010f;
            if(m_Parameter >= 1.0f) {
                for(rose::Entity* e: m_Entities) e->OnAnimationEnd();
            }else{
                for(rose::Entity* e: m_Entities) e->OnAnimationUpdate(rose::Sigmoid(m_Parameter));
                return false; //skip remainder of Update function if animation is still playing
            }
        }else {
            if(m_CommandQueue.size() > 0) CallNextCommand();
        }


        if(m_QuitButton->PointCollision(mouse.x, mouse.y) && input == InputType::Tap) return true;

        if(m_StartButton->PointCollision(mouse.x, mouse.y) && input == InputType::Tap) {
            AddCommand({CommandType::OpenPuzzleSetMenu, nullptr, nullptr, nullptr});
        }

        if(m_CloseButton->PointCollision(mouse.x, mouse.y) && input == InputType::Tap) {
            PuzzleSet* openPuzzleSet = GetOpenPuzzleSet();

            if(openPuzzleSet) {
                AddCommand({CommandType::ClosePuzzleSet, openPuzzleSet, nullptr, nullptr});
            }else{
                AddCommand({CommandType::OpenMainMenu, nullptr, nullptr, nullptr});
            }
        }

        for(PuzzleSet* ps : m_PuzzleSets) {
            if(ps->PointCollision(mouse.x, mouse.y) && input == InputType::Tap) {
                AddCommand({CommandType::OpenPuzzleSet, ps, nullptr, nullptr});
                break;
            }
            for(PuzzleIcon* icon: ps->GetPuzzleIcons()) {
                if(icon->PointCollision(mouse.x, mouse.y) && input == InputType::Tap) {
                    AddCommand({CommandType::OpenPuzzle, nullptr, icon->GetPuzzle(), nullptr});
                    break;
                }
            }
        }


        if(Puzzle* puzzle = GetOpenPuzzle()) {
            //no normal collision checks
            if(input == InputType::PinchIn) {
                FractalCorners fc = puzzle->FindFractalCorners(mouse.x, mouse.y); //fourFractals should be a pointer to a struct of Fractal pointers
                if(fc.TopLeft && fc.TopRight && fc.BottomLeft && fc.BottomRight) {
                    FormFractal(puzzle, fc);
                }
            }

            if(keys.at(SDLK_SPACE) && puzzle->GetTransformationCount() > 0) {
                AddCommand({CommandType::UndoResizeFractals, nullptr, puzzle, nullptr}); //UndoResizeFractals calls UndoTransformation immediately afterwards
            }


            //check for collisions with fractals
            for(Fractal* fractal: puzzle->GetFractals()) {

                if(input == InputType::PinchOut && fractal->GetSize() != 1 && fractal->PointCollision(mouse.x, mouse.y)) {

                    SplitFractal(puzzle, fractal);
                    break;
                }

                //check for transformations here
                if(puzzle->GetTransformationCount() < puzzle->GetMaxTransformations()) {
                    glm::ivec2 index = fractal->GetIndex();
                    Fractal* otherFractal;
                    switch(input) {
                        case InputType::Tap: 
                            if(fractal->EdgeCollision(mouse.x, mouse.y) == rose::Edge::Left || fractal->EdgeCollision(mouse.x, mouse.y) == rose::Edge::Right) {
                                if(fractal->GetSize() > 1) { 
                                    puzzle->ReflectFractalY(fractal);
                                    SetAnimationStart();
                                    break;
                                }
                            }else if(fractal->EdgeCollision(mouse.x, mouse.y) == rose::Edge::Top || fractal->EdgeCollision(mouse.x, mouse.y) == rose::Edge::Bottom) {
                                if(fractal->GetSize() > 1) { 
                                    puzzle->ReflectFractalX(fractal);
                                    SetAnimationStart();
                                    break;
                                }
                            }
                            break;
                        case InputType::FlickLeft:
                            if(fractal->PointCollision(m_DownMouseCoords.x, m_DownMouseCoords.y)) {
                                otherFractal = puzzle->GetFractal(glm::ivec2(index.x - fractal->GetSize(), index.y)); 
                                if(otherFractal && fractal->GetSize() == otherFractal->GetSize()) { 
                                    puzzle->SwapFractals(fractal, otherFractal);
                                    SetAnimationStart();
                                    break;
                                }
                            }
                            if(fractal->CornerCollision(m_DownMouseCoords.x, m_DownMouseCoords.y) == rose::Corner::TopRight && fractal->GetSize() > 1) {
                                puzzle->RotateFractalCCW(fractal);
                                SetAnimationStart();
                                break;
                            }
                            if(fractal->CornerCollision(m_DownMouseCoords.x, m_DownMouseCoords.y) == rose::Corner::BottomRight && fractal->GetSize() > 1) {
                                puzzle->RotateFractalCW(fractal);
                                SetAnimationStart();
                                break;
                            }
                            break;
                        case InputType::FlickRight:
                            if(fractal->PointCollision(m_DownMouseCoords.x, m_DownMouseCoords.y)) {
                                otherFractal = puzzle->GetFractal(glm::ivec2(index.x + fractal->GetSize(), index.y)); 
                                if(otherFractal && fractal->GetSize() == otherFractal->GetSize()) { 
                                    puzzle->SwapFractals(fractal, otherFractal);
                                    SetAnimationStart();
                                    break;
                                }
                            }
                            if(fractal->CornerCollision(m_DownMouseCoords.x, m_DownMouseCoords.y) == rose::Corner::TopLeft && fractal->GetSize() > 1) {
                                puzzle->RotateFractalCW(fractal);
                                SetAnimationStart();
                                break;
                            }
                            if(fractal->CornerCollision(m_DownMouseCoords.x, m_DownMouseCoords.y) == rose::Corner::BottomLeft && fractal->GetSize() > 1) {
                                puzzle->RotateFractalCCW(fractal);
                                SetAnimationStart();
                                break;
                            }
                            break;
                        case InputType::FlickDown:
                            if(fractal->PointCollision(m_DownMouseCoords.x, m_DownMouseCoords.y)) {
                                otherFractal = puzzle->GetFractal(glm::ivec2(index.x, index.y + fractal->GetSize())); 
                                if(otherFractal && fractal->GetSize() == otherFractal->GetSize()) { 
                                    puzzle->SwapFractals(fractal, otherFractal);
                                    SetAnimationStart();
                                    break;
                                }
                            }
                            if(fractal->CornerCollision(m_DownMouseCoords.x, m_DownMouseCoords.y) == rose::Corner::TopLeft && fractal->GetSize() > 1) {
                                puzzle->RotateFractalCCW(fractal);
                                SetAnimationStart();
                                break;
                            }
                            if(fractal->CornerCollision(m_DownMouseCoords.x, m_DownMouseCoords.y) == rose::Corner::TopRight && fractal->GetSize() > 1) {
                                puzzle->RotateFractalCW(fractal);
                                SetAnimationStart();
                                break;
                            }
                            break;
                        case InputType::FlickUp:
                            if(fractal->PointCollision(m_DownMouseCoords.x, m_DownMouseCoords.y)) {
                                otherFractal = puzzle->GetFractal(glm::ivec2(index.x, index.y - fractal->GetSize())); 
                                if(otherFractal && fractal->GetSize() == otherFractal->GetSize()) { 
                                    puzzle->SwapFractals(fractal, otherFractal);
                                    SetAnimationStart();
                                    break;
                                }
                            }
                            if(fractal->CornerCollision(m_DownMouseCoords.x, m_DownMouseCoords.y) == rose::Corner::BottomLeft && fractal->GetSize() > 1) {
                                puzzle->RotateFractalCW(fractal);
                                SetAnimationStart();
                                break;
                            }
                            if(fractal->CornerCollision(m_DownMouseCoords.x, m_DownMouseCoords.y) == rose::Corner::BottomRight && fractal->GetSize() > 1) {
                                puzzle->RotateFractalCCW(fractal);
                                SetAnimationStart();
                                break;
                            }
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





}
