#include <QApplication>
#include <iostream>
#include "serverwidget.h"

int main(int argc, char* argv[])
{
	QApplication app{argc, argv};

	Server::ServerWidget serverWidget{};
	serverWidget.show();

	return app.exec();
}