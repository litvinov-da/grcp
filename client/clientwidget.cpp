#include "clientwidget.h"
#include "model/broadcasttablemodel.h"
#include "ui_clientwidget.h"

namespace Client {

ClientWidget::ClientWidget(QWidget* parent)
    : QWidget(parent),
      ui(new Ui::ClientWidget),
      broadcastModel(new BroadcastTableModel(this)) {
  ui->setupUi(this);

  ui->broadcastTable->setModel(broadcastModel);
}

ClientWidget::~ClientWidget() {
  delete ui;
}

}  // namespace Client
