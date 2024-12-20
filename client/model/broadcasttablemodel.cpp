#include "broadcasttablemodel.h"

#include <broadcastlistener.h>
#include <QColor>

namespace Client {

namespace {
AvailableAction toAvailableAction(BroadcastTableItem::Status status) {
  switch (status) {
    case BroadcastTableItem::Status::Offline:
      return AvailableAction::Connect;
    case BroadcastTableItem::Status::Online:
      return AvailableAction::Disconnect;
  }

  assert(false);

  // return something, does not really matter what exactly
  return AvailableAction::Connect;
}
}  // namespace

QString BroadcastTableModel::availableActionToString(AvailableAction action) {
  switch (action) {
    case AvailableAction::Connect:
      return "Connect";
    case AvailableAction::Disconnect:
      return "Disconnect";
  }

  assert(false && "Not all type handled");
  return {};
}

BroadcastTableModel::BroadcastTableModel(QObject* parent)
    : QAbstractTableModel{parent},
      broadcastListener(new BroadcastUtil::BroadcastListener(this)) {
  connect(broadcastListener,
          &BroadcastUtil::BroadcastListener::newBroadcastServerDetected, this,
          &BroadcastTableModel::addItem);
}

}  // namespace Client

int Client::BroadcastTableModel::rowCount(const QModelIndex& /*parent*/) const {
  return items.size();
}

int Client::BroadcastTableModel::columnCount(
    const QModelIndex& /*parent*/) const {
  return SizeInternal_Column;
}

QVariant Client::BroadcastTableModel::data(const QModelIndex& index,
                                           int role) const {
  if (!index.isValid()) {
    return {};
  }

  if (role == Qt::DisplayRole) {
    return getDisplayRole(index);
  }

  // TODO(d.litvinov) add Qt::ToolTipRole

  if (role == Qt::TextAlignmentRole) {
    return getTextAlignmentRole(index);
  }

  if (role == Qt::ForegroundRole) {
    return getForegroundRole(index);
  }

  return {};
}

QVariant Client::BroadcastTableModel::headerData(int section,
                                                 Qt::Orientation orientation,
                                                 int role) const {
  if (orientation != Qt::Horizontal) {
    return {};
  }

  if (section >= SizeInternal_Column) {
    // TODO(d.litvinov) think how to handle
    return {};
  }

  if (role == Qt::DisplayRole) {
    return getHeaderDataDisplayRole(section);
  } else if (role == Qt::ToolTipRole) {
    return getHeaderDataToolTipRole(section);
  } else {
    return {};
  }
}

void Client::BroadcastTableModel::triggerAction(const QModelIndex& index) {
  assert(index.isValid());
  assert(index.column() == AvailableAction_Column);

  auto& item = items[index.row()];
  const auto action = toAvailableAction(item.status);
  if (pingSenders.contains(item.ipInfo)) {
    switch (action) {
      case AvailableAction::Connect: {
        pingSenders[item.ipInfo]->start();
      } break;
      case AvailableAction::Disconnect: {
        pingSenders[item.ipInfo]->stop();
        item.status = BroadcastTableItem::Status::Offline;
        emit dataChanged(this->index(index.row(), 0),
                         this->index(index.row(), columnCount()));
      } break;
    }
  } else {
    assert(action == AvailableAction::Connect);

    auto pingSender = std::make_shared<PingSender>(item.ipInfo);
    connect(pingSender.get(), &PingSender::hasPing, this,
            &BroadcastTableModel::updateHasPing);
    connect(pingSender.get(), &PingSender::disconnect, this,
            &BroadcastTableModel::updateDisconnect);

    pingSenders.insert({item.ipInfo, pingSender});
    pingSender->start();
  }
}

void Client::BroadcastTableModel::updateHasPing(
    const NetworkUtil::NetworkNodeInfo& serverInfo) {
  auto item =
      std::find_if(items.begin(), items.end(),
                   [serverInfo](const BroadcastTableItem& item) -> bool {
                     return item.ipInfo == serverInfo;
                   });

  assert(item != items.end());

  item->lastPingTime = QDateTime::currentDateTime();
  item->status = BroadcastTableItem::Status::Online;

  const auto row = std::distance(items.begin(), item);
  emit dataChanged(index(row, 0), index(row, columnCount()));
}

void Client::BroadcastTableModel::updateDisconnect(
    const NetworkUtil::NetworkNodeInfo& serverInfo) {
  auto item =
      std::find_if(items.begin(), items.end(),
                   [serverInfo](const BroadcastTableItem& item) -> bool {
                     return item.ipInfo == serverInfo;
                   });

  assert(item != items.end());

  item->status = BroadcastTableItem::Status::Offline;

  auto pingSender = pingSenders.find(serverInfo);
  assert(pingSender != pingSenders.end());

  pingSender->second->stop();

  const auto row = std::distance(items.begin(), item);
  emit dataChanged(index(row, 0), index(row, columnCount()));
}

QString Client::BroadcastTableModel::getHeaderDataDisplayRole(
    int column) const {
  assert(column < SizeInternal_Column);

  const auto modelColumn = static_cast<Columns>(column);
  switch (modelColumn) {
    case Ip_Column:
      return "IP";
    case LastPingTime_Column:
      return "Last ping time";
    case Status_Column:
      return "Status";
    case AvailableAction_Column:
      return "Action";
    case SizeInternal_Column:
      assert(false && "Should not use internal type");
      return "";
  }

  assert(false && "Not all columns handled");
  return {};
}

QString Client::BroadcastTableModel::getHeaderDataToolTipRole(
    int column) const {
  assert(column < SizeInternal_Column);

  const auto modelColumn = static_cast<Columns>(column);

  switch (modelColumn) {
    case Ip_Column:
      return "IP address and port for communication with detected server";
    case LastPingTime_Column:
      return "Last time when server was detected";
    case Status_Column:
      return "Current status of server connection";
    case AvailableAction_Column:
      return "Available action";
    case SizeInternal_Column:
      assert(false && "Should not use internal type");
      return {};
  }

  assert(false && "Not all columns handled");
  return {};
}

QString Client::BroadcastTableModel::getDisplayRole(
    const QModelIndex& index) const {
  assert(index.isValid());
  assert(index.column() < SizeInternal_Column);
  assert(index.row() < items.size());

  const auto& item = items[index.row()];

  const auto column = static_cast<Columns>(index.column());
  switch (column) {
    case Ip_Column: {
      static constinit const QStringView ipDisplayRoleTemplate{u"%1:%2"};
      return ipDisplayRoleTemplate.arg(item.ipInfo.ip.toString(),
                                       QString::number(item.ipInfo.port));
    }
    case LastPingTime_Column: {
      static constinit const QStringView timeMask{u"HH:mm"};
      static constinit const QStringView nullTime{u"-"};

      if (item.lastPingTime.isNull()) {
        return nullTime.toString();
      }

      return item.lastPingTime.time().toString(timeMask);
    }
    case Status_Column: {
      return broadcastStatusToString(item.status);
    }
    case AvailableAction_Column: {
      return availableActionToString(toAvailableAction(item.status));
    }
    case SizeInternal_Column: {
      assert(false && "Should not use internal type");
      return {};
    }
  }

  assert(false && "Not all columns handled");
  return {};
}

QVariant Client::BroadcastTableModel::getTextAlignmentRole(
    const QModelIndex& index) const {
  return Qt::AlignCenter;
}

QVariant Client::BroadcastTableModel::getForegroundRole(
    const QModelIndex& index) const {
  assert(index.isValid());
  assert(index.column() < SizeInternal_Column);
  assert(index.row() < items.size());

  if (index.column() != Status_Column) {
    return {};
  }

  const auto& item = items[index.row()];
  switch (item.status) {
    case BroadcastTableItem::Status::Offline:
      return QColor(Qt::red);
    case BroadcastTableItem::Status::Online:
      return QColor(Qt::green);
  }

  assert(false && "Not all handled");
  return {};
}

void Client::BroadcastTableModel::addItem(const QHostAddress& serverIp,
                                          quint16 serverPort) {
  beginInsertRows(QModelIndex{}, rowCount(), 1);
  items.emplace_back(serverIp, serverPort);
  endInsertRows();
}
