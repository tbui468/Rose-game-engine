#ifndef FRACTAL_H
#define FRACTAL_H

#include "Rose.h"
#include "BaseFractal.h"
#include <glm/gtc/matrix_integer.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace sqs {

    class Puzzle;


    int GetFractalSize(BaseFractal* f);

    enum class FractalElement {
        Empty = 0,
        Block,
        Red,
        Blue,
        Green,
        RedEntry,
        RedExit,
        BlueEntry,
        BlueExit,
        GreenEntry,
        GreenExit
    };

    enum class Corner {
        None,
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight
    };

    enum class Edge {
        None,
        Left,
        Right,
        Top,
        Bottom
    };

    template <class T>
    class Fractal: public BaseFractal {
        public:
            Fractal(const T& elements, const glm::ivec2& index, const glm::vec2& coords, int puzzleNumber) 
                : Fractal(MakeEntityData(elements, index, coords, puzzleNumber), index) {
                    m_Elements = elements;
                    m_PuzzleNumber = puzzleNumber;
            }
            virtual ~Fractal() {}
            static int GetSize() {
                if(std::is_same<T, int>::value) return 1;
                else if(std::is_same<T, glm::imat2>::value) return 2;
                else if(std::is_same<T, glm::imat4>::value) return 4;
                else assert(false);
            }
            int GetPuzzleNumber() const { return m_PuzzleNumber; };
            
            const T& GetElements() const { return m_Elements; }

            Corner CornerCollision(const glm::vec2& mouse) {
                //check each of the corners for a collision with mouse
                return Corner::None;
            }

            Edge EdgeCollision(const glm::vec2& mouse) {
                //check each of the edges for a collision with mouse
                retunr Edge::None;
            }
            
            virtual void OnAnimationEnd() override {
                Entity::OnAnimationEnd();
                //setting fractal angles  and scale back to 0 to avoid saving states
                m_Angle = 0.0f;
                m_ToAngle = 0.0f;
                m_FromAngle = 0.0f;
                m_FromScale = {1.0f, 1.0f};
                m_Scale = {1.0f, 1.0f};
                m_ToScale = {1.0f, 1.0f};
                UpdateTextureData(m_Elements, GetIndex(), m_PuzzleNumber);
                UpdateSprite();
            }

            virtual void ScaleTo(const glm::vec2& scale) override {
                Entity::ScaleTo(scale);

                if(GetFractalSize(this) == 1) return;

                if(GetFractalSize(this) == 2) {
                    if(scale.y < 0) { //x-axis reflection
                        int topLeft = GetSubElementsI({0, 1});
                        int topRight = GetSubElementsI({1, 1});
                        int bottomLeft = GetSubElementsI({0, 0});
                        int bottomRight = GetSubElementsI({1, 0});
                        SetElements({topLeft, topRight, bottomLeft, bottomRight});
                    }else{
                        int topLeft = GetSubElementsI({1, 0});
                        int topRight = GetSubElementsI({0, 0});
                        int bottomLeft = GetSubElementsI({1, 1});
                        int bottomRight = GetSubElementsI({0, 1});
                        SetElements({topLeft, topRight, bottomLeft, bottomRight});
                    }
                    return;
                }

                if(GetFractalSize(this) == 4) {
                    if(scale.y < 0) { //x-axis reflection
                        int e00 = GetSubElementsI({0, 3});
                        int e10 = GetSubElementsI({1, 3});
                        int e20 = GetSubElementsI({2, 3});
                        int e30 = GetSubElementsI({3, 3});

                        int e01 = GetSubElementsI({0, 2});
                        int e11 = GetSubElementsI({1, 2});
                        int e21 = GetSubElementsI({2, 2});
                        int e31 = GetSubElementsI({3, 2});

                        int e02 = GetSubElementsI({0, 1});
                        int e12 = GetSubElementsI({1, 1});
                        int e22 = GetSubElementsI({2, 1});
                        int e32 = GetSubElementsI({3, 1});

                        int e03 = GetSubElementsI({0, 0});
                        int e13 = GetSubElementsI({1, 0});
                        int e23 = GetSubElementsI({2, 0});
                        int e33 = GetSubElementsI({3, 0});

                        SetElements({e00, e10, e20, e30,
                                     e01, e11, e21, e31,
                                     e02, e12, e22, e32,
                                     e03, e13, e23, e33});
                    }else{ //y-axis reflection
                        int e00 = GetSubElementsI({3, 0});
                        int e10 = GetSubElementsI({2, 0});
                        int e20 = GetSubElementsI({1, 0});
                        int e30 = GetSubElementsI({0, 0});

                        int e01 = GetSubElementsI({3, 1});
                        int e11 = GetSubElementsI({2, 1});
                        int e21 = GetSubElementsI({1, 1});
                        int e31 = GetSubElementsI({0, 1});

                        int e02 = GetSubElementsI({3, 2});
                        int e12 = GetSubElementsI({2, 2});
                        int e22 = GetSubElementsI({1, 2});
                        int e32 = GetSubElementsI({0, 2});

                        int e03 = GetSubElementsI({3, 3});
                        int e13 = GetSubElementsI({2, 3});
                        int e23 = GetSubElementsI({1, 3});
                        int e33 = GetSubElementsI({0, 3});

                        SetElements({e00, e10, e20, e30,
                                     e01, e11, e21, e31,
                                     e02, e12, e22, e32,
                                     e03, e13, e23, e33});
                    }
                    return;
                }
            }


            virtual void RotateBy(float angle) override {
                Entity::RotateBy(angle);

                if(GetFractalSize(this) == 1) return;

                if(GetFractalSize(this) == 2) {
                    if(angle < 0) { //clockwise
                        int topLeft = GetSubElementsI({0, 1});
                        int topRight = GetSubElementsI({0, 0});
                        int bottomLeft = GetSubElementsI({1, 1});
                        int bottomRight = GetSubElementsI({1, 0});
                        SetElements({topLeft, topRight, bottomLeft, bottomRight});
                    }else{
                        int topLeft = GetSubElementsI({1, 0});
                        int topRight = GetSubElementsI({1, 1});
                        int bottomLeft = GetSubElementsI({0, 0});
                        int bottomRight = GetSubElementsI({0, 1});
                        SetElements({topLeft, topRight, bottomLeft, bottomRight});
                    }
                    return;
                }

                if(GetFractalSize(this) == 4) {
                    if(angle < 0) { //clockwise
                        int e00 = GetSubElementsI({0, 3});
                        int e10 = GetSubElementsI({0, 2});
                        int e20 = GetSubElementsI({0, 1});
                        int e30 = GetSubElementsI({0, 0});

                        int e01 = GetSubElementsI({1, 3});
                        int e11 = GetSubElementsI({1, 2});
                        int e21 = GetSubElementsI({1, 1});
                        int e31 = GetSubElementsI({1, 0});

                        int e02 = GetSubElementsI({2, 3});
                        int e12 = GetSubElementsI({2, 2});
                        int e22 = GetSubElementsI({2, 1});
                        int e32 = GetSubElementsI({2, 0});

                        int e03 = GetSubElementsI({3, 3});
                        int e13 = GetSubElementsI({3, 2});
                        int e23 = GetSubElementsI({3, 1});
                        int e33 = GetSubElementsI({3, 0});

                        SetElements({e00, e10, e20, e30,
                                     e01, e11, e21, e31,
                                     e02, e12, e22, e32,
                                     e03, e13, e23, e33});
                    }else{
                        int e00 = GetSubElementsI({3, 0});
                        int e10 = GetSubElementsI({3, 1});
                        int e20 = GetSubElementsI({3, 2});
                        int e30 = GetSubElementsI({3, 3});

                        int e01 = GetSubElementsI({2, 0});
                        int e11 = GetSubElementsI({2, 1});
                        int e21 = GetSubElementsI({2, 2});
                        int e31 = GetSubElementsI({2, 3});

                        int e02 = GetSubElementsI({1, 0});
                        int e12 = GetSubElementsI({1, 1});
                        int e22 = GetSubElementsI({1, 2});
                        int e32 = GetSubElementsI({1, 3});

                        int e03 = GetSubElementsI({0, 0});
                        int e13 = GetSubElementsI({0, 1});
                        int e23 = GetSubElementsI({0, 2});
                        int e33 = GetSubElementsI({0, 3});

                        SetElements({e00, e10, e20, e30,
                                     e01, e11, e21, e31,
                                     e02, e12, e22, e32,
                                     e03, e13, e23, e33});
                    }
                    return;
                }

            }

            void SetElements(int elements) {
                int* ptr = (int*)&m_Elements;
                *(ptr) = elements;
            }

            void SetElements(glm::imat2 elements) {
                int* ptr = (int*)&m_Elements;
                int* ptrIn = (int*)&elements;
                for(int i = 0; i < 4; ++i) {
                    *(ptr + i) = *(ptrIn + i);
                }
            }

            void SetElements(glm::imat4 elements) {
                int* ptr = (int*)&m_Elements;
                int* ptrIn = (int*)&elements;
                for(int i = 0; i < 16; ++i) {
                    *(ptr + i) = *(ptrIn + i);
                }
            }

            void UpdateSprite() {
                int size = GetSize();
                float fWidth = UnitSize() * size;
                float fHeight = UnitSize() * size;


                glm::ivec2 texStart = GetTextureStart(GetIndex(), GetPuzzleNumber());


                rose::Sprite sprite = { {texStart.x, texStart.y - (size - 1) * UnitSize()}, {fWidth, fHeight}, rose::TextureType::Custom };
                Entity::SetSprite(sprite);
            }

            bool Contains(const glm::ivec2& index) const {
                if(index.x < GetIndex().x) return false;
                if(index.y < GetIndex().y) return false;
                if(index.x >= GetIndex().x + GetSize()) return false;
                if(index.y >= GetIndex().y + GetSize()) return false;
                return true;
            }

            int GetSubElementsI(const glm::ivec2& startOffset) const {
                const int* ptr = (const int*)&m_Elements;
                return ptr[startOffset.y * GetFractalSize((BaseFractal*)this) + startOffset.x];
            }

            glm::imat2 GetSubElementsIMat2(const glm::ivec2& startOffset) const {
                int arr[4];
                const int* ptr = (const int*)&m_Elements;
                for(int row = startOffset.y; row < startOffset.y + 2; ++row) {
                    for(int col = startOffset.x; col < startOffset.x + 2; ++col) {
                        arr[(row - startOffset.y) * 2 + (col - startOffset.x)] = ptr[row * GetFractalSize((BaseFractal*)this) + col];
                    }
                }
                return glm::make_mat2(arr);
            }

            glm::imat4 GetSubElementsIMat4(const glm::ivec2& startOffset) const {
                int arr[16];
                const int* ptr = (const int*)&m_Elements;
                for(int row = startOffset.y; row < startOffset.y + 4; ++row) {
                    for(int col = startOffset.x; col < startOffset.x + 4; ++col) {
                        arr[(row - startOffset.y) * 4 + (col - startOffset.x)] = ptr[row * GetFractalSize((BaseFractal*)this) + col];
                    }
                }
                return glm::make_mat4(arr);
            }

        private:
            Fractal(rose::EntityData e, const glm::ivec2& index): BaseFractal(e.sprite, e.size, e.boundingBox, e.position, index) {}

            static rose::EntityData MakeEntityData(const T& elements, const glm::ivec2& index, const glm::vec2& pos, int puzzleNumber) {
                UpdateTextureData(elements, index, puzzleNumber);

                int size = GetSize();
                float fWidth = UnitSize() * size;
                float fHeight = UnitSize() * size;

                glm::ivec2 texStart = GetTextureStart(index, puzzleNumber);

                glm::vec2 entitySize = glm::vec2(fWidth, fHeight);
                glm::vec4 boundingBox = glm::vec4(0.0f, 0.0f, fWidth, fHeight);
                rose::EntityData e;

                e.sprite = { {texStart.x, texStart.y - (size - 1) * UnitSize()}, {fWidth, fHeight}, rose::TextureType::Custom };
                e.size = entitySize;
                e.boundingBox = boundingBox;
                e.position = pos;
                
                return e;
            }

            static FractalElement GetElementData(int elements, int col, int row) {
                return (FractalElement)elements;
            }

            static FractalElement GetElementData(const glm::imat2& elements, int col, int row) {
                FractalElement* first = (FractalElement*)glm::value_ptr(elements);
                return first[ row * 2 + col];
            }

            static FractalElement GetElementData(const glm::imat4& elements, int col, int row) {
                FractalElement* first = (FractalElement*)glm::value_ptr(elements);
                return first[ row * 4 + col];
            }

            static void UpdateTextureData(const T& elements, const glm::ivec2& index, int puzzleNumber) {
                int size = GetSize();
                float fWidth = UnitSize() * size;
                float fHeight = UnitSize() * size;

                //textures start from bottom left, but starting textures from top left to fit fractal order (left to right, top to bottom)
                glm::ivec2 texStart = GetTextureStart(index, puzzleNumber);

                std::shared_ptr<rose::Renderer> renderer = (rose::Application::GetApplication())->GetRenderer();

                std::vector<rose::SubTextureMapping> texMapping;

                for(int row = 0; row < size; ++row) {
                    for(int col = 0; col < size; ++col) {
                        texMapping.push_back({{texStart.x + col * UnitSize(), texStart.y - row * UnitSize()}, {0, 0}, {UnitSize(), UnitSize()}}); //fractal frame
                        
                        //elements are drawn inside fractal frame, so start point is offset by 1 and side length is reduced by 2 in each dimension
                        FractalElement element = GetElementData(elements, col, row);

                        switch(element) {
                            case FractalElement::Red:
                                texMapping.push_back({{texStart.x + col * UnitSize() + 1, texStart.y - row * UnitSize() + 1}, 
                                                     {UnitSize() + 1, 1}, {UnitSize() - 2, UnitSize() - 2}});
                                break;
                            case FractalElement::Blue:
                                texMapping.push_back({{texStart.x + col * UnitSize() + 1, texStart.y - row * UnitSize() + 1}, 
                                                     {UnitSize() + 1, UnitSize() + 1}, {UnitSize() - 2, UnitSize() - 2}});
                                break;
                            case FractalElement::Green:
                                texMapping.push_back({{texStart.x + col * UnitSize() + 1, texStart.y - row * UnitSize() + 1}, 
                                                     {UnitSize() + 1, UnitSize() * 2 + 1}, {UnitSize() - 2, UnitSize() - 2}});
                                break;
                        }
                    }
                }

                renderer->SetCustomTexture(texMapping);
            }

            static glm::ivec2 GetTextureStart(const glm::ivec2& index, int puzzleNumber) {
                return glm::ivec2(index.x * UnitSize() + puzzleNumber * 256, 256 - (index.y + 1) * UnitSize()); 
            }
        private:
            T m_Elements;
            int m_PuzzleNumber {0};
    };






}

#endif //FRACTAL_H
