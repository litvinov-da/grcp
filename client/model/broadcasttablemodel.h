#pragma once

#include "broadcasttableitem.h"

#include <QAbstractTableModel>

namespace BroadcastUtil {
class BroadcastListener;
}
namespace Client {

class BroadcastTableModel final : public QAbstractTableModel {
  Q_OBJECT

 public:
  enum Columns {
    Ip_Column = 0,
    LastPingTime_Column,
    Status_Column,
    AvailableAction_Column,
    SizeInternal_Column
  };

  enum class AvailableAction : std::uint8_t { Connect = 0, Disconnect };

  static QString availableActionToString(AvailableAction action);

  explicit BroadcastTableModel(QObject* parent = nullptr);

  int rowCount(const QModelIndex& parent = {}) const override;
  int columnCount(const QModelIndex& parent = {}) const override;
  QVariant data(const QModelIndex& index, int role) const override;
  bool setData(const QModelIndex& index, const QVariant& value,
               int role) override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role) const override;
  bool insertRows(int row, int count, const QModelIndex& parent) override;

 private:
  QString getHeaderDataDisplayRole(int column) const;
  QString getHeaderDataToolTipRole(int column) const;

  QString getDisplayRole(const QModelIndex& index) const;
  QVariant getTextAlignmentRole(const QModelIndex& index) const;

  void addItem(const QHostAddress& serverIp, quint16 serverPort);

 private:
  std::vector<BroadcastTableItem> items {};

  BroadcastUtil::BroadcastListener* broadcastListener{nullptr};
};

}  // namespace Client
