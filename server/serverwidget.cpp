#include "serverwidget.h"

#include <spdlog/sinks/qt_sinks.h>
#include "ui_serverwidget.h"
#include <spdlog/async.h>

namespace Server {

ServerWidget::ServerWidget(QWidget* parent)
    : QWidget(parent), ui(new Ui::ServerWidget()) {
  ui->setupUi(this);

  server = new Server(spdlog::qt_logger_mt(
                          "textedit_logger", ui->logEdit),
                      this);

  connect(ui->startButton, &QPushButton::clicked, this,
          &ServerWidget::startServer);

  connect(ui->portEdit, &QLineEdit::textChanged, this,
          [this](const QString& text) {
            // validation of text being number
            // put on input mask of portEdit
            server->setPort(text.toUInt());
          });
}

ServerWidget::~ServerWidget() {
  delete ui;
}

void ServerWidget::startServer() {
  auto isSuccessful = server->start();
  ui->startButton->setEnabled(!isSuccessful);
}
}  // namespace Server
