#include <SDL2/SDL.h>
#include <napi.h>

#include "zic_node_args.h"

#define SCREEN_W 480
#define SCREEN_H 320

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;

Napi::Value open(const Napi::CallbackInfo& info)
{
    window = SDL_CreateWindow(
        "Zic", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_W, SCREEN_H,
        SDL_WINDOW_SHOWN);

    screenSurface = SDL_GetWindowSurface(window);

    return info.Env().Undefined();
}

Napi::Value render(const Napi::CallbackInfo& info)
{
    SDL_UpdateWindowSurface(window);
    return info.Env().Undefined();
}

Napi::Value rgb(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    try {
        uint32_t r = getArgsInRange(info, 0, "r", 0, 255);
        uint32_t g = getArgsInRange(info, 1, "g", 0, 255);
        uint32_t b = getArgsInRange(info, 2, "b", 0, 255);
        return Napi::Number::New(env, SDL_MapRGB(screenSurface->format, r, g, b));
    } catch (const Napi::Error& e) {
        e.ThrowAsJavaScriptException();
        return env.Undefined();
    }
}

Napi::Value drawRect(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    try {
        uint32_t x = getArgsInRange(info, 0, "x", 0, SCREEN_W - 1);
        uint32_t y = getArgsInRange(info, 1, "y", 0, SCREEN_H - 1);
        uint32_t w = getArgsInRange(info, 2, "w", 1, SCREEN_W - x);
        uint32_t h = getArgsInRange(info, 3, "h", 1, SCREEN_H - y);
        uint32_t color = getArgsInRange(info, 4, "color", 0, 0xFFFFFF);
        bool filled = info.Length() > 5 && info[5].As<Napi::Boolean>().Value();
        SDL_Rect rect = { (int)x, (int)y, (int)w, (int)h };
        // if (filled) {
            SDL_FillRect(screenSurface, &rect, color);
        // } else {
            // SDL_drawRect(screenSurface, &rect, color);
        // }
        return env.Undefined();
    } catch (const Napi::Error& e) {
        e.ThrowAsJavaScriptException();
        return env.Undefined();
    }
}

Napi::Value close(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    SDL_DestroyWindow(window);
    SDL_Quit();

    return env.Undefined();
}

Napi::Object getEvents(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    Napi::Object events = Napi::Object::New(env);
    Napi::Array keysDown = Napi::Array::New(env);
    Napi::Array keysUp = Napi::Array::New(env);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            events.Set("exit", Napi::Boolean::New(env, true));
            break;
        case SDL_KEYDOWN:
            keysDown.Set(keysDown.Length(), Napi::Number::New(env, event.key.keysym.scancode));
            break;
        case SDL_KEYUP:
            keysUp.Set(keysUp.Length(), Napi::Number::New(env, event.key.keysym.scancode));
            break;
        }
    }
    if (keysDown.Length() > 0) {
        events.Set("keysDown", keysDown);
    }
    if (keysUp.Length() > 0) {
        events.Set("keysUp", keysUp);
    }
    return events;
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "open"), Napi::Function::New(env, open));
    exports.Set(Napi::String::New(env, "close"), Napi::Function::New(env, close));
    exports.Set(Napi::String::New(env, "getEvents"), Napi::Function::New(env, getEvents));
    exports.Set(Napi::String::New(env, "render"), Napi::Function::New(env, render));
    exports.Set(Napi::String::New(env, "rgb"), Napi::Function::New(env, rgb));
    exports.Set(Napi::String::New(env, "drawRect"), Napi::Function::New(env, drawRect));
    return exports;
}

NODE_API_MODULE(zic_ui, Init)
