#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

namespace sqs {

class BoundingBox {
    public:
        BoundingBox(){}
        BoundingBox(float x, float y, float w, float h): m_Box{x, y, w, y} {}
        virtual ~BoundingBox() = default;
        bool Collision(float x, float y) const {
            if(x < m_Box.x) return false;
            if(y < m_Box.y) return false;
            if(x > m_Box.x + m_Box.w) return false;
            if(y > m_Box.y + m_Box.h) return false;
            return true;
        }

        void SetBoundingCoords(float x, float y, float w, float h) {
            m_Box.x = x;
            m_Box.y = y;
            m_Box.w = w;
            m_Box.h = h;
        }
    private:
        Rectangle m_Box;
};

}

#endif //INTERACTABLE_H

