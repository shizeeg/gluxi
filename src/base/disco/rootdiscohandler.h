/***************************************************************************
 *   Copyright (C) 2008 by Dmitry Nezhevenko                               *
 *   dion@inhex.net                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef ROOTDISCOHANDLER_H_
#define ROOTDISCOHANDLER_H_

#include "discohandler.h"

#include <QList>
#include <QMap>

class GluxiBot;

class RootDiscoHandler: public DiscoHandler
{
public:
	RootDiscoHandler(GluxiBot* bot);
	virtual ~RootDiscoHandler();
	virtual gloox::Stanza* handleDiscoRequest(gloox::Stanza* s);
	DiscoHandler* rootHandler() { return rootHandler_; }
	void registerDiscoHandler(DiscoHandler* handler);
	void unregisterDiscoHandler(DiscoHandler* handler);
	void addIqHandler(const QString& service);
private:
	 GluxiBot* bot_;
	 QMap<QString, DiscoHandler*> handlersMap_;
	 DiscoHandler* rootHandler_;
};

#endif /*ROOTDISCOHANDLER_H_*/
