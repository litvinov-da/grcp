#include "clientwidget.h"

#include <QApplication>
#include <iostream>

int main(int argc, char* argv[])
{
  QApplication app{argc, argv};

  Client::ClientWidget clientWidget{};
  clientWidget.show();

  return app.exec();
}
