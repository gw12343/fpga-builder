//
// Created by gabed on 4/13/2026.
//

#pragma once
#include <SDL3/SDL_video.h>
#include <string>


class Renderer {
public:
    void InitWindow(int w, int h, const std::string &title);

    void CloseWindow() const;

    void StartFrame();
    void EndFrame() const;

    [[nodiscard]] bool IsRunning() const { return running; };

private:
    bool running = true;
    SDL_Window *window = nullptr;
    SDL_GLContext gl_context = nullptr;
};
