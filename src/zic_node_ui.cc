#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <napi.h>

#include "zic_node_args.h"

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
        Color& color = getColor(env, info[0]);
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
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
        Point point = getPoint(env, info[0]);
        SDL_RenderDrawPoint(renderer, point.x, point.y);
    } catch (const Napi::Error& e) {
        e.ThrowAsJavaScriptException();
    }
    return env.Undefined();
}

Napi::Value drawLine(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    try {
        Point point1, point2;
        getPoint(env, info[0], point1);
        getPoint(env, info[1], point2);
        SDL_RenderDrawLine(renderer, point1.x, point1.y, point2.x, point2.y);
    } catch (const Napi::Error& e) {
        e.ThrowAsJavaScriptException();
    }
    return env.Undefined();
}

Napi::Value drawRect(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    try {
        Rect rect = getRect(env, info[0]);
        SDL_Rect sldRect = { (int)rect.point.x, (int)rect.point.y, (int)rect.w, (int)rect.h };
        SDL_RenderDrawRect(renderer, &sldRect);
        return env.Undefined();
    } catch (const Napi::Error& e) {
        e.ThrowAsJavaScriptException();
        return env.Undefined();
    }
}

Napi::Value drawFilledRect(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    try {
        Rect rect = getRect(env, info[0]);
        SDL_Rect sldRect = { (int)rect.point.x, (int)rect.point.y, (int)rect.w, (int)rect.h };
        SDL_RenderFillRect(renderer, &sldRect);
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
        Point point = getPoint(env, info[1]);
        Color color = getColor(env, info[2]);
        uint32_t size = info.Length() > 3 ? getArgsInRange(info, 3, "size", 1, 255) : 16;

        // TTF_Font* font = TTF_OpenFont("FreeSans.ttf", size);
        TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeMono.ttf", size);
        if (font == NULL) {
            throw Napi::Error::New(env, "Failed to load font");
        }
        SDL_Color sdlColor = { (uint8_t)color.r, (uint8_t)color.g, (uint8_t)color.b, (uint8_t)color.a };
        SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), sdlColor);
        if (surface == NULL) {
            throw Napi::Error::New(env, "Failed to render text");
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (texture == NULL) {
            throw Napi::Error::New(env, "Failed to create texture");
        }

        SDL_Rect rect = { (int)point.x, (int)point.y, surface->w, surface->h };
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
    exports.Set(Napi::String::New(env, "drawFilledRect"), Napi::Function::New(env, drawFilledRect));
    exports.Set(Napi::String::New(env, "drawPoint"), Napi::Function::New(env, drawPoint));
    exports.Set(Napi::String::New(env, "drawLine"), Napi::Function::New(env, drawLine));
    exports.Set(Napi::String::New(env, "drawText"), Napi::Function::New(env, drawText));
    return exports;
}

NODE_API_MODULE(zic_ui, Init)
