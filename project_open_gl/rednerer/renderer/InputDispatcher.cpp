//
// Created by Andrii Krenevych on 10.06.2025.
//

#include "InputDispatcher.h"

#include <algorithm>

#include "IKeyboardListener.h"
#include "IMouseListener.h"
#include "GLFW/glfw3.h"

namespace Renderer {
    InputDispatcher& InputDispatcher::getInstance() {
        static InputDispatcher instance;
        return instance;
    }

    void InputDispatcher::addKeyboardListener(IKeyboardListener* listener) {
        keyboardListeners.push_back(listener);
    }

    void InputDispatcher::removeKeyboardListener(IKeyboardListener* listener) {
        keyboardListeners.erase(
            std::remove(keyboardListeners.begin(), keyboardListeners.end(), listener),
            keyboardListeners.end());
    }

    void InputDispatcher::addMouseListener(IMouseListener* listener) {
        mouseListeners.push_back(listener);
    }

    void InputDispatcher::removeMouseListener(IMouseListener* listener) {
        mouseListeners.erase(
            std::remove(mouseListeners.begin(), mouseListeners.end(), listener),
            mouseListeners.end());
    }

    void InputDispatcher::dispatchKey(const int key, const int scancode, const int action, const int mods) const {
        for (auto* l: keyboardListeners) {
            switch (action) {
                case GLFW_PRESS:
                    l->onKeyDown(key, mods);
                    break;
                case GLFW_REPEAT:
                    l->onKeyRepeat(key, mods);
                    break;
                case GLFW_RELEASE:
                    l->onKeyUp(key, mods);
                    break;
                default: ;
            }
        }
    }

    void InputDispatcher::dispatchMouseButton(
        const int button,
        double xPos, double yPos,
        const int action,
        const int mods
    ) const {
        for (auto* l: mouseListeners) {
            if (action == GLFW_PRESS) {
                l->onButtonDown(button, xPos, yPos, mods);
            } else if (action == GLFW_RELEASE) {
                l->onButtonUp(button, xPos, yPos, mods);
            }
        }
    }

    void InputDispatcher::dispatchMouseMove(const double xPos, const double yPos) const {
        for (auto* l: mouseListeners) {
            l->onMove(xPos, yPos);
        }
    }

    void InputDispatcher::dispatchScroll(const double xOffset, const double yOffset) const {
        for (auto* l: mouseListeners) {
            l->onScroll(xOffset, yOffset);
        }
    }
} // namespace Renderer
