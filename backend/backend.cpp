#include <napi.h>

#include <addon/database.hpp>

//static Napi::String Method(const Napi::CallbackInfo& info) {
//  Napi::Env env = info.Env();
//  return Napi::String::New(env, "Hello, world!");
//}
//
Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    using namespace addon::database;

    exports.Set(Napi::String::New(env, "OK"), Napi::Number::New(env, status_t::OK));
    exports.Set(Napi::String::New(env, "ARGUMENT_ERROR"), Napi::Number::New(env, status_t::ARGUMENT_ERROR));
    exports.Set(Napi::String::New(env, "INTERNAL_ERROR"), Napi::Number::New(env, status_t::INTERNAL_ERROR));
    exports.Set(Napi::String::New(env, "LOST_MESSAGE"), Napi::Number::New(env, status_t::LOST_MESSAGE));
    exports.Set(Napi::String::New(env, "NOT_INITIALIZED"), Napi::Number::New(env, status_t::NOT_INITIALIZED));

    return exports;
}

NODE_API_MODULE(backend, Init)