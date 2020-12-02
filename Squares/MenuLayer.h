#ifndef MENU_LAYER_H
#define MENU_LAYER_H

#include "Rose.h"
#include "Fractal.h"
#include "PuzzleSet.h"

namespace sqs {

    class Button;
    class PuzzleSet;
    class Puzzle;

    struct CommandData {
        PuzzleSet* puzzleSet {nullptr};
        Puzzle* puzzle {nullptr};
        BaseFractal* baseFractal {nullptr};
    };

    class MenuLayer: public rose::Layer {
        public:
            MenuLayer();
            virtual ~MenuLayer();
            virtual bool Update(double deltaTime, const std::array<bool, rose::g_MaxKeys>& keys, const std::array<bool, 
                                rose::g_MaxMouseButtons>& mouse , const glm::vec2& mousePos) override;
            Puzzle* GetOpenPuzzle() const;
            PuzzleSet* GetOpenPuzzleSet() const;
            virtual void Draw() override;
            float Sigmoid(float t) const;
            void AddCommand(void (MenuLayer::*func)());
            void CallNextCommand();
        public: //commands that can be added to command queue
            void OpenPuzzleSetMenu();
            void OpenMainMenu();
            void OpenPuzzleSet(PuzzleSet* puzzleSet);
            void ClosePuzzleSet(PuzzleSet* puzzleSet);
            void OpenPuzzle(Puzzle* puzzle);
            void SetAnimationStart();
            void SplitFractal(BaseFractal* fractal);
            void FormFractal(Puzzle* puzzle, FractalCorners fc);
            void ResizeFractalsToUndo(Puzzle* puzzle); 
            void UndoTransformation(Puzzle* puzzle);
        private:
            void LoadPuzzleData(const std::string& path, std::vector<PuzzleSetData>* puzzleSetList);
        private:
            Button* m_QuitButton;
            Button* m_StartButton;
            Button* m_CloseButton;
            rose::Sound* m_Sound;

            std::vector<rose::Entity*> m_Entities;
            std::vector<PuzzleSet*> m_PuzzleSets;

            float m_Parameter {1.0f};
            bool m_Start {false};

            float m_LeftEdge {-240.0f};
            float m_RightEdge {240.0f};
            float m_TopEdge {135.0f};
            float m_BottomEdge {-135.0f};
            std::vector<void (MenuLayer::*)()> m_CommandQueue; //vector of MenuLayer function pointers with void return and void (none) parameters

            //processing raw inputs 
            bool m_PreviousLeftButton {false};
            glm::vec2 m_DownMouseCoords {0.0f, 0.0f};
            glm::vec2 m_UpMouseCoords {0.0f, 0.0f};
    };

}


#endif //MENU_LAYER_H
