/**
 * vim: ft=cpp tw=78
 * Copyright (C) 2014 Jacky Alciné <me@jalcine.me>
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

#include <Wintermute/Globals>
#include <QtCore/QVariantMap>

namespace Wintermute {
  namespace Procedure {
    /**
     * @class Wintermute::Procedure::MessagePrivate
     * @brief Data class for `Wintermute::Procedure::Message`.
     *
     * Holds the underlying aspect of the data class for the `Message` class.
     * This allows for the class to be implicitly shared.
     */
    class MessagePrivate : public QSharedData {
      public:
        MessagePrivate() : senderMap(), receiverMap(), dataMap() { }
        MessagePrivate( const MessagePrivate& other ) : 
          QSharedData ( other ), senderMap ( other.senderMap ), 
          receiverMap ( other.receiverMap ), dataMap ( other.dataMap ) { }
        virtual ~MessagePrivate() { }

        // TODO: Check integrity of data.
        bool isValid() { return true; }
        bool isValidSenderData () { return true; }
        bool isValidReceiverData () { return true; }

      private:
        QVariantMap senderMap;
        QVariantMap receiverMap;
        QVariantMap dataMap;
    };
  }
}