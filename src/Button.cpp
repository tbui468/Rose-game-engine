#include "Button.h"

namespace sqs {

Button::Button(CartCoords pos, uint32_t depth) {
    SetPosition(pos);
    SetDepth(depth);
}

bool Button::OnClick() {
    return true;
}

}
