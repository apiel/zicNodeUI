#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <napi.h>

#include "zic_node_args.h"

#ifndef ZIC_DEFAULT_FONT
#define ZIC_DEFAULT_FONT "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"
#endif

#ifndef ZIC_DEFAULT_FONT_SIZE
#define ZIC_DEFAULT_FONT_SIZE 16
#endif

#ifndef ZIC_DEFAULT_FONT_COLOR
#define ZIC_DEFAULT_FONT_COLOR \
    {                          \
        255, 255, 255, 255     \
    }
#endif

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

Napi::Value getScreen(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    Napi::Object obj = Napi::Object::New(env);
    Napi::Object size = Napi::Object::New(env);
    size.Set("w", Napi::Number::New(env, screen.w));
    size.Set("h", Napi::Number::New(env, screen.h));
    obj.Set("size", size);

    int x, y;
    SDL_GetWindowPosition(window, &x, &y);
    Napi::Object pos = Napi::Object::New(env);
    pos.Set("x", Napi::Number::New(env, x));
    pos.Set("y", Napi::Number::New(env, y));
    obj.Set("position", pos);

    return obj;
}

Napi::Value open(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    int x = SDL_WINDOWPOS_UNDEFINED;
    int y = SDL_WINDOWPOS_UNDEFINED;
    int w = screen.w;
    int h = screen.h;

    if (info.Length() > 0) {
        Napi::Object obj = info[0].As<Napi::Object>();
        if (obj.Has("size") && !obj.Get("size").IsUndefined()) {
            Size size = getSize(env, obj.Get("size"));
            w = size.w;
            h = size.h;
        }
        if (obj.Has("position") && !obj.Get("position").IsUndefined()) {
            Napi::Object position = obj.Get("position").As<Napi::Object>();
            x = position.Get("x").As<Napi::Number>();
            y = position.Get("y").As<Napi::Number>();
        }
    }

    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Zic", x, y, w, h, SDL_WINDOW_SHOWN);

    TTF_Init();

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    return env.Undefined();
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
        Point position = getPoint(env, info[0]);
        SDL_RenderDrawPoint(renderer, position.x, position.y);
    } catch (const Napi::Error& e) {
        e.ThrowAsJavaScriptException();
    }
    return env.Undefined();
}

Napi::Value drawLine(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    try {
        Point position1, position2;
        getPoint(env, info[0], position1);
        getPoint(env, info[1], position2);
        SDL_RenderDrawLine(renderer, position1.x, position1.y, position2.x, position2.y);
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
        SDL_Rect sldRect = { (int)rect.position.x, (int)rect.position.y, (int)rect.size.w, (int)rect.size.h };
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
        SDL_Rect sldRect = { (int)rect.position.x, (int)rect.position.y, (int)rect.size.w, (int)rect.size.h };
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
        Point position = getPoint(env, info[1]);
        Color color = ZIC_DEFAULT_FONT_COLOR;
        uint32_t size = ZIC_DEFAULT_FONT_SIZE;
        std::string fontPath = ZIC_DEFAULT_FONT;

        if (info.Length() > 2) {
            if (info[2].IsObject()) {
                if (info[2].As<Napi::Object>().Has("color")) {
                    color = getColor(env, info[2].As<Napi::Object>().Get("color"));
                }
                if (info[2].As<Napi::Object>().Has("size")) {
                    size = getValueInRange(env, info[2].As<Napi::Object>().Get("size"), "size", 1, 255);
                }
                if (info[2].As<Napi::Object>().Has("font")) {
                    fontPath = info[2].As<Napi::Object>().Get("font").As<Napi::String>().Utf8Value();
                }
            } else {
                throw Napi::Error::New(env, "Invalid options argument: { color?: Color, size?: number }");
            }
        }

        TTF_Font* font = TTF_OpenFont(fontPath.c_str(), size);
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

        SDL_Rect rect = { (int)position.x, (int)position.y, surface->w, surface->h };
        SDL_RenderCopy(renderer, texture, NULL, &rect);

        Napi::Object result = Napi::Object::New(env);
        Napi::Object sizeObj = Napi::Object::New(env);
        sizeObj.Set("w", surface->w);
        sizeObj.Set("h", surface->h);
        result.Set("size", sizeObj);
        Napi::Object positionObj = Napi::Object::New(env);
        positionObj.Set("x", position.x);
        positionObj.Set("y", position.y);
        result.Set("position", positionObj);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
        TTF_CloseFont(font);

        return result;
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
    exports.Set(Napi::String::New(env, "getScreen"), Napi::Function::New(env, getScreen));
    exports.Set(Napi::String::New(env, "open"), Napi::Function::New(env, open));
    exports.Set(Napi::String::New(env, "close"), Napi::Function::New(env, close));
    exports.Set(Napi::String::New(env, "getEvents"), Napi::Function::New(env, getEvents));
    exports.Set(Napi::String::New(env, "render"), Napi::Function::New(env, render));
    exports.Set(Napi::String::New(env, "clear"), Napi::Function::New(env, clear));
    exports.Set(Napi::String::New(env, "setColor"), Napi::Function::New(env, setColor));
    exports.Set(Napi::String::New(env, "drawRect"), Napi::Function::New(env, drawRect));
    exports.Set(Napi::String::New(env, "drawFilledRect"), Napi::Function::New(env, drawFilledRect));
    exports.Set(Napi::String::New(env, "drawPoint"), Napi::Function::New(env, drawPoint));
    exports.Set(Napi::String::New(env, "drawLine"), Napi::Function::New(env, drawLine));
    exports.Set(Napi::String::New(env, "drawText"), Napi::Function::New(env, drawText));
    return exports;
}

NODE_API_MODULE(zic_ui, Init)
