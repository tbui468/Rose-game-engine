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

    enum class AnimationEndEvent {
        None = 0,
        Destroy,
        Recreate
    };


    class Fractal: public rose::Entity {
        public:
            Fractal(rose::Sprite sprite, FractalData data, const glm::vec2& coords, CallbackData callback);
            virtual ~Fractal() {}
            int GetSize() const { return m_Data.size; } //@todo: make public and const
            glm::ivec2 GetIndex() const { return m_Data.index; } //@todo: make public and const
            void SetIndex(const glm::ivec2& index) { m_Data.index = index; } //@todo: delete this - fractals should be destroyed/recreated in OnAnimationEnd()
            bool Contains(const glm::ivec2& index) const; 
            void Transform(TransformationType type);
        public:
            virtual rose::Edge EdgeCollision(float pointX, float pointY) const override;
            virtual rose::Corner CornerCollision(float pointX, float pointY) const override;
            virtual void OnAnimationEnd() override;
            AnimationEndEvent GetAnimationEndEvent() const { return m_AnimationEndEvent; }
            void SetAnimationEndEvent(AnimationEndEvent event) { m_AnimationEndEvent = event; }
        public:
            static glm::ivec2 GetTextureStart(const glm::ivec2& fractalIndex, int puzzleIndex); //@todo: move to puzzle class, rename to Calculate... or Find....
            static glm::vec2 GetCoordsForTarget(const glm::ivec2& index, int size, const glm::ivec2& targetIndex, int targetSize, 
                                                const glm::ivec2& puzzleDim, const glm::vec2& puzzlePos); //@todo: move to puzzle class, rename to Calculate...
            static glm::vec2 GetCoords(const glm::ivec2& index, int size, const glm::ivec2& puzzleDim, const glm::vec2& puzzlePos); //@todo: move to puzzle, rename Calculate.
            static OverlapType FindOverlapType(FractalData f1, FractalData f2); 
        private:
            void UpdateSprite(); //@todo: move functionality to puzzle - fractals that are transformed should be recreated when OnAnimationEnd() is called
        public:
            inline static const float s_UnitSize = 32.0f; //puzzle needs access
        private:
            inline static const float s_UnitMargin = 16.0f;
            FractalData m_Data {0, {-1, -1}}; //@todo: const this and make it public once SetIndex() is removed
            const CallbackData m_Callback;
            AnimationEndEvent m_AnimationEndEvent {AnimationEndEvent::None};

    };


}

#endif // FRACTAL_H
