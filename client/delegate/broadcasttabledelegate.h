#pragma once

#include <QItemDelegate>

namespace Client {

class BroadcastTableDelegate final : public QItemDelegate {
  Q_OBJECT

 public:
  explicit BroadcastTableDelegate(QObject* parent = nullptr);

  void paint(QPainter* painter, const QStyleOptionViewItem& option,
             const QModelIndex& index) const override;
  bool editorEvent(QEvent* event, QAbstractItemModel* model,
                   const QStyleOptionViewItem& option,
                   const QModelIndex& index) override;

 signals:
  void actionClicked(const QModelIndex& index);
};

}  // namespace Client
