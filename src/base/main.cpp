#include "gluxibot.h"
#include "segfault.h"

#include <QCoreApplication>
#include <QLocale>
#include <QTextCodec>

#ifndef Q_WS_WIN
#include <sys/types.h>
#include <signal.h>
#endif

GluxiBot *bot;

#ifndef Q_WS_WIN
void termHandler(int)
{
	bot->onQuit("got SIGTERM signal");
}

void quitHandler(int)
{
	bot->onQuit("got SIGINT signal. Possible Ctrl+C from console");
}

void installSigHandlers()
{
        struct sigaction act;
        memset(&act,0,sizeof(act));
        act.sa_handler=termHandler;
        sigaction(SIGTERM, &act, NULL);
	act.sa_handler=quitHandler;
	sigaction(SIGINT, &act, NULL);
}
#endif

int main(int argc, char*argv[])
{
	initSegFaultHandler();
	setenv("LANG","en_US.UTF-8",1);
	QCoreApplication app(argc, argv);
	QLocale::setDefault(QLocale("en_US"));
	QTextCodec::setCodecForCStrings (QTextCodec::codecForName("UTF-8"));
	installSigHandlers();
	bot=new GluxiBot();
	int res=app.exec();
	delete bot;
	return res;
}

