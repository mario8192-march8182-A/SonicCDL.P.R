#include "Game/GemstoneSonicController.h"

namespace sclpr::game {

void GemstoneSonicController::Activate() {
    m_active = true;
}

void GemstoneSonicController::Deactivate() {
    m_active = false;
}

bool GemstoneSonicController::IsActive() const noexcept {
    return m_active;
}

const TransformationSpec& GemstoneSonicController::GetSpec() const noexcept {
    return m_spec;
}

} // namespace sclpr::game
