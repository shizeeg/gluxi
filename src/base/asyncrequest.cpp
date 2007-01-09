#include "asyncrequest.h"

#include <gloox/stanza.h>

#include <QtDebug>

AsyncRequest::AsyncRequest(int id, BasePlugin *plugin, gloox::Stanza *from, int timeout)
{
	myId=id;
	myPlugin=plugin;
	myStanza=from;
	myTimeout=timeout;
	update();
}

AsyncRequest::~AsyncRequest()
{
	qDebug() << "~AsyncRequest";
	if (myStanza)
		delete myStanza;
	emit onDelete(this);
}

void AsyncRequest::update()
{
	myTime=QDateTime::currentDateTime();
	notified=false;
}

bool AsyncRequest::expired()
{
	bool res=myTime.secsTo(QDateTime::currentDateTime())>myTimeout;
	if (res && !notified)
	{
		emit onExpire();
		notified=true;
	}
	return res;
}

QString AsyncRequest::stanzaId() const
{
        if (!myStanza)
                return QString::null;
        return QString::fromStdString(myStanza->findAttribute("id"));
}

void AsyncRequest::run()
{
}	

void AsyncRequest::wantDelete()
{
	emit onWantDelete(this);
}

