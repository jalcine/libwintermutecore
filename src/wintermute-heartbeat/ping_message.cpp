/*
   Wintermute - A foundation for intelligent computing.
   Copyright (c) 2010 - 2015 by Jacky Alcine

   Wintermute is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 3 of the License, or (at your option) any later version.

   Wintermute is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with Wintermute; if not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
   */

#include "messages.hpp"
#include <wintermute-core/module.hpp>
#include <wintermute-core/plugin.hpp>
#include <wintermute-core/logging.hpp>
#include <wintermute-core/util.hh>
#include <algorithm>

using Wintermute::Message;
using Wintermute::Plugin;
using Wintermute::Heartbeat::PingMessage;
using Wintermute::Util::join_string;
using std::for_each;

PingMessage::PingMessage() : Message()
{
}

PingMessage::~PingMessage()
{
}

Message PingMessage::craft()
{
  Message msg;
  Message::HashType msgData;
  list<string> pluginNames, moduleNames;
  const auto pluginList = Plugin::all();
  const auto moduleList = Module::Pool::instance()->modules();

  if (!pluginList.empty())
  {
    for ( auto pluginName : pluginList )
    {
      pluginNames.push_back(pluginName);
    }
  }

  if (!moduleList.empty())
  {
    for_each ( begin(moduleList), end(moduleList),
      [&moduleNames](const Wintermute::Module::Ptr& modulePtr)
    {
      auto des = modulePtr->designation();
      string name = des.domain() + "." + des.name();
      moduleNames.push_back(name);
    });
  }

  const auto pluginNamesStr = join_string(pluginNames, ",");
  const auto moduleNamesStr = join_string(moduleNames, ",");

  msgData.emplace("modules", moduleNamesStr);
  msgData.emplace("plugins", pluginNamesStr);
  msg.setPayload(msgData);
  msg.setReceiver(Module::Designation(
    WINTERMUTE_HEARTBEAT_MODULE_PONGER,
    WINTERMUTE_HEARTBEAT_DOMAIN
  ));
  return msg;
}
