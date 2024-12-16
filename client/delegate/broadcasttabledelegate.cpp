#include "broadcasttabledelegate.h"

#include <QApplication>
#include <QMouseEvent>
#include <QPainter>

#include "model/broadcasttablecommon.h"

Client::BroadcastTableDelegate::BroadcastTableDelegate(QObject* parent)
    : QItemDelegate(parent) {}

void Client::BroadcastTableDelegate::paint(QPainter* painter,
                                           const QStyleOptionViewItem& option,
                                           const QModelIndex& index) const {
  if (index.column() == Columns::AvailableAction_Column) {
    QStyleOptionButton button;
    button.rect = option.rect;
    button.text = index.data().toString();
    button.state = QStyle::State_Enabled;

    QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);
  } else {
    QItemDelegate::paint(painter, option, index);
  }
}

bool Client::BroadcastTableDelegate::editorEvent(
    QEvent* event, QAbstractItemModel* model,
    const QStyleOptionViewItem& option, const QModelIndex& index) {
  if (event->type() != QEvent::MouseButtonRelease ||
      index.column() != AvailableAction_Column) {
    return QItemDelegate::editorEvent(event, model, option, index);
  }

  emit actionClicked(index);
  return true;
}
