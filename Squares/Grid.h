#ifndef GRID_H
#define GRID_H

#include <vector>

namespace sqs {

    template <class T>
    class Grid {
        public:
            Grid() = default;
            Grid(const std::vector<T>& elements): m_Elements(elements) {
                m_Size = floor(sqrt(elements.size() + 1));
            }
            virtual ~Grid() = default;
            T At(int col, int row) const { 
                assert(row * m_Size + col  < m_Size * m_Size);
                return m_Elements.at(row * m_Size + col); 
            }
            void RotateCW() {}
            void RotateCCW() {}
            void ReflectX() {}
            void ReflectY() {}
            int GetSize() const { return m_Size; }
            void SetElements(const std::vector<T>& elements) {
                m_Elements = elements;
                m_Size = floor(sqrt(elements.size() + 1));
            }
            std::vector<T> GetSubElements(const glm::ivec2& start, int size) const {
                std::vector<T> subElements;

                for(int row = start.y; row < start.y + size; ++row) {
                    for(int col = start.x; col < start.x + size; ++col) {
                        subElements.emplace_back(At(col, row)); 
                    }
                }

                return subElements;
            }
        private:
            std::vector<T> m_Elements;
            int m_Size {0};
    };

}


#endif //GRID_H
