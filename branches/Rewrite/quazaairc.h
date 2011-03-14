/*
** quazaairc.h
**
** Copyright  Quazaa Development Team, 2009-2011.
** This file is part of QUAZAA (quazaa.sourceforge.net)
**
** Quazaa is free software; this file may be used under the terms of the GNU
** General Public License version 3.0 or later as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Quazaa is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
**
** Please review the following information to ensure the GNU General Public
** License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** You should have received a copy of the GNU General Public License version
** 3.0 along with Quazaa; if not, write to the Free Software Foundation,
** Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef QUAZAAIRC_H
#define QUAZAAIRC_H

#include <QtGui>

namespace Irc
{
	class Session;
	class Buffer;
}

namespace IrcEvent {
	enum IrcEvent { Command, Status, Notice, Message, Action, Server };
}

class QuazaaIRC : public QObject
{
	Q_OBJECT
	Q_ENUMS(Event)

protected:
	bool m_connected;


public:
	QuazaaIRC(QObject* parent = 0);
	QString sServer;
	QString sNick;
	QString sRealName;
	bool bLoginCompleted;

public slots:
	void startIrc();
	void stopIrc();
	void sendIrcMessage(QString channel, QString message);
	void addRoom(QString room);
	void removeRoom(QString room);

signals:
	void ircBufferAdded(Irc::Buffer* buffer);
	void ircBufferRemoved(Irc::Buffer* buffer);
	void joined(QString chan);

protected slots:
	void on_IrcSession_connected();
	void on_IrcSession_disconnected();
	void on_IrcSession_welcomed();
	void on_IrcSession_bufferAdded(Irc::Buffer* buffer);
	void on_IrcSession_bufferRemoved(Irc::Buffer* buffer);
	void ctcpReply(QString nick, QString reply);
	void numericMessageReceived(QString sender, uint code, QStringList list);

protected:
	Irc::Session *ircSession;
};

#endif //QUAZAAIRC_H
