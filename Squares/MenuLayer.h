#ifndef MENU_LAYER_H
#define MENU_LAYER_H

#include "Rose.h"
#include "Fractal.h"

namespace sqs {

    class Button;
    class PuzzleSet;
    class Puzzle;

    class MenuLayer: public rose::Layer {
        public:
            MenuLayer();
            virtual ~MenuLayer();
            virtual bool Update(double deltaTime, const std::array<bool, rose::g_MaxKeys>& keys, const std::array<bool, rose::g_MaxMouseButtons>& mouse , const glm::vec2& mousePos) override;
            Puzzle* GetOpenPuzzle() const;
            PuzzleSet* GetOpenPuzzleSet() const;
            virtual void Draw() override;
            float Sigmoid(float t) const;
        public:
            void OpenPuzzleSetMenu();
            void OpenMainMenu();
            void OpenPuzzleSet(PuzzleSet* puzzleSet);
            void ClosePuzzleSet(PuzzleSet* puzzleSet);
            void OpenPuzzle(Puzzle* puzzle);
            void SetAnimationStart();
            void SplitFractal(BaseFractal* fractal);
        private:
            Button* m_QuitButton;
            Button* m_StartButton;
            Button* m_CloseButton;
            rose::Sound* m_Sound;

            std::vector<rose::Entity*> m_Entities;

            float m_Parameter {1.0f};
            bool m_Start {false};

            float m_LeftEdge {-240.0f};
            float m_RightEdge {240.0f};
            float m_TopEdge {135.0f};
            float m_BottomEdge {-135.0f};
    };

}


#endif //MENU_LAYER_H
