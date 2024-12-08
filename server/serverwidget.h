#pragma once

#include <QWidget>

#include "server.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class ServerWidget;
};
QT_END_NAMESPACE

namespace Server {
class ServerWidget : public QWidget {
  Q_OBJECT

 public:
  explicit ServerWidget(QWidget* parent = nullptr);
  ~ServerWidget();

 private:
  void startServer();

 private:
  Ui::ServerWidget* ui;

  Server* server{nullptr};
};
}  // namespace Server
