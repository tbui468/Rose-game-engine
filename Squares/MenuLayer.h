#ifndef MENU_LAYER_H
#define MENU_LAYER_H

#include "Rose.h"
#include "Fractal.h"
#include "PuzzleSet.h"
#include "Puzzle.h"

namespace sqs {

    class Button;



    enum class CommandType {
        None,
        OpenPuzzleSetMenu,
        OpenMainMenu,
        OpenPuzzleSet,
        ClosePuzzleSet,
        OpenPuzzle,
        SplitFractal,
        FormFractal,
        UndoResizeFractals,
        UndoTransformation 
    };


    struct CommandData {
        CommandType type {CommandType::None};
        PuzzleSet* puzzleSet {nullptr};
        Puzzle* puzzle {nullptr};
        Fractal* fractal {nullptr};
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
            void AddCommand(CommandData data);
            void CallNextCommand();
            void SetAnimationStart();
        public: //commands that can be added to command queue
            void OpenPuzzleSetMenu();
            void OpenMainMenu();
            void OpenPuzzleSet(PuzzleSet* puzzleSet);
            void ClosePuzzleSet(PuzzleSet* puzzleSet);
            void OpenPuzzle(Puzzle* puzzle);
            void SplitFractal(Puzzle* puzzle, Fractal* fractal);
            void FormFractal(Puzzle* puzzle, FractalCorners fc);
            void UndoResizeFractals(Puzzle* puzzle); 
            void UndoTransformation(Puzzle* puzzle);
        private:
            //void ReadPuzzleData(const std::string& path, std::vector<PuzzleSetData>* puzzleSetList);
            void ReadPuzzleData(const std::string& path, std::vector<PuzzleSetData>& data);
            void WritePuzzleData(const std::string& path, const std::vector<PuzzleSetData>& data); 
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
            std::vector<CommandData> m_CommandQueue; //vector of MenuLayer function pointers with void return and void (none) parameters

            //processing raw inputs 
            glm::vec2 m_DownMouseCoords {0.0f, 0.0f};
            glm::vec2 m_UpMouseCoords {0.0f, 0.0f};
    };

}


#endif //MENU_LAYER_H
