#ifndef LOGMODEL_H_
#define LOGMODEL_H_

#include <QAbstractListModel>

#include "interfaces/imodelable.h"
#include "log.h"

class HistoryModel : public QAbstractListModel,
                     public interfaces::IModelable
{
  Q_OBJECT
public:
  explicit HistoryModel(QObject *parent = nullptr);
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  int rowCount(const QModelIndex &parent) const override;
  void setDir(const QString &dir);

  void doBeginResetModel() override;
  void doEndResetModel() override;
  void doBeginInsertRows() override;
  void doEndInsertRows() override;

  qsizetype pageCount() const;
  qsizetype page() const;

signals:
  void signalSizeChanged(int size);
  void signalPageCountChanged(qsizetype count);
  void signalPageChanged(qsizetype number);

public slots:
  void slotAddToHistory(QString record);
  void slotNextPage();
  void slotPreviousPage();

private:
  void connect_();

  Log log_;
};

#endif // LOGMODEL_H_
