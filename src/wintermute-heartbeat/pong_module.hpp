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

#ifndef WINTERMUTE_HEARTBEAT_PONG_MODULE_HPP_
# define WINTERMUTE_HEARTBEAT_PONG_MODULE_HPP_

#include "globals.hpp"
#include <wintermute-core/module.hpp>

namespace Wintermute
{
namespace Heartbeat
{
class PongModule : public Wintermute::Module
{
  Events::Timer::Ptr timer;
  void onTimerElasped(const Events::Event::Ptr& );

  public:
    W_DECL_PTR_TYPE(PongModule)
    explicit PongModule();
    ~PongModule() override;
    string uuid;
    void regenerateUuid();

  protected:
    virtual bool sendMessage(const Message& message);
    virtual bool receiveMessage(const Message& message) const;
};
}
}

#endif
