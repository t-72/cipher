#ifndef WORKER_H_
#define WORKER_H_

#include <QObject>
#include <QString>
#include <QStringRef>
#include <QtConcurrent>

#include "libcontainer.h"

#define PORTION 16384
#define MAX_PERCENT 100

class Worker : public QObject
{
  Q_OBJECT
public:
  explicit Worker(QObject *parent = nullptr);
  ~Worker();

  void set(LibContainer::EncrDecrFuncType func,
           const QStringView *str,
           const QJsonObject *options);
  void run();

signals:
  void signalProgress(int value);
  void signalFinished(bool isEnd, QString *str);

private slots:
  void slotFinished_();

private:
  QFutureWatcher<QString*> *fWatcher_;
  LibContainer::EncrDecrFuncType func_;
  const QStringView *strIn_;
  const QJsonObject *options_;
  bool isRun_;
  QStringView str_;
  qsizetype count_;
  qsizetype counter_;
  qsizetype pos_;
  qsizetype sizeStrIn_;
  const qsizetype portion_;
};

#endif // WORKER_H_
