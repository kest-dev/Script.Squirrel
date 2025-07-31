%module(naturalvar=1) Plugin
%{
#include "SampleComponent.h"

using namespace Urho3D;
#include <Urho3D/CSharp/Native/SWIGHelpers.h>
%}
#define PLUGIN_CORE_SAMPLEPLUGIN_API
%import "Urho3D.i"
%{
#include "SampleComponent.h"
%}
%include "SampleComponent.h"
