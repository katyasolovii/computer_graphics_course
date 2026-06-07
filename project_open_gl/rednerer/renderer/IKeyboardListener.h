//
// Created by Andrii Krenevych on 10.06.2025.
//

#ifndef I_KEYBOARD_LISTENER_H
#define I_KEYBOARD_LISTENER_H

namespace Renderer {
    class IKeyboardListener {
    public:
        virtual ~IKeyboardListener() = default;

        virtual void onKeyUp(int key, int mods) {
        };

        virtual void onKeyDown(int key, int mods) {
        };

        virtual void onKeyRepeat(int key, int mods) {
        };
    };
} // namespace Renderer

#endif //I_KEYBOARD_LISTENER_H
