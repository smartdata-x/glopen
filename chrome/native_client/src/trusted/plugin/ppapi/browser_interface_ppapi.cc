/*
 * Copyright 2010 The Native Client Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can
 * be found in the LICENSE file.
 */

#include "native_client/src/trusted/plugin/ppapi/browser_interface_ppapi.h"

#include <string>

#include "native_client/src/include/nacl_elf.h"
#include "native_client/src/include/nacl_macros.h"
#include "native_client/src/include/portability.h"
#include "native_client/src/trusted/plugin/api_defines.h"
#include "native_client/src/trusted/plugin/ppapi/scriptable_handle_ppapi.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/var.h"

// TODO(polina,sehr): InstanceIdentifier should be typedefed to pp::Instance*.
// for v2 plugins.

namespace {

bool GetWindow(plugin::InstanceIdentifier instance_id, pp::Var* window) {
  pp::Instance* instance = reinterpret_cast<pp::Instance*>(instance_id);
  *window = instance->GetWindowObject();
  return window->is_void();
}

}  // namespace

namespace plugin {

uintptr_t BrowserInterfacePpapi::StringToIdentifier(const nacl::string& str) {
  return reinterpret_cast<uintptr_t>(new pp::Var(str));
}


nacl::string BrowserInterfacePpapi::IdentifierToString(uintptr_t ident) {
  pp::Var* var = reinterpret_cast<pp::Var*>(ident);
  return var->AsString();
}


bool BrowserInterfacePpapi::Alert(InstanceIdentifier instance_id,
                                  const nacl::string& text) {
  pp::Var window;
  if (!GetWindow(instance_id, &window)) {
    return false;
  }
  pp::Var exception;
  window.Call("alert", text, &exception);
  return exception.is_void();
}


bool BrowserInterfacePpapi::EvalString(InstanceIdentifier instance_id,
                                       const nacl::string& expression) {
  pp::Var window;
  if (!GetWindow(instance_id, &window)) {
    return false;
  }
  pp::Var exception;
  window.Call("eval", expression, &exception);
  return exception.is_void();
}


bool BrowserInterfacePpapi::GetOrigin(InstanceIdentifier instance_id,
                                      nacl::string* origin) {
  const nacl::string kUnknownOrigin = "";
  *origin = kUnknownOrigin;
  pp::Var window;
  if (!GetWindow(instance_id, &window)) {
    return false;
  }
  pp::Var location = window.GetProperty("location");
  if (location.is_string()) {
    *origin = location.AsString();
  } else {
    pp::Var href = window.GetProperty("href");
    if (!href.is_string()) {
      *origin = href.AsString();
    }
  }
  return (kUnknownOrigin != *origin);
}


ScriptableHandle* BrowserInterfacePpapi::NewScriptableHandle(
    PortableHandle* handle) {
  UNREFERENCED_PARAMETER(handle);
  NACL_UNIMPLEMENTED();
  return ScriptableHandlePpapi::New(handle);
}

}  // namespace plugin
