#include <napi.h>
#include <addon/database.hpp>

#include <iostream>

namespace {

Napi::Object JSSideError(Napi::Env &env, const char *msg)
{
    Napi::Object obj = Napi::Object::New(env);

    const auto status = Napi::String::New(env, "status");
    const auto message = Napi::String::New(env, "message");

    if (env.IsExceptionPending())
    {
        Napi::Error e = env.GetAndClearPendingException();
        std::cout << "Exception during exception: JSSideError(): Exception during Napi::String::New(): "
            << e.Message() << std::endl;
        std::cout << "Primary exception: " << msg << std::endl;
        return Napi::Object::New(env);
    }

    obj.Set(status, static_cast<double>(addon::database::status_t::JS_SIDE_ERROR));
    obj.Set(message, msg);
    return obj;
}

Napi::Object CriticalError(Napi::Env &env, const char *msg)
{
    Napi::Error e = env.GetAndClearPendingException();
    std::cout << msg << e.Message();
    return Napi::Object::New(env);
}

} // namespace

Napi::Object Initialize(const Napi::CallbackInfo &info)
{
    using namespace addon::database;

    const auto result = initialize();

    Napi::Env env = info.Env();
    Napi::Object obj = Napi::Object::New(env);

    const auto status = Napi::String::New(env, "status");
    const auto message = Napi::String::New(env, "message");

    if (env.IsExceptionPending())
    {
        return CriticalError(env, "Initialize(): Exception during Napi::String::New(): ");
    }

    obj.Set(status, static_cast<double>(std::get<0>(result)));
    obj.Set(message, static_cast<const char *>(std::get<1>(result).data()));

    return obj;
}

Napi::Object GetTask(const Napi::CallbackInfo &info)
{
    using namespace addon::database;

    Napi::Env env = info.Env();

    if (info.Length() != 2)
    {
        return JSSideError(env, "GetTask(): Wrong count of arguments");
    }

    if (!info[0].IsNumber() || !info[1].IsNumber())
    {
        return JSSideError(env, "GetTask(): Wrong type of arguments");
    }

    const auto napi_client_id = info[0].As<Napi::Number>();
    const auto napi_group_id = info[1].As<Napi::Number>();

    if (env.IsExceptionPending())
    {
        return CriticalError(env, "GetTask(): Exception during Napi::Value::As(): ");
    }

    const uint_t client_id = napi_client_id.Uint32Value();
    const uint_t group_id = napi_group_id.Uint32Value();

    const auto status_string = Napi::String::New(env, "status");
    const auto message_string = Napi::String::New(env, "message");
    const auto client_id_string = Napi::String::New(env, "client_id");

    if (env.IsExceptionPending())
    {
        return CriticalError(env, "GetTask(): Exception during Napi::String::New(): ");
    }

    const auto result = get_task(client_id, group_id);
    Napi::Object obj = Napi::Object::New(env);
    obj.Set(status_string, static_cast<double>(std::get<0>(result)));
    obj.Set(message_string, static_cast<const char *>(std::get<1>(result).data()));
    obj.Set(client_id_string, static_cast<double>(std::get<2>(result)));

    return obj;
}

Napi::Object TaskCount(const Napi::CallbackInfo &info)
{
    using namespace addon::database;

    Napi::Env env = info.Env();

    if (info.Length() != 1)
    {
        return JSSideError(env, "TaskCount(): Wrong count of arguments");
    }

    if (!info[0].IsNumber())
    {
        return JSSideError(env, "TaskCount(): Wrong type of arguments");
    }

    const auto napi_group_id = info[0].As<Napi::Number>();

    if (env.IsExceptionPending())
    {
        return CriticalError(env, "TaskCount(): Exception during Napi::Value::As(): ");
    }

    const uint_t group_id = napi_group_id.Uint32Value();

    const auto status_string = Napi::String::New(env, "status");
    const auto message_string = Napi::String::New(env, "message");
    const auto count_string = Napi::String::New(env, "count");

    if (env.IsExceptionPending())
    {
        return CriticalError(env, "TaskCount(): Exception during Napi::String::New(): ");
    }

    const auto result = task_count(group_id);
    Napi::Object obj = Napi::Object::New(env);
    obj.Set(status_string, static_cast<double>(std::get<0>(result)));
    obj.Set(message_string, static_cast<const char *>(std::get<1>(result).data()));
    obj.Set(count_string, static_cast<double>(std::get<2>(result)));

    return obj;
}

Napi::Object ExamineTask(const Napi::CallbackInfo &info)
{
    using namespace addon::database;

    Napi::Env env = info.Env();

    if (info.Length() != 2)
    {
        return JSSideError(env, "ExamineTask(): Wrong count of arguments");
    }

    if (!info[0].IsNumber() || !info[1].IsNumber())
    {
        return JSSideError(env, "ExamineTask(): Wrong type of arguments");
    }

    const auto napi_group_id = info[0].As<Napi::Number>();
    const auto napi_task_id = info[1].As<Napi::Number>();

    if (env.IsExceptionPending())
    {
        return CriticalError(env, "ExamineTask(): Exception during Napi::Value::As(): ");
    }

    const uint_t group_id = napi_group_id.Uint32Value();
    const uint_t task_id = napi_task_id.Uint32Value();

    const auto status_string = Napi::String::New(env, "status");
    const auto message_string = Napi::String::New(env, "message");
    const auto removed_string = Napi::String::New(env, "removed");

    if (env.IsExceptionPending())
    {
        return CriticalError(env, "ExamineTask(): Exception during Napi::String::New(): ");
    }

    const auto result = examine_task(group_id, task_id);
    Napi::Object obj = Napi::Object::New(env);
    obj.Set(status_string, static_cast<double>(std::get<0>(result)));
    obj.Set(message_string, static_cast<const char *>(std::get<1>(result).data()));
    obj.Set(removed_string, static_cast<bool>(std::get<2>(result)));

    return obj;
}

Napi::Object SetRemovedFlag(const Napi::CallbackInfo &info)
{
    using namespace addon::database;

    Napi::Env env = info.Env();

    if (info.Length() != 3)
    {
        return JSSideError(env, "SetRemovedFlag(): Wrong count of arguments");
    }

    if (!info[0].IsNumber() || !info[1].IsNumber() || !info[2].IsBoolean())
    {
        return JSSideError(env, "SetRemovedFlag(): Wrong type of arguments");
    }

    const auto napi_group_id = info[0].As<Napi::Number>();
    const auto napi_task_id = info[1].As<Napi::Number>();
    const auto napi_removed = info[2].As<Napi::Boolean>();

    if (env.IsExceptionPending())
    {
        return CriticalError(env, "SetRemovedFlag(): Exception during Napi::Value::As(): ");
    }

    const uint_t group_id = napi_group_id.Uint32Value();
    const uint_t task_id = napi_task_id.Uint32Value();
    const bool removed = napi_removed.Value();

    const auto status_string = Napi::String::New(env, "status");
    const auto message_string = Napi::String::New(env, "message");

    if (env.IsExceptionPending())
    {
        return CriticalError(env, "SetRemovedFlag(): Exception during Napi::String::New(): ");
    }

    const auto result = set_removed_flag(group_id, task_id, removed);
    Napi::Object obj = Napi::Object::New(env);
    obj.Set(status_string, static_cast<double>(std::get<0>(result)));
    obj.Set(message_string, static_cast<const char *>(std::get<1>(result).data()));

    return obj;
}

Napi::Object UpdateTask(const Napi::CallbackInfo &info)
{
    using namespace addon::database;

    Napi::Env env = info.Env();

    if (info.Length() != 3)
    {
        return JSSideError(env, "UpdateTask(): Wrong count of arguments");
    }

    if (!info[0].IsNumber() || !info[1].IsNumber() || !info[2].IsString())
    {
        return JSSideError(env, "UpdateTask(): Wrong type of arguments");
    }

    const auto napi_group_id = info[0].As<Napi::Number>();
    const auto napi_task_id = info[1].As<Napi::Number>();
    const auto napi_task = info[2].As<Napi::String>();

    if (env.IsExceptionPending())
    {
        return CriticalError(env, "UpdateTask(): Exception during Napi::Value::As(): ");
    }

    const uint_t group_id = info[0].As<Napi::Number>().Uint32Value();
    const uint_t task_id = info[1].As<Napi::Number>().Uint32Value();
    const std::string task = info[2].As<Napi::String>().Utf8Value();

    const auto status_string = Napi::String::New(env, "status");
    const auto message_string = Napi::String::New(env, "message");

    if (env.IsExceptionPending())
    {
        return CriticalError(env, "UpdateTask(): Exception during Napi::String::New(): ");
    }

    const auto result = update_task(group_id, task_id, task);
    Napi::Object obj = Napi::Object::New(env);
    obj.Set(status_string, static_cast<double>(std::get<0>(result)));
    obj.Set(message_string, static_cast<const char *>(std::get<1>(result).data()));

    return obj;
}

Napi::Object AddTask(const Napi::CallbackInfo &info)
{
    using namespace addon::database;

    Napi::Env env = info.Env();

    if (info.Length() != 2)
    {
        return JSSideError(env, "GetTask(): Wrong count of arguments");
    }

    if (!info[0].IsNumber() || !info[1].IsString())
    {
        return JSSideError(env, "GetTask(): Wrong type of arguments");
    }

    const auto napi_client_id = info[0].As<Napi::Number>();
    const auto napi_task = info[1].As<Napi::String>();

    if (env.IsExceptionPending())
    {
        return CriticalError(env, "AddTask(): Exception during Napi::Value::As(): ");
    }

    const uint_t client_id = napi_client_id.Uint32Value();
    const std::string task = napi_task.Utf8Value();

    const auto status_string = Napi::String::New(env, "status");
    const auto message_string = Napi::String::New(env, "message");

    if (env.IsExceptionPending())
    {
        return CriticalError(env, "AddTask(): Exception during Napi::String::New(): ");
    }

    const auto result = add_task(client_id, task);
    Napi::Object obj = Napi::Object::New(env);
    obj.Set(status_string, static_cast<double>(std::get<0>(result)));
    obj.Set(message_string, static_cast<const char *>(std::get<1>(result).data()));

    return obj;
}

Napi::Object Serialize(const Napi::CallbackInfo &info)
{
    using namespace addon::database;

    Napi::Env env = info.Env();

    if (info.Length() != 1)
    {
        return JSSideError(env, "Serialize(): Wrong count of arguments");
    }

    if (!info[0].IsString())
    {
        return JSSideError(env, "Serialize(): Wrong type of arguments");
    }

    const auto napi_to_folder = info[0].As<Napi::String>();

    if (env.IsExceptionPending())
    {
        return CriticalError(env, "Serialize(): Exception during Napi::Value::As(): ");
    }

    const std::string to_folder = napi_to_folder.Utf8Value();

    const auto status_string = Napi::String::New(env, "status");
    const auto message_string = Napi::String::New(env, "message");

    if (env.IsExceptionPending())
    {
        return CriticalError(env, "Serialize(): Exception during Napi::String::New(): ");
    }

    const auto result = serialize(to_folder);
    Napi::Object obj = Napi::Object::New(env);
    obj.Set(status_string, static_cast<double>(std::get<0>(result)));
    obj.Set(message_string, static_cast<const char *>(std::get<1>(result).data()));

    return obj;
}

Napi::Object Deserialize(const Napi::CallbackInfo &info)
{
    using namespace addon::database;

    Napi::Env env = info.Env();

    if (info.Length() != 1)
    {
        return JSSideError(env, "Deserialize(): Wrong count of arguments");
    }

    if (!info[0].IsString())
    {
        return JSSideError(env, "Deserialize(): Wrong type of arguments");
    }

    const auto napi_from_folder = info[0].As<Napi::String>();

    if (env.IsExceptionPending())
    {
        return CriticalError(env, "Deserialize(): Exception during Napi::Value::As(): ");
    }

    const std::string from_folder = napi_from_folder.Utf8Value();

    const auto status_string = Napi::String::New(env, "status");
    const auto message_string = Napi::String::New(env, "message");

    if (env.IsExceptionPending())
    {
        return CriticalError(env, "Deserialize(): Exception during Napi::String::New(): ");
    }

    const auto result = deserialize(from_folder);
    Napi::Object obj = Napi::Object::New(env);
    obj.Set(status_string, static_cast<double>(std::get<0>(result)));
    obj.Set(message_string, static_cast<const char *>(std::get<1>(result).data()));

    return obj;
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    using namespace addon::database;

    exports.Set(Napi::String::New(env, "OK"), Napi::Number::New(env, status_t::OK));
    exports.Set(Napi::String::New(env, "ARGUMENT_ERROR"), Napi::Number::New(env, status_t::ARGUMENT_ERROR));
    exports.Set(Napi::String::New(env, "INTERNAL_ERROR"), Napi::Number::New(env, status_t::INTERNAL_ERROR));
    exports.Set(Napi::String::New(env, "LOST_MESSAGE"), Napi::Number::New(env, status_t::LOST_MESSAGE));
    exports.Set(Napi::String::New(env, "NOT_INITIALIZED"), Napi::Number::New(env, status_t::NOT_INITIALIZED));
    exports.Set(Napi::String::New(env, "JS_SIDE_ERROR"), Napi::Number::New(env, status_t::JS_SIDE_ERROR));

    exports.Set(Napi::String::New(env, "UNDEFINED_CLIENT_ID"), Napi::Number::New(env, database::UNDEFINED_CLIENT_ID));

    exports.Set(Napi::String::New(env, "initialize"), Napi::Function::New(env, Initialize));
    exports.Set(Napi::String::New(env, "get_task"), Napi::Function::New(env, GetTask));
    exports.Set(Napi::String::New(env, "task_count"), Napi::Function::New(env, TaskCount));
    exports.Set(Napi::String::New(env, "examine_task"), Napi::Function::New(env, ExamineTask));
    exports.Set(Napi::String::New(env, "set_removed_flag"), Napi::Function::New(env, SetRemovedFlag));
    exports.Set(Napi::String::New(env, "update_task"), Napi::Function::New(env, UpdateTask));
    exports.Set(Napi::String::New(env, "add_task"), Napi::Function::New(env, AddTask));
    exports.Set(Napi::String::New(env, "serialize"), Napi::Function::New(env, Serialize));
    exports.Set(Napi::String::New(env, "deserialize"), Napi::Function::New(env, Deserialize));

    return exports;
}

NODE_API_MODULE(backend, Init)