/**
 * vim: ft=cpp tw=78
 * Copyright (C) 2011 - 2013 Jacky Alciné <me@jalcine.me>
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

#ifndef WINTERMUTE_PROCEDURE_CALL_HPP
#define WINTERMUTE_PROCEDURE_CALL_HPP

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QSharedPointer>

namespace Wintermute
{
namespace Procedure
{
class CallPrivate;
class Call : public QObject
{
  Q_OBJECT;

protected:
  Q_DISABLE_COPY ( Call );
  Q_DECLARE_PRIVATE ( Call );
  QSharedPointer<CallPrivate> d_ptr;
  Call ( CallPrivate* d );
  Q_ENUMS ( Type );

  /**
   * @fn invoke
   * @param data A list of QVariant variables.
   */
  virtual QVariant invoke ( const QVariantList& data = QVariantList() );

  friend class Module;

public:

  /**
   * @enum Type
   *
   * Defines the potential types a Call can take in the procedural system. In
   * order to keep things clean, you can add your own types only if it's over
   * 0x9000 (this was intentional).
   */
  enum Type {
    TypeUndefined    = 0x0000,  // Undefined call. Don't bother with.

    TypeResponse     = 0x0010,  // Represents a invoking call being responsed to.
    TypeDispatch     = 0x0011,  // Represents a invoked call being sent to.
    TypeSignal       = 0x0012,  // Represents a invoking signal being raised.

    TypeInvocation   = 0x0020,  // Represents a call to be invoked.
    TypeRemoteNoAuth = 0x0021,  // This call deals with a remote network with no authentication.
    TypeRemoteAuth   = 0x0022,  // This call deals with a remote network with authentication.

    TypeUser         = 0x9000   // Anything above this is available to the user.
  };
  Q_FLAGS ( Type Types );

  /**
   * @ctor
   * @fn Call
   */
  explicit Call ( QObject* parent );

  /**
   * @dtor
   * @fn ~Call
   */
  virtual ~Call ();

  /**
   * @fn name
   * @brief Obtains the name of the call.
   */
  QString name() const;

  /**
   * @fn type
   * @brief Obtains the type of call this is.
   *
   * Obtains the type of message that this exposes.
   */
  Type type() const;

  /**
   * @fn toString
   * @brief Generates a string representation of this call.
   */
  QString toString() const;

  static Call* fromString ( const QString& data );

  /**
   * @fn recipient
   * @brief Obtains the name of the module to recieve this message.
   *
   * Obtains the qualified module name that this Call is sending a message
   * to.
   */
  QString recipient() const;

  /**
   * @fn setRecipient
   * @brief Changes the recipient of this Call.
   */
  void setRecipient ( const QString moduleName );

  /**
   * @operator operator()
   * @brief Allow for functor-like capabilities for the Call object.
   */
  QVariant operator() ( const QVariantList& arguments = QVariantList() );

  static bool attemptInvocation ( const Call* call );
};

typedef QSharedPointer<Call> CallPointer;
} /* Procedure */
} /* Wintermute */

#endif /* WINTERMUTE_PROCEDURE_CALL_HPP */
