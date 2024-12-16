#pragma once

#include "broadcasttableitem.h"
#include "model/broadcasttablecommon.h"
#include "pingsender.h"

#include <QAbstractTableModel>

namespace BroadcastUtil {
class BroadcastListener;
}
namespace Client {

class BroadcastTableModel final : public QAbstractTableModel {
  Q_OBJECT

 public:
  static QString availableActionToString(AvailableAction action);

  explicit BroadcastTableModel(QObject* parent = nullptr);

  int rowCount(const QModelIndex& parent = {}) const override;
  int columnCount(const QModelIndex& parent = {}) const override;
  QVariant data(const QModelIndex& index, int role) const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role) const override;

  void triggerAction(const QModelIndex& index);

  void updateHasPing(const NetworkUtil::NetworkNodeInfo& serverInfo);
  void updateDisconnect(const NetworkUtil::NetworkNodeInfo& serverInfo);

 private:
  QString getHeaderDataDisplayRole(int column) const;
  QString getHeaderDataToolTipRole(int column) const;

  QString getDisplayRole(const QModelIndex& index) const;
  QVariant getTextAlignmentRole(const QModelIndex& index) const;
  QVariant getForegroundRole(const QModelIndex& index) const;

  void addItem(const QHostAddress& serverIp, quint16 serverPort);

 private:
  std::vector<BroadcastTableItem> items{};

  BroadcastUtil::BroadcastListener* broadcastListener{nullptr};

  std::unordered_map<NetworkUtil::NetworkNodeInfo, std::shared_ptr<PingSender>>
      pingSenders;
};

}  // namespace Client
