#pragma once

#include <QWidget>

namespace Client {

class BroadcastTableModel;

namespace Ui {
class ClientWidget;
}

class ClientWidget final : public QWidget {
  Q_OBJECT

 public:
  explicit ClientWidget(QWidget* parent = nullptr);
  ~ClientWidget() override;

 private:
  Ui::ClientWidget* ui;

  BroadcastTableModel* broadcastModel{nullptr};
};

}  // namespace Client
