#ifndef FRACTAL_H
#define FRACTAL_H

#include "Rose.h"
#include "Data.h"

namespace sqs {

    class Puzzle;

    enum class OverlapType {
        None = 0, //
        Within, //proper subset
        Partial,
        Enclose, //fractal is larger and encloses other fractal
        Equal
    };

    //this is essentially the raw way of doing std::bind and std::function to create function object containing member function pointer and object
    struct CallbackData {
      Puzzle* puzzle;
      void (Puzzle::*transformCallback)(FractalData, TransformationType);
    };


    class Fractal: public rose::Entity {
        public:
            Fractal(rose::Sprite sprite, FractalData data, const glm::vec2& coords, CallbackData callback);
            virtual ~Fractal() {}
            int GetSize() const { return m_Data.size; }
//            FractalElement GetSubElement(const glm::ivec2& index) const;
            glm::ivec2 GetIndex() const { return m_Data.index; }
            void SetIndex(const glm::ivec2& index) { m_Data.index = index; } //@todo: delete this - fractals should be destroyed/recreated in OnAnimationEnd()
            bool Contains(const glm::ivec2& index) const; 
            //void WriteData(std::vector<PuzzleSetData>& data, int setIndex, int puzzleIndex) const; //@todo: move to MenuLayer class
            void Transform(TransformationType type);
        public:
//            virtual void RotateBy(float angle) override; //@todo: need to move element transformations to g_Data instead of m_Elements
//            virtual void ScaleTo(const glm::vec2& scale) override; //@todo: need to move element transformations to g_Data instead of m_Elements
            virtual rose::Edge EdgeCollision(float pointX, float pointY) const override;
            virtual rose::Corner CornerCollision(float pointX, float pointY) const override;
            virtual void OnAnimationEnd() override;
        public:
    //        static rose::EntityData MakeEntityData(const std::vector<FractalElement>& elements, const glm::ivec2& fractalIndex, const glm::vec2& pos, int puzzleIndex);
   //         static void UpdateTextureData(const std::vector<FractalElement>& elements, const glm::ivec2& fractalIndex, int puzzleIndex); //@todo: move to puzzle class
            static glm::ivec2 GetTextureStart(const glm::ivec2& fractalIndex, int puzzleIndex); //@todo: move to puzzle class
            static glm::vec2 GetCoordsForTarget(const glm::ivec2& index, int size, const glm::ivec2& targetIndex, int targetSize, 
                                                const glm::ivec2& puzzleDim, const glm::vec2& puzzlePos);
            static glm::vec2 GetCoords(const glm::ivec2& index, int size, const glm::ivec2& puzzleDim, const glm::vec2& puzzlePos);
            static OverlapType FindOverlapType(FractalData f1, FractalData f2); 
        private:
//            std::vector<FractalElement> GetSubElements(const glm::ivec2& index, int subElementSize) const;
            void UpdateSprite(); //@todo: delete - fractals that are transformed should be destroyed when OnAnimationEnd() is called
        private:
            inline static const float s_UnitSize = 32.0f;
            inline static const float s_UnitMargin = 16.0f;
            FractalData m_Data {0, {-1, -1}};
            CallbackData m_Callback;
            bool m_ShouldRecreate {false}; //if true, Puzzle class will destroy this instance and recreate it (using data in g_Data) in OnAnimationEnd() call
    };


}

#endif // FRACTAL_H
