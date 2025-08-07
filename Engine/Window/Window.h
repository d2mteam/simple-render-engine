//
// Created by brosobad on 08/07/2025.
//

#ifndef WINDOW_H
#define WINDOW_H

namespace RenderEngine {
    struct ApplicationConfiguration;
    class EventDispatcher;
    struct InputState;

    class NativeWindow {
    public:
        virtual ~NativeWindow() = default;
        virtual bool Init(const ApplicationConfiguration&, EventDispatcher*) = 0;
        virtual void Shutdown() = 0;
        virtual void SwapBuffers() = 0;
        virtual void PollsEvent() = 0;
        virtual bool ShouldClose() = 0;
        virtual InputState* GetInputState() = 0;
        virtual float GetTimeSeconds() = 0;
    protected:
        NativeWindow() = default;
        NativeWindow(NativeWindow&) = default;
    };
}

#endif //WINDOW_H
