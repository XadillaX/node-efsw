/**
 * node-efsw - Node.js binding for EFSW
 *
 * Copyright (c) 2017 XadillaX <i@2333.moe>
 *
 * MIT License <https://github.com/XadillaX/node-efsw/blob/master/LICENSE>
 */
#include "efsw_core.h"
using namespace efsw;
using namespace v8;

namespace efsw_core {

EFSWCore::EFSWCore(const char* path, Nan::Callback* listener) :
    path(path),
    listener(listener),
    core_listener(listener)
{
    watcher = new FileWatcher();
    watch_id = 0;
}

EFSWCore::~EFSWCore()
{
    Nan::HandleScope scope;
    delete watcher;
    delete listener;
}

WatchId EFSWCore::Start()
{
    if(watch_id > 0)
    {
        return -100;
    }

    WatchId ret = watcher->AddWatch(path.c_str(), &core_listener, true);
    if(ret > 0) {
        watch_id = ret;
        watcher->watch();
    }

    return ret;
}

void EFSWCore::Stop()
{
    if(watch_id <= 0) return;

    watcher->RemoveWatch(watch_id);
    watch_id = 0;
}

NAN_METHOD(EFSWCore::New)
{
    v8::Isolate* isolate = info.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();

    Nan::Utf8String path(info[0]->ToString(context).ToLocalChecked());
    Nan::Callback* listener = new Nan::Callback(info[1].As<v8::Function>());

    EFSWCore* core = new EFSWCore(*path, listener);
    core->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(EFSWCore::Start)
{
    EFSWCore* core = Nan::ObjectWrap::Unwrap<EFSWCore>(info.Holder());
    WatchId watch_id = core->Start();

    if(watch_id < 0)
    {
        if(watch_id == -100)
        {
            return Nan::ThrowError("Watcher is already in watching.");
        }

        return Nan::ThrowError(Errors::Log::GetLastErrorLog().c_str());
    }
}

NAN_METHOD(EFSWCore::Stop)
{
    EFSWCore* core = Nan::ObjectWrap::Unwrap<EFSWCore>(info.Holder());
    core->Stop();
}

NAN_MODULE_INIT(EFSWCore::Init)
{
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("EFSWCore").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(tpl, "start", Start);
    Nan::SetPrototypeMethod(tpl, "stop", Stop);

    Nan::Set(target, Nan::New("EFSWCore").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

}
