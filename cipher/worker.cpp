#include "worker.h"

#include <memory>

Worker::Worker(QObject *parent)
  : QObject(parent)
  , fWatcher_(new QFutureWatcher<QString*>(this))
  , func_(nullptr)
  , strIn_(nullptr)
  , options_(nullptr)
  , isRun_(false)
  , count_(0)
  , counter_(0)
  , pos_(0)
  , portion_(PORTION)
{
  connect(fWatcher_, &QFutureWatcher<QString>::finished,
          this, &Worker::slotFinished_);
}

Worker::~Worker() {
  fWatcher_->cancel();
  fWatcher_->waitForFinished();
}

void Worker::set(LibContainer::EncrDecrFuncType func,
                 const QStringView *str,
                 const QJsonObject *options)
{
  if (isRun_) return;
  func_ = func;
  strIn_ = str;
  if (strIn_ != nullptr) {
    sizeStrIn_ = strIn_->size();
    count_ = (sizeStrIn_ / portion_)
        + ((sizeStrIn_ % portion_) > 0 ? 1 : 0);
  }
  options_ = options;
}

void Worker::run()
{
  if (isRun_) return;
  isRun_ = true;
  counter_ = 0;
  pos_ = 0;
  if ((func_ == nullptr) || (strIn_ == nullptr)) {
    emit signalProgress(MAX_PERCENT);
    emit signalFinished(true, new QString);
    isRun_ = false;
    return;
  }
  str_ = strIn_->mid(pos_, qMin(portion_, sizeStrIn_ - pos_));
  fWatcher_->setFuture(QtConcurrent::run(func_, &str_, options_));
}

void Worker::slotFinished_()
{
  std::unique_ptr<QString> out(fWatcher_->result());
  ++counter_;
  if (counter_ == count_) {
    isRun_ = false;
    emit signalProgress(MAX_PERCENT);
    emit signalFinished(true, out.release());
  } else {
    emit signalProgress(((counter_ * MAX_PERCENT) / count_));
    emit signalFinished(false, out.release());
    pos_ += PORTION;
    str_ = strIn_->mid(pos_, qMin(portion_, sizeStrIn_ - pos_));
    fWatcher_->setFuture(QtConcurrent::run(func_, &str_, options_));
  }
}
