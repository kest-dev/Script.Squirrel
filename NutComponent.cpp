#include "NutComponent.h"

#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Resource/ResourceCache.h>

#include <sqstdio.h>
#include <sqstdaux.h>

namespace Urho3D
{

NutComponent::NutComponent(Context* context)
    : LogicComponent(context)
{
}

void NutComponent::RegisterObject(Context* context)
{
    context->AddFactoryReflection<NutComponent>(Category_User);

    URHO3D_ACCESSOR_ATTRIBUTE("Script", GetScriptFileAttr, SetScriptFileAttr, ResourceRef, ResourceRef(ScriptFile::GetTypeStatic()), AM_DEFAULT);
}

void NutComponent::squirrel_print(HSQUIRRELVM v, const SQChar* s, ...) {
    char buffer[1024]; // Adjust size as needed
    va_list args;
    va_start(args, s);
    vsnprintf(buffer, sizeof(buffer), s, args);
    va_end(args);

    URHO3D_LOGDEBUGF("[SQUIRREL] %s", buffer);
}

void NutComponent::squirrel_error(HSQUIRRELVM v, const SQChar* s, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, s);
    vsnprintf(buffer, sizeof(buffer), s, args);
    va_end(args);

    URHO3D_LOGERRORF("[SQUIRREL] %s", buffer);
}

ResourceRef NutComponent::GetScriptFileAttr() const
{
    return GetResourceRef(scriptFile_, ScriptFile::GetTypeStatic());
}

void NutComponent::SetScriptFileAttr(const ResourceRef& value)
{
    auto* cache = GetSubsystem<ResourceCache>();
    scriptFile_ = cache->GetResource<ScriptFile>(value.name_);
}

void NutComponent::DelayedStart()
{
    if (!scriptFile_)
        return;

    vm_ = sq_open(1024);

    sqstd_register_iolib(vm_);

    sqstd_seterrorhandlers(vm_);

    sq_setprintfunc(vm_, squirrel_print, squirrel_error);
    sq_pushroottable(vm_);

    if (SQ_SUCCEEDED(sq_compilebuffer(vm_, scriptFile_->GetText(), strlen(scriptFile_->GetText()), "", true)))
    {
        sq_pushroottable(vm_);

        // 5. Call the compiled script
        if (SQ_FAILED(sq_call(vm_, 1, SQFalse, SQTrue))) {
            URHO3D_LOGDEBUG("Failed to run script");
        }
    }
    else
        {
        URHO3D_LOGERROR("Failed to compile script");
    }
    sq_pop(vm_, 1);
    sq_close(vm_);
}

void NutComponent::Update(float timeStep)
{

}

} // namespace Urho3D
