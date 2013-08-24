/**
 * vim: ft=qt.cpp
 * Copyright (C) 2013 Jacky Alcine <me@jalcine.me>
 *
 * This file is part of Wintermute, the extensible AI platform.
 *
 *
 * Wintermute is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Wintermute is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Wintermute.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "Wintermute/Procedure/call.hpp"
#include "Wintermute/Procedure/heart_beat_call.hpp"
#include "Wintermute/Procedure/module.hpp"
#include "Wintermute/logging.hpp"
#include <QtCore/QMap>
#include <QtZeroMQ/context.hpp>
#include <QtZeroMQ/socket.hpp>

using QtZeroMQ::Socket;
using QtZeroMQ::Context;

namespace Wintermute {
  class ApplicationPrivate;
  namespace Procedure {
    class Module;
    class ModulePrivate {
      Q_DECLARE_PUBLIC(Module);

      public:
      Module* q_ptr;
      QString package;
      QString domain;
      Socket* socketIn;
      Socket* socketOut;
      QMap<QString, CallPointer> calls;
      static Context* context;

      ModulePrivate (Module* q) :
        q_ptr(q), package(""), domain(""), calls() {
      }

      void sendHeartbeat() {
        Q_Q(Module);
        q->dispatch(new HeartbeatCall(q));
      }

      void connectToWire() {
        socketIn = ModulePrivate::context->createSocket(Socket::TYP_SUB);
        socketIn->bindTo("tcp://0.0.0.0:3991");

        socketOut = ModulePrivate::context->createSocket(Socket::TYP_PUB);
        socketOut->connectTo("tcp://0.0.0.0:3991");
      }

      void disconnectFromWire() {
        socketIn->deleteLater();
        delete socketIn;

        socketOut->deleteLater();
        delete socketOut;
      }

      void sendData(const QString& data) {
        socketOut->sendMessage(data.toLocal8Bit());
      }

      void recieveDataAsync(std::function<void (QVariant)> callback){
        // TODO: Recieve data in async.
      }

      QVariant receiveData(){
        QList<QByteArray> bytes = socketIn->receiveFullMessage();
        return QVariant();
      }

      virtual ~ModulePrivate () {
        disconnectFromWire();
        winfo(q_ptr, "We out!");
      }
    };
  } /* Procedure */
} /* Wintermute  */