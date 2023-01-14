#include <SDL2/SDL.h>
#include <napi.h>

#define SCREEN_W 480
#define SCREEN_H 320

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

Napi::Value open(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    window = SDL_CreateWindow(
        "Zic", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_W, SCREEN_H,
        SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    return env.Undefined();
}

Napi::Value close(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    SDL_DestroyWindow(window);
    SDL_Quit();

    return env.Undefined();
}

Napi::Boolean shouldExit(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            return Napi::Boolean::New(env, true);
        }
    }

    return Napi::Boolean::New(env, false);
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "open"), Napi::Function::New(env, open));
    exports.Set(Napi::String::New(env, "close"), Napi::Function::New(env, close));
    exports.Set(Napi::String::New(env, "shouldExit"), Napi::Function::New(env, shouldExit));
    return exports;
}

NODE_API_MODULE(zic_ui, Init)
