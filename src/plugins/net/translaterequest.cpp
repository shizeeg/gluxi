/***************************************************************************
 *   Copyright (C) 2008 by Sidgyck                                         *
 *   sidgyck@gmail.com                                                     *
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
#include "translaterequest.h"
#include "base/baseplugin.h"
#include "base/common.h"
#include "base/messageparser.h"

#include <QHttp>
#include <QUrl>
#include <QRegExp>

#include <QTextCodec>
#include <QDate>
#include <QTime>

#include <QtDebug>

#define TRANSLATE_RU "http://m.translate.ru/translator/result/?dirCode=%1&text=%2"

TranslateRequest::TranslateRequest(BasePlugin *plugin, gloox::Stanza *from, MessageParser& parser)
	:AsyncRequest(-1, plugin, from, 300)
{
	myType = parser.nextToken().toLower();
	if (myType == "gtr")
	{
		myFrom	= parser.nextToken();
		myTo = parser.nextToken();
		myText = parser.joinBody();

	}
	else if (myType == "ya")
	{
		myFrom	= parser.nextToken();
		myText = parser.joinBody();
	}
	http=0;
}

TranslateRequest::~TranslateRequest()
{
	if (http)
		delete http;
	http=0;
}

void TranslateRequest::exec()
{
	if ( myType == "gtr" )
	{
		if( myFrom.toLower().startsWith("list") ) /// FIXIT: Implement parsing list form html.
		{
			QString url = QString("http://translate.google.com/translate_t");

			QUrl qurl(url);
			qDebug() << qurl.toEncoded();

			http=new QHttp(qurl.host());
			connect(http,SIGNAL(requestFinished(int, bool)), this, SLOT(httpRequestListFinished(int, bool)));
			http->get(qurl.toEncoded());
			return;
		}
		if( myFrom.isEmpty() || myTo.isEmpty() || myText.isEmpty() )
		{
			plugin()->reply(stanza(),"Usage: net transtlate gtr <from> <to> <text>");
			deleteLater();
			return;
		}
		QString url = QString("http://translate.google.com/translate_t?langpair=%1|%2&ie=UTF8&oe=UTF8&text=%3")
			.arg(myFrom).arg(myTo).arg(myText);

		QUrl qurl(url);
		qDebug() << qurl.toEncoded();

		http=new QHttp(qurl.host());
		connect(http,SIGNAL(requestFinished(int, bool)), this, SLOT(httpRequestFinished(int, bool)));
		http->get(qurl.toEncoded());
	}
	else if ( myType == "ya" )
	{
		if( myFrom.toLower().startsWith("list") ) /// FIXIT: Implement parsing list form html.
		{
			QString url = QString("http://translate.google.com/translate_t");

			QUrl qurl(url);
			qDebug() << qurl.toEncoded();

			http=new QHttp(qurl.host());
			connect(http,SIGNAL(requestFinished(int, bool)), this, SLOT(httpRequestListYandexFinished(int, bool)));
			http->get(qurl.toEncoded());
			return;
		}
		if( myFrom.isEmpty() || myText.isEmpty() )
		{
			plugin()->reply(stanza(),"Usage: net transtlate ya <from> <text>");
			deleteLater();
			return;
		}
		QString url = QString("http://lingvo.yandex.ru/%1?text=%3&lang=%1&search_type=%2&st_translate=on")
			.arg(myFrom).arg("lingvo").arg(myText);

		QUrl qurl(url);
		qDebug() << qurl.toEncoded();

		http=new QHttp(qurl.host());
		connect(http,SIGNAL(requestFinished(int, bool)), this, SLOT(httpRequestYandexFinished(int, bool)));
		http->get(qurl.toEncoded());

	}
	else
	{
		plugin()->reply(stanza(),"Usage: net transtlate <gtr | ya> <from> [to] <text>");
		deleteLater();
		return;
	}

}

void TranslateRequest::httpRequestFinished(int, bool err)
{
	if (err || http->lastResponse().statusCode()!=200)
	{
		plugin()->reply(stanza(),"Failed to translate: "+http->lastResponse().reasonPhrase());
		deleteLater();
		return;
	}
	QString buf = http->readAll();

	plugin()->reply(stanza(), getValue(buf,
            "onmouseout=\"this.style.backgroundColor='#fff'\">(.*)</span>").trimmed() );
	deleteLater();
}

void TranslateRequest::httpRequestListFinished(int, bool err)
{
  ///FIXIT: implement fetch list from the web.

	QStringList dlist;
	dlist		  << "auto	Detect language"
			  << "ar	Arabic"
			  << "bg	Bulgarian"
			  << "ca	Catalan"
			  << "zh-CN	Chinese"
			  << "hr	Croatian"
			  << "cs	Czech"
			  << "da	Danish"
			  << "nl	Dutch"
			  << "en	English"
			  << "tl	Filipino"
			  << "fi	Finnish"
			  << "fr	French"
			  << "de	German"
			  << "el	Greek"
			  << "iw	Hebrew"
			  << "hi	Hindi"
			  << "id	Indonesian"
			  << "it	Italian"
			  << "ja	Japanese"
			  << "ko	Korean"
			  << "lv	Latvian"
			  << "lt 	Lithuanian"
			  << "no 	Norwegian"
			  << "pl	Polish"
			  << "pt 	Portuguese"
			  << "ro	Romanian"
			  << "ru	Russian"
			  << "sr	Serbian"
			  << "sk	Slovak"
			  << "sl	Slovenian"
			  << "es 	Spanish"
			  << "sv	Swedish"
			  << "uk	Ukrainian"
			  << "vi	Vietnamese";

	plugin()->reply(stanza(), "Possible directions: \n" + dlist.join("\n"));
	deleteLater();
}


void TranslateRequest::httpRequestYandexFinished(int, bool err)
{
	if (err || http->lastResponse().statusCode()!=200)
	{
		plugin()->reply(stanza(),"Failed to translate: "+http->lastResponse().reasonPhrase());
		deleteLater();
		return;
	}
	QString buf = http->readAll();
	QString body = getValue(buf,"</span><div>(.*)</div></li>");

	plugin()->reply(stanza(), (body.isEmpty()) ? "<not found>":body );
	deleteLater();
}

void TranslateRequest::httpRequestListYandexFinished(int, bool err)
{
	QStringList dlist;
		dlist << "en	Englist	<-> Russian";
		dlist << "de	Deutch	<-> Russian";
		dlist << "fr	French	<-> Russian";
		dlist << "it	Italian	<-> Russian";
		dlist << "es	Spanish	<-> Russian";
		dlist << "uk	Ukraine	<-> Russian";
		dlist << "la	Latin	<-> Russian";

	plugin()->reply(stanza(), "Possible directions: \n" + dlist.join("\n"));
	deleteLater();
}

