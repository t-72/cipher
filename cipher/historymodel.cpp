#include "historymodel.h"

#include <QDebug>

HistoryModel::HistoryModel(QObject *parent)
  : QAbstractListModel(parent)
  , log_(this)
{ connect_(); }

QVariant HistoryModel::data(const QModelIndex &index, int role) const
{
  QVariant result;
  if (!index.isValid() || (role != Qt::DisplayRole))
    return result;
  result = log_[log_.size() - index.row() - 1];
  return result;
}

Qt::ItemFlags HistoryModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return Qt::NoItemFlags;
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

int HistoryModel::rowCount(const QModelIndex&) const
{
  return log_.size();
}

void HistoryModel::setDir(const QString &dir)
{
  log_.setDir(dir);
}

void HistoryModel::doBeginResetModel() {
  beginResetModel();
}

void HistoryModel::doEndResetModel() {
  endResetModel();
}

void HistoryModel::doBeginInsertRows() {
  beginInsertRows(QModelIndex(), 0, 0);
}

void HistoryModel::doEndInsertRows() {
  endInsertRows();
}

qsizetype HistoryModel::pageCount() const {
  return log_.pageCount();
}

qsizetype HistoryModel::page() const {
  return log_.page();
}

void HistoryModel::slotAddToHistory(QString record)
{
  log_.addRecord(record);
}

void HistoryModel::slotNextPage() {
  log_.previousPage();
}

void HistoryModel::slotPreviousPage() {
  log_.nextPage();
}

void HistoryModel::connect_() {
  connect(&log_, &Log::signalSizeChanged,
          this, &HistoryModel::signalSizeChanged);
  connect(&log_, &Log::signalPageCountChanged,
          this, &HistoryModel::signalPageCountChanged);
  connect(&log_, &Log::signalPageChanged,
          this, &HistoryModel::signalPageChanged);
}
