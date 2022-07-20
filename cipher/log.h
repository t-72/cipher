#ifndef LOG_H_
#define LOG_H_

#include <QDir>
#include <QFile>
#include <QMap>
#include <QObject>
#include <QString>

#include "interfaces/imodelable.h"

#define MAX_LINES 1000

class Log : public QObject
{
  Q_OBJECT
public:
  using IModelable = interfaces::IModelable;

  explicit Log(IModelable *model = nullptr,
               QObject *parent = nullptr);
  ~Log();
  const QString& operator [] (int i) const;
  void setDir(const QString &path);
  int size() const;
  qsizetype pageCount() const;
  qsizetype page() const;
  void nextPage();
  void previousPage();
  void addRecord(QString record);

signals:
  void signalSizeChanged(int size);
  void signalPageCountChanged(qsizetype count);
  void signalPageChanged(qsizetype number);

private:
  void load_(const QString &path);
  void save_();
  bool checkFile_(const QString &fileName);
  void removeFile_(const QString &fileName) const;
  void reopen_(const QString &fileName);
  QString fileName_(qsizetype fileNum) const;
  QString fileName_() const;
  QString nextFileName_();
  void saveRecord_(const QString &record);
  void clear_();
  void clearShadow_();
  void reset_();
  bool checkCompletion_() const;
  void beginResetModel_();
  void endResetModel_();
  void beginInsertRows_();
  void endInsertRows_();
  bool isWorkPage_() const;
  void loadShadow_(const QString &fileName);

  const QString patternBaseName_;
  IModelable *model_;
  QString path_;
  QFile file_;
  QList<QString> records_;
  QList<QString> shadow_;
  qsizetype fileNum_;
  qsizetype page_;
};

#endif // LOG_H_
