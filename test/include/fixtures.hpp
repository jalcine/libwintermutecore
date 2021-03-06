/*
 * Wintermute is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * Wintermute is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with Wintermute; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <wintermute-core/globals.hpp>
#include <wintermute-core/module.hpp>
#include <wintermute-core/tunnel.hpp>
#include <wintermute-core/logging.hpp>
#include <wintermute-core/message.hpp>
#include <wintermute-core/call.hpp>
#include <wintermute-core/library.hpp>
#include <wintermute-core/events.hpp>
#include "wintermute-core/event_loop.hh"
#include "wintermute-core/plugin.hpp"
#include "wintermute-core/plugin.hh"
#include <string>
#include <cxxtest/TestSuite.h>

using Wintermute::Plugin;

#define KILL_TEST_TIMEOUT 2000

// TODO: Compile a list of faux values.

// {{{ Value Traits for CxxTest
CXXTEST_ENUM_TRAITS(Wintermute::Library::LoadState,
  CXXTEST_ENUM_MEMBER(Wintermute::Library::LoadIsLoaded)
  CXXTEST_ENUM_MEMBER(Wintermute::Library::LoadNotLoaded)
  CXXTEST_ENUM_MEMBER(Wintermute::Library::LoadStateFailure)
  CXXTEST_ENUM_MEMBER(Wintermute::Library::LoadStateSuccess)
  CXXTEST_ENUM_MEMBER(Wintermute::Library::LoadUndefined)
);
// }}}

Wintermute::Plugin::Ptr wntrtest_load_plugin(const string& pluginName)
{
  Plugin::Ptr pluginPtr = nullptr;
  TSM_ASSERT (
    "Plugin " + pluginName + " has not been loaded already.",
    !Plugin::hasPlugin(pluginName)
  );

  const string libDir(TEST_BASE_DIR "/../lib");
  setenv(WINTERMUTE_ENV_PLUGIN_PATH, libDir.c_str() , 1);

  TSM_ASSERT_THROWS_NOTHING (
    "Load and obtain a pointer to the Plugin.",
    pluginPtr = Plugin::find(pluginName)
  );

  TSM_ASSERT (
    "Confirm that the plugin pointer is valid.",
    pluginPtr != nullptr && pluginPtr
  );

  TSM_ASSERT (
    "Confirm that the plugin pool is aware of " + pluginName,
    Plugin::hasPlugin(pluginName) == true
  );

  unsetenv(WINTERMUTE_ENV_PLUGIN_PATH);
  return pluginPtr;
}

void wntrtest_unload_plugin(const string& pluginName)
{
  TSM_ASSERT ( "Freed plugin",
    Plugin::release(pluginName) );
  TSM_ASSERT ( "Removed plugin.",
    !Plugin::hasPlugin(pluginName) );
}

// Generates a random designation.
Wintermute::Module::Designation craftRandomDesignation()
{
  return Wintermute::Module::Designation("foo", "bar");
}

Wintermute::Message craftRandomMessage()
{
  Wintermute::Message::HashType data;
  data.insert(std::make_pair("foo", "bar"));
  return Wintermute::Message(data, craftRandomDesignation(), craftRandomDesignation());
}

class SamplePlugin : public Wintermute::Plugin
{
public:
  SamplePlugin(const string& newName = "sample")
    : Wintermute::Plugin(newName)
  {
  }

  ~SamplePlugin()
  {
  }

  virtual bool startup() final { return true; }
  virtual bool shutdown() final {  return true; }
  virtual PluginType type() const final { return PluginType::PluginTypeService; };

  void addToPool()
  {
    Plugin::Ptr pluginPtr = shared_from_this();
    Wintermute::PluginPrivate::registerPlugin(pluginPtr);
  }

  void removeFromPool()
  {
    Wintermute::PluginPrivate::unregisterPlugin(name());
  }
};

class SampleModule : public Wintermute::Module
{
public:
  explicit SampleModule(const unsigned int index = 1) :
    Wintermute::Module(
      Wintermute::Module::Designation(
        "input",
        "test" + std::to_string(index) + ".wintermute.in"
      )
    )
  {
    winfo("SampleModule: My name is " + (string) designation() + ".");
  }

  SampleModule(const Wintermute::Module::Designation& des) :
    Module(des)
  {
    winfo("SampleModule: I'm pretending to be " + (string) des + ".");
  }

  virtual ~SampleModule() { }
};

class SampleLoop : public Wintermute::Events::Loop
{
public:
  W_DECL_PTR_TYPE(SampleLoop)
  explicit SampleLoop(const bool useDefault = true) : Loop(useDefault) { }
  inline uv_loop_t* uvLoop()
  {
    return d_func()->loop;
  }
};


class SampleDispatcher : public Wintermute::Tunnel::Dispatcher
{
public:
  explicit SampleDispatcher(const string& theName = "sample") : Wintermute::Tunnel::Dispatcher(theName) { }
  virtual ~SampleDispatcher() { }

  virtual bool send (const Wintermute::Message& sendingMessage)
  {
    message = sendingMessage;
    return message == sendingMessage;
  }

  virtual void start() { }
  virtual void stop() { }

  Wintermute::Message message;
};

class SampleReceiver : public Wintermute::Tunnel::Receiver
{
public:
  explicit SampleReceiver(const Wintermute::Message& storedMessage, const string& theName = "sample")
    : Wintermute::Tunnel::Receiver(theName), message(storedMessage) { }

  virtual ~SampleReceiver() { }

  virtual void receive (const Wintermute::Message& incomingMessage)
  {
    Wintermute::Tunnel::MessageEvent::Ptr msgPtr =
      make_shared<Wintermute::Tunnel::MessageEvent>(incomingMessage);
    msgPtr->direction = Wintermute::Tunnel::MessageEvent::DirectionIncoming;
    emitEvent(msgPtr);
  }

  virtual void start() { }
  virtual void stop() { }

  Wintermute::Message message;
};

class SampleConsumingDispatcher : public Wintermute::Tunnel::Dispatcher
{
public:
  explicit SampleConsumingDispatcher(const string& theName = "sample") : Wintermute::Tunnel::Dispatcher(theName) { }
  virtual ~SampleConsumingDispatcher() { }

  virtual bool send (const Wintermute::Message& sendingMessage)
  {
    receiver->message = sendingMessage;
    return true;
  }

  SharedPtr<SampleReceiver> receiver;
};

class SampleVoidCall : public Wintermute::Call
{
public:
  explicit SampleVoidCall() : Wintermute::Call("sampleVoid")
  {
    Call::Function func = [ = ](const string & arguments) -> const string
    {
      winfo(arguments);
      return string();
    };

    bindFunction(func);
  }
  virtual ~SampleVoidCall() { }
};

class SampleCallWithValue : public Wintermute::Call
{
private:
  string val;
public:
  // TODO Fix this shit. Keep getting '-Wwrite-strings'.
  static const string DefaultValue;

  explicit SampleCallWithValue(const string& value = DefaultValue) :
    Wintermute::Call("samplevalue")
  {
    Call::Function func = [&value](const string & arguments) -> const string
    {
      winfo(arguments);
      return value;
    };

    bindFunction(func);
  }
  virtual ~SampleCallWithValue() { }
};

class SampleVoidModuleCall : public Wintermute::Module::Call
{
public:
  explicit SampleVoidModuleCall(const Wintermute::Module::Ptr& modulePtr) :
    Wintermute::Module::Call(modulePtr, "sampleVoid")
  {
    Call::Function func = [](const string & arguments) -> const string
    {
      winfo(arguments);
      return string();
    };

    bindFunction(func);
  }
  virtual ~SampleVoidModuleCall() { }
};

class SampleModuleCallWithValue : public Wintermute::Module::Call
{
public:
  explicit SampleModuleCallWithValue(const Wintermute::Module::Ptr& modulePtr,
      const string& value = string(SampleCallWithValue::DefaultValue)) :
    Wintermute::Module::Call(modulePtr, "sampleWithValues")
  {
    Call::Function func = [ &value ](const string & arguments) -> const string
    {
      winfo(arguments);
      return value;
    };

    bindFunction(func);
  }
  virtual ~SampleModuleCallWithValue() { }
};

class SampleMimicModuleCall : public Wintermute::Module::Call
{
public:
  explicit SampleMimicModuleCall(const Wintermute::Module::Ptr& modulePtr) :
    Wintermute::Module::Call(modulePtr, "sampleMimic")
  {
    Call::Function func = [](const string & arguments) -> const string
    {
      assert(arguments == arguments);
      return arguments;
    };

    bindFunction(func);
  }
  virtual ~SampleMimicModuleCall() { }
};

class SampleEvent : public Wintermute::Events::Event
{
public:
  explicit SampleEvent(const string& eventName) : Event(eventName) { }
};

const string SampleCallWithValue::DefaultValue = string("sample");
