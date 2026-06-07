//
// Created by Andrii Krenevych on 10.06.2025.
//

#ifndef I_MOUSE_LISTENER_H_
#define I_MOUSE_LISTENER_H_

namespace Renderer {
    class IMouseListener {
    public:
        virtual ~IMouseListener() = default;

        virtual void onMove(double xPos, double yPos) {
        }

        virtual void onButtonDown(int button, double xPos, double yPos, const int mods) {
        }

        virtual void onButtonUp(int button, double xPos, double yPos, const int mods) {
        }

        virtual void onScroll(double xOffset, double yOffset) {
        }
    };
} // namespace Renderer
#endif //  I_MOUSE_LISTENER_H_
