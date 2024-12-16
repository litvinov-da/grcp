#include "clientwidget.h"
#include "model/broadcasttablemodel.h"
#include "ui_clientwidget.h"

#include "delegate/broadcasttabledelegate.h"

namespace Client {

ClientWidget::ClientWidget(QWidget* parent)
    : QWidget(parent),
      ui(new Ui::ClientWidget),
      broadcastModel(new BroadcastTableModel(this)) {
  ui->setupUi(this);

  auto* delegate = new BroadcastTableDelegate(this);
  connect(delegate, &BroadcastTableDelegate::actionClicked, broadcastModel,
          &BroadcastTableModel::triggerAction);

  ui->broadcastTable->setItemDelegate(delegate);
  ui->broadcastTable->setModel(broadcastModel);
}

ClientWidget::~ClientWidget() {
  delete ui;
}

}  // namespace Client
