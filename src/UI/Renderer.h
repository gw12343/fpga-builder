//
// Created by gabed on 4/13/2026.
//

#pragma once
#include <SDL3/SDL_video.h>


class Renderer {
public:
    void InitWindow(int w, int h, const std::string &title);

    void CloseWindow() const;

    void StartFrame();
    void EndFrame() const;

    [[nodiscard]] bool IsRunning() const { return m_running; }

private:
    bool m_running = true;
    SDL_Window *m_window = nullptr;
    SDL_GLContext m_gl_context = nullptr;
};
