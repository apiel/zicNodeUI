#ifndef ZIC_NODE_ARGS_H_
#define ZIC_NODE_ARGS_H_

#include <napi.h>

uint32_t getArgsInRange(const Napi::CallbackInfo& info, uint32_t pos, const std::string& name, uint32_t min, uint32_t max)
{
    Napi::Env env = info.Env();
    if (!info[pos].IsNumber()) {
        throw Napi::Error::New(env, name + " must be a number");
    }
    uint32_t value = info[pos].As<Napi::Number>().Uint32Value();
    if (value < min || value > max) {
        throw Napi::Error::New(env, name + " out of range , min: " + std::to_string(min) + ", max: " + std::to_string(max));
    }
    return value;
}

#endif
