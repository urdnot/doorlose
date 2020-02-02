#include <napi.h>

#include <addon/database.hpp>

namespace {

Napi::Object ExposeError(const Napi::Env &env, const char *msg)
{
    Napi::Object obj = Napi::Object::New(env);
    obj.Set(Napi::String::New(env, "status"), static_cast<double>(addon::database::status_t::EXPOSE_ERROR));
    obj.Set(Napi::String::New(env, "message"), msg);
    return obj;
}

} // namespace

Napi::Object Initialize(const Napi::CallbackInfo &info)
{
    using namespace addon::database;

    const auto result = initialize();

    Napi::Env env = info.Env();
    Napi::Object obj = Napi::Object::New(env);
    obj.Set(Napi::String::New(env, "status"), static_cast<double>(std::get<0>(result)));
    obj.Set(Napi::String::New(env, "message"), static_cast<const char *>(std::get<1>(result).data()));

    return obj;
}

Napi::Object GetTask(const Napi::CallbackInfo &info)
{
    using namespace addon::database;

    Napi::Env env = info.Env();

    if (info.Length() != 2)
    {
        return ExposeError(env, "GetTask(): Wrong count of arguments");
    }

    if (!info[0].IsNumber() || !info[1].IsNumber())
    {
        return ExposeError(env, "GetTask(): Wrong type of arguments");
    }

    const uint_t client_id = info[0].As<Napi::Number>().Uint32Value();
    const uint_t group_id = info[1].As<Napi::Number>().Uint32Value();

    const auto result = get_task(client_id, group_id);
    Napi::Object obj = Napi::Object::New(env);
    obj.Set(Napi::String::New(env, "status"), static_cast<double>(std::get<0>(result)));
    obj.Set(Napi::String::New(env, "message"), static_cast<const char *>(std::get<1>(result).data()));
    obj.Set(Napi::String::New(env, "client_id"), static_cast<double>(std::get<2>(result)));

    return obj;
}

//Napi::Object TaskCount(const Napi::CallbackInfo &info)
//{
//    using namespace addon::database;
//}
//
//Napi::Object ExamineTask(const Napi::CallbackInfo &info)
//{
//    using namespace addon::database;
//}
//
//Napi::Object SetRemovedFlag(const Napi::CallbackInfo &info)
//{
//    using namespace addon::database;
//}
//
//Napi::Object UpdateTask(const Napi::CallbackInfo &info)
//{
//    using namespace addon::database;
//}

Napi::Object AddTask(const Napi::CallbackInfo &info)
{
    using namespace addon::database;

    Napi::Env env = info.Env();

    if (info.Length() != 2)
    {
        return ExposeError(env, "GetTask(): Wrong count of arguments");
    }

    if (!info[0].IsNumber() || !info[1].IsString())
    {
        return ExposeError(env, "GetTask(): Wrong type of arguments");
    }

    const uint_t client_id = info[0].As<Napi::Number>().Uint32Value();
    const std::string task = info[1].As<Napi::String>().Utf8Value();

    const auto result = add_task(client_id, task);
    Napi::Object obj = Napi::Object::New(env);
    obj.Set(Napi::String::New(env, "status"), static_cast<double>(std::get<0>(result)));
    obj.Set(Napi::String::New(env, "message"), static_cast<const char *>(std::get<1>(result).data()));

    return obj;
}

//Napi::Object Serialize(const Napi::CallbackInfo &info)
//{
//    using namespace addon::database;
//}
//
//Napi::Object Deserialize(const Napi::CallbackInfo &info)
//{
//    using namespace addon::database;
//}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    using namespace addon::database;

    exports.Set(Napi::String::New(env, "OK"), Napi::Number::New(env, status_t::OK));
    exports.Set(Napi::String::New(env, "ARGUMENT_ERROR"), Napi::Number::New(env, status_t::ARGUMENT_ERROR));
    exports.Set(Napi::String::New(env, "INTERNAL_ERROR"), Napi::Number::New(env, status_t::INTERNAL_ERROR));
    exports.Set(Napi::String::New(env, "LOST_MESSAGE"), Napi::Number::New(env, status_t::LOST_MESSAGE));
    exports.Set(Napi::String::New(env, "NOT_INITIALIZED"), Napi::Number::New(env, status_t::NOT_INITIALIZED));
    exports.Set(Napi::String::New(env, "EXPOSE_ERROR"), Napi::Number::New(env, status_t::EXPOSE_ERROR));

    exports.Set(Napi::String::New(env, "UNDEFINED_CLIENT_ID"), Napi::Number::New(env, database::UNDEFINED_CLIENT_ID));

    exports.Set(Napi::String::New(env, "initialize"), Napi::Function::New(env, Initialize));
    exports.Set(Napi::String::New(env, "get_task"), Napi::Function::New(env, GetTask));
    //exports.Set(Napi::String::New(env, "task_count"), Napi::Function::New(env, TaskCount));
    //exports.Set(Napi::String::New(env, "examine_task"), Napi::Function::New(env, ExamineTask));
    //exports.Set(Napi::String::New(env, "set_removed_flag"), Napi::Function::New(env, SetRemovedFlag));
    //exports.Set(Napi::String::New(env, "update_task"), Napi::Function::New(env, UpdateTask));
    exports.Set(Napi::String::New(env, "add_task"), Napi::Function::New(env, AddTask));
    //exports.Set(Napi::String::New(env, "serialize"), Napi::Function::New(env, Serialize));
    //exports.Set(Napi::String::New(env, "deserialize"), Napi::Function::New(env, Deserialize));

    return exports;
}

NODE_API_MODULE(backend, Init)