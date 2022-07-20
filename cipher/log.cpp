#include "log.h"

#include <limits>

#include <QApplication>
#include <QDebug>
#include <QRegExp>
#include <QSettings>
#include <QTextStream>

Log::Log(IModelable *model, QObject *parent)
  : QObject(parent)
  , patternBaseName_("log%1.txt")
  , model_(model)
  , fileNum_(0)
  , page_(fileNum_)
{}

Log::~Log() {
  file_.close();
  save_();
}

const QString& Log::operator [] (int i) const {
  if (isWorkPage_())
    return records_[i];
  else
    return shadow_[i];
}

void Log::setDir(const QString &path) {
  beginResetModel_();
  save_();
  load_(path);
  endResetModel_();
}

int Log::size() const {
  if (isWorkPage_())
    return records_.size();
  else
    return shadow_.size();
}

qsizetype Log::pageCount() const {
  return fileNum_ + 1;
}

qsizetype Log::page() const {
  return page_;
}

void Log::nextPage() {
  if (page_ < fileNum_) {
    beginResetModel_();
    ++page_;
    clearShadow_();
    if (!isWorkPage_())
      loadShadow_(fileName_(page_));
    endResetModel_();

    emit signalSizeChanged(size());
    emit signalPageChanged(page_ + 1);
  }
}

void Log::previousPage() {
  if (page_ > 0) {
    beginResetModel_();
    --page_;
    clearShadow_();
    loadShadow_(fileName_(page_));
    endResetModel_();

    emit signalSizeChanged(size());
    emit signalPageChanged(page_ + 1);
  }
}

void Log::addRecord(QString record) {
  if (checkCompletion_()) {
    if (isWorkPage_())
      beginResetModel_();
    clear_();
    reopen_(nextFileName_());
    saveRecord_(record);
    if (isWorkPage_())
      endResetModel_();
  } else {
    if (isWorkPage_())
      beginInsertRows_();
    saveRecord_(record);
    if (isWorkPage_())
      endInsertRows_();
  }
  emit signalSizeChanged(size());
}

void Log::load_(const QString &path) {
  reset_();
  if (!path.isEmpty()) {
    QDir dir(path);
    path_ = dir.path();
    if (dir.exists()) {
      QString iniFile = path_ + QDir::separator() + "log.cfg";
      QSettings qSettings(iniFile, QSettings::IniFormat);
      fileNum_ = qSettings.value("file_number", fileNum_).toULongLong();
      page_ = fileNum_;
    } else {
      dir.mkpath(".");
    }
    if (!checkFile_(fileName_()))
      nextFileName_();
    reopen_(fileName_());
  }
  emit signalSizeChanged(size());
  emit signalPageCountChanged(fileNum_ + 1);
  emit signalPageChanged(page_ + 1);
}

void Log::save_() {
  if (!path_.isEmpty()) {
    QDir dir(path_);
    if (dir.exists()) {
      QString iniFile = dir.path() + QDir::separator() + "log.cfg";
      QSettings qSettings(iniFile, QSettings::IniFormat);
      qSettings.setValue("file_number", fileNum_);
    }
  }
}

bool Log::checkFile_(const QString &fileName) {
  int result = true;
  if (QFile::exists(fileName)) {
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    int counter = 0;
    while (!stream.atEnd()) {
      ++counter;
      if (MAX_LINES <= counter) {
        records_.clear();
        result = false;
        break;
      }
      records_.push_back(stream.readLine());
    }
    file.close();
  }
  return result;
}

void Log::removeFile_(const QString &fileName) const {
  if (QFile::exists(fileName))
    QFile::remove(fileName);
}

void Log::reopen_(const QString &fileName) {
  file_.close();
  file_.setFileName(fileName);
  file_.open(QIODevice::WriteOnly | QIODevice::Append);
}

QString Log::fileName_(qsizetype fileNum) const {
  return path_ + QDir::separator()
      + patternBaseName_.arg(fileNum);
}

QString Log::fileName_() const {
  return fileName_(fileNum_);
}

QString Log::nextFileName_() {
  if (page_ == fileNum_) {
    ++page_;
    emit signalPageChanged(page_ + 1);
  }
  ++fileNum_;
  QString result = fileName_();
  removeFile_(result);

  emit signalPageCountChanged(fileNum_ + 1);
  return result;
}

void Log::saveRecord_(const QString &record) {
  records_.push_back(record);
  if (file_.isOpen()) {
    QTextStream stream(&file_);
    stream << record << Qt::endl;
  }
}

void Log::clear_() {
  file_.close();
  records_.clear();
}

void Log::clearShadow_() {
  shadow_.clear();
}

void Log::reset_() {
  file_.close();
  records_.clear();
  shadow_.clear();
  fileNum_ = page_ = 0;

  emit signalPageCountChanged(fileNum_ + 1);
  emit signalPageChanged(page_ + 1);
}

bool Log::checkCompletion_() const {
  return MAX_LINES == records_.size();
}

void Log::beginResetModel_() {
  if (model_ != nullptr)
    model_->doBeginResetModel();
}

void Log::endResetModel_() {
  if (model_ != nullptr)
    model_->doEndResetModel();
}

void Log::beginInsertRows_() {
  if (model_ != nullptr)
    model_->doBeginInsertRows();
}

void Log::endInsertRows_() {
  if (model_ != nullptr)
    model_->doEndInsertRows();
}

bool Log::isWorkPage_() const {
  return fileNum_ == page_;
}

void Log::loadShadow_(const QString &fileName)
{
  if (QFile::exists(fileName)) {
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    while (!stream.atEnd())
      shadow_.push_back(stream.readLine());
    file.close();
  }
}
