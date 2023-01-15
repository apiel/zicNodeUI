#ifndef ZIC_NODE_ARGS_H_
#define ZIC_NODE_ARGS_H_

#include <napi.h>

uint32_t getValueInRange(const Napi::Env& env, const Napi::Value& value, const std::string& name, uint32_t min, uint32_t max)
{
    if (!value.IsNumber()) {
        throw Napi::Error::New(env, name + " must be a number");
    }
    uint32_t v = value.As<Napi::Number>().Uint32Value();
    if (v < min || v > max) {
        throw Napi::Error::New(env, name + " out of range , min: " + std::to_string(min) + ", max: " + std::to_string(max));
    }
    return v;
}

uint32_t getArgsInRange(const Napi::CallbackInfo& info, uint32_t pos, const std::string& name, uint32_t min, uint32_t max)
{
    return getValueInRange(info.Env(), info[pos], name, min, max);
}

uint32_t* getColor(const Napi::Env& env, const Napi::Value& value)
{
    if (!value.IsObject()) {
        throw Napi::Error::New(env, "Color must be an object {r: number, g: number, b: number, a: number}");
    }
    static uint32_t color[4];
    color[0] = getValueInRange(env, value.As<Napi::Object>().Get("r"), "r", 0, 255);
    color[1] = getValueInRange(env, value.As<Napi::Object>().Get("g"), "g", 0, 255);
    color[2] = getValueInRange(env, value.As<Napi::Object>().Get("b"), "b", 0, 255);
    color[3] = value.As<Napi::Object>().Has("a") ? getValueInRange(env, value.As<Napi::Object>().Get("a"), "a", 0, 255)
                                                 : 255;
    return color;
}

#endif
