#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <napi.h>

#include "zic_node_args.h"

#define SCREEN_W 480
#define SCREEN_H 320

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

Napi::Value open(const Napi::CallbackInfo& info)
{
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(
        "Zic", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_W, SCREEN_H,
        SDL_WINDOW_SHOWN);

    TTF_Init();

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    return info.Env().Undefined();
}

Napi::Value render(const Napi::CallbackInfo& info)
{
    SDL_RenderPresent(renderer);
    return info.Env().Undefined();
}

Napi::Value setColor(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    try {
        uint32_t r = getArgsInRange(info, 0, "r", 0, 255);
        uint32_t g = getArgsInRange(info, 1, "g", 0, 255);
        uint32_t b = getArgsInRange(info, 2, "b", 0, 255);
        uint32_t a = info.Length() > 3 ? getArgsInRange(info, 3, "a", 0, 255) : 0xff;
        SDL_SetRenderDrawColor(renderer, r, g, b, a);
    } catch (const Napi::Error& e) {
        e.ThrowAsJavaScriptException();
    }
    return env.Undefined();
}

Napi::Value clear(const Napi::CallbackInfo& info)
{
    if (info.Length() > 0) {
        setColor(info);
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
    }
    SDL_RenderClear(renderer);
    return info.Env().Undefined();
}

Napi::Value drawPoint(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    try {
        uint32_t x = getArgsInRange(info, 0, "x", 0, SCREEN_W - 1);
        uint32_t y = getArgsInRange(info, 1, "y", 0, SCREEN_H - 1);
        SDL_RenderDrawPoint(renderer, x, y);
    } catch (const Napi::Error& e) {
        e.ThrowAsJavaScriptException();
    }
    return env.Undefined();
}

Napi::Value drawLine(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    try {
        uint32_t x1 = getArgsInRange(info, 0, "x1", 0, SCREEN_W - 1);
        uint32_t y1 = getArgsInRange(info, 1, "y1", 0, SCREEN_H - 1);
        uint32_t x2 = getArgsInRange(info, 2, "x2", 0, SCREEN_W - 1);
        uint32_t y2 = getArgsInRange(info, 3, "y2", 0, SCREEN_H - 1);
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    } catch (const Napi::Error& e) {
        e.ThrowAsJavaScriptException();
    }
    return env.Undefined();
}

Napi::Value drawRect(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    try {
        uint32_t x = getArgsInRange(info, 0, "x", 0, SCREEN_W - 1);
        uint32_t y = getArgsInRange(info, 1, "y", 0, SCREEN_H - 1);
        uint32_t w = getArgsInRange(info, 2, "w", 1, SCREEN_W - x);
        uint32_t h = getArgsInRange(info, 3, "h", 1, SCREEN_H - y);
        bool filled = info.Length() > 4 && info[4].As<Napi::Boolean>().Value();
        SDL_Rect rect = { (int)x, (int)y, (int)w, (int)h };
        if (filled) {
            SDL_RenderFillRect(renderer, &rect);
        } else {
            SDL_RenderDrawRect(renderer, &rect);
        }
        return env.Undefined();
    } catch (const Napi::Error& e) {
        e.ThrowAsJavaScriptException();
        return env.Undefined();
    }
}

Napi::Value drawText(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    try {
        std::string text = info[0].As<Napi::String>().Utf8Value();
        uint32_t x = getArgsInRange(info, 1, "x", 0, SCREEN_W - 1);
        uint32_t y = getArgsInRange(info, 2, "y", 0, SCREEN_H - 1);
        // TODO make rgba object
        uint32_t r = getArgsInRange(info, 3, "r", 0, 255);
        uint32_t g = getArgsInRange(info, 4, "g", 0, 255);
        uint32_t b = getArgsInRange(info, 5, "b", 0, 255);
        uint32_t size = info.Length() > 6 ? getArgsInRange(info, 6, "size", 1, 255) : 16;
        uint32_t a = info.Length() > 7 ? getArgsInRange(info, 7, "a", 0, 255) : 0xff;

        // TTF_Font* font = TTF_OpenFont("FreeSans.ttf", size);
        TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeMono.ttf", size);
        if (font == NULL) {
            throw Napi::Error::New(env, "Failed to load font");
        }

        SDL_Color color = { (uint8_t)r, (uint8_t)g, (uint8_t)b, (uint8_t)a };
        SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
        if (surface == NULL) {
            throw Napi::Error::New(env, "Failed to render text");
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (texture == NULL) {
            throw Napi::Error::New(env, "Failed to create texture");
        }

        SDL_Rect rect = { (int)x, (int)y, surface->w, surface->h };
        SDL_RenderCopy(renderer, texture, NULL, &rect);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
        TTF_CloseFont(font);
    } catch (const Napi::Error& e) {
        e.ThrowAsJavaScriptException();
    }
    return env.Undefined();
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
    exports.Set(Napi::String::New(env, "setColor"), Napi::Function::New(env, setColor));
    exports.Set(Napi::String::New(env, "drawRect"), Napi::Function::New(env, drawRect));
    exports.Set(Napi::String::New(env, "drawPoint"), Napi::Function::New(env, drawPoint));
    exports.Set(Napi::String::New(env, "drawLine"), Napi::Function::New(env, drawLine));
    exports.Set(Napi::String::New(env, "drawText"), Napi::Function::New(env, drawText));
    return exports;
}

NODE_API_MODULE(zic_ui, Init)
