#pragma once

#include "ScriptFile.h"
#include "_Plugin.h"

#include <Urho3D/Scene/LogicComponent.h>

#include <squirrel.h>


namespace Urho3D
{

class PLUGIN_SCRIPT_SQUIRREL_API NutComponent : public LogicComponent
{
    URHO3D_OBJECT(NutComponent, LogicComponent);

public:
    NutComponent(Context* context);
    static void RegisterObject(Context* context);

    ResourceRef GetScriptFileAttr() const;
    void SetScriptFileAttr(const ResourceRef& value);

    static void squirrel_print(HSQUIRRELVM v, const SQChar* s, ...);
    static void squirrel_error(HSQUIRRELVM v, const SQChar* s, ...);

    void DelayedStart() override;

    void Update(float timeStep) override;

private:
    SharedPtr<ScriptFile> scriptFile_;

    HSQUIRRELVM vm_;
};

} // namespace Urho3D
