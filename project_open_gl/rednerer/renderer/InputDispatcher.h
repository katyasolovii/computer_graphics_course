//
// Created by Andrii Krenevych on 10.06.2025.
//

#ifndef INPUT_DISPATCHER_H_
#define INPUT_DISPATCHER_H_
#include <vector>

namespace Renderer {
    class IKeyboardListener;
    class IMouseListener;

    class InputDispatcher {
    public:
        static InputDispatcher& getInstance();

        void addKeyboardListener(IKeyboardListener* listener);

        void removeKeyboardListener(IKeyboardListener* listener);

        void addMouseListener(IMouseListener* listener);

        void removeMouseListener(IMouseListener* listener);

        void dispatchKey(int key, int scancode, int action, int mods) const;

        void dispatchMouseButton(int button, double xPos, double yPos, int action, int mods) const;

        void dispatchMouseMove(double xPos, double yPos) const;

        void dispatchScroll(double xOffset, double yOffset) const;

        InputDispatcher(const InputDispatcher&) = delete;

        InputDispatcher& operator=(const InputDispatcher&) = delete;

    private:
        InputDispatcher() = default; // конструктор приватний
        ~InputDispatcher() = default;

        std::vector<IKeyboardListener*> keyboardListeners;
        std::vector<IMouseListener*> mouseListeners;
    };

#endif // INPUT_DISPATCHER_H_
} // namespace Renderer
