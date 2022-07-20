#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <utility>

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QSettings>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , worker_(new Worker(this))
  , hisModel_(new HistoryModel(this))
  , newIndex_(-1)
  , oldIndex_(-1)
  , curIndex_(0)
  , isEncrypt_(false)
{
  ui->setupUi(this);
  connect_();
  loadSettings_();
}

MainWindow::~MainWindow()
{
  saveSettings_();
  delete ui;
}

void MainWindow::slotPushButtonEncrClicked_()
{
  strView_.setStr(ui->textEditDecr->toPlainText());
  if (!strView_.view().isEmpty()) {
    options_ = QJsonObject();
    QString message = libConts_[newIndex_].encrOptions(options_);
    if (!message.isEmpty()) {
      QMessageBox::warning(nullptr,
                           objectName_(libConts_[newIndex_].encrOptWd()),
                           message);
      return;
    }
    startEncrypt_(newIndex_);
  }
}

void MainWindow::slotPushButtonDecrClicked_()
{
  strView_.setStr(ui->textEditEncr->toPlainText());
  if (!strView_.view().isEmpty()) {
    options_ = QJsonObject();
    QString message = libConts_[newIndex_].decrOptions(options_);
    if (!message.isEmpty()) {
      QMessageBox::warning(nullptr,
                           objectName_(libConts_[newIndex_].decrOptWd()),
                           message);
      return;
    }
    startDecrypt_(newIndex_);
  }
}

void MainWindow::slotComboBoxCiphersCurrentIndexChanged_(int) {
  if (oldIndex_ > -1) {
    hideWidget_(libConts_[oldIndex_].encrOptWd());
    hideWidget_(libConts_[oldIndex_].decrOptWd());
  }
  newIndex_ = ui->comboBoxCiphers->currentData().toInt();
  showWidget_(libConts_[newIndex_].encrOptWd());
  showWidget_(libConts_[newIndex_].decrOptWd());
  oldIndex_ = newIndex_;
}

void MainWindow::slotFinished_(bool isEnd, QString *str) {
  std::unique_ptr<QString> pStr(str);
  if (isEncrypt_)
    ui->textEditEncr->appendPlainText(*pStr);
  else
    ui->textEditDecr->appendPlainText(*pStr);
  if (isEnd) {
    addToHistory_();
    enableWidgets_();
    QTimer::singleShot(200, this, &MainWindow::slotResetProgressBar_);
  }
}

void MainWindow::slotResetProgressBar_() {
  ui->progressBar->setValue(0);
}

void MainWindow::slotSizeChanged_(int size) {
  ui->labelRecordSize->setText(QString::number(size));
}

void MainWindow::slotPageCountChanged_(qsizetype count) {
  ui->labelPages->setText(QString::number(count));
}

void MainWindow::slotPageChanged_(qsizetype number) {
  ui->labelPage->setText(QString::number(number));
}

void MainWindow::connect_() {
  connect(ui->pushButtonEncr, &QPushButton::clicked,
          this, &MainWindow::slotPushButtonEncrClicked_);
  connect(ui->pushButtonDecr, &QPushButton::clicked,
          this, &MainWindow::slotPushButtonDecrClicked_);
  connect(ui->comboBoxCiphers, QOverload<int>::of(&QComboBox::currentIndexChanged),
          this, &MainWindow::slotComboBoxCiphersCurrentIndexChanged_);
  connect(worker_, &Worker::signalFinished,
          this, &MainWindow::slotFinished_);
  connect(worker_, &Worker::signalProgress,
          ui->progressBar, &QProgressBar::setValue);
  connect(this, &MainWindow::signalAddToHistory,
          hisModel_, &HistoryModel::slotAddToHistory);
  connect(hisModel_, &HistoryModel::signalSizeChanged,
          this, &MainWindow::slotSizeChanged_);
  connect(hisModel_, &HistoryModel::signalPageCountChanged,
          this, &MainWindow::slotPageCountChanged_);
  connect(hisModel_, &HistoryModel::signalPageChanged,
          this, &MainWindow::slotPageChanged_);
  connect(ui->pushButtonPrevious, &QPushButton::clicked,
          hisModel_, &HistoryModel::slotPreviousPage);
  connect(ui->pushButtonNext, &QPushButton::clicked,
          hisModel_, &HistoryModel::slotNextPage);
}

void MainWindow::loadSettings_() {
  QString dir = QDir::toNativeSeparators(QApplication::applicationDirPath());
  QString iniFile = dir + QDir::separator() + "config.cfg";
  QString logsDir = dir + QDir::separator() + "logs";

  QSettings qSettings(iniFile, QSettings::IniFormat);
  QStringList ciphers = qSettings.value("Ciphers/ciphers", "")
      .toStringList();
  loadLibraries_(ciphers);
  ui->listViewHistory->setModel(hisModel_);
  hisModel_->setDir(logsDir);
}

void MainWindow::saveSettings_() {}

void MainWindow::loadLibraries_(const QStringList &ciphers)
{
  for (const QString &cipher : ciphers)
    addLibrary_(cipher);
}

void MainWindow::addLibrary_(const QString &name)
{
  LibContainer libCont(new QLibrary("lib" + name));
  if (libCont.isValid()) {
    libConts_.push_back(std::move(libCont));
    int index = libConts_.size() - 1;
    ui->comboBoxCiphers->addItem(*libConts_[index].name(), index);
    if (libConts_[index].isEncrOptWd()) {
      ui->verticalLayoutOptions->addWidget(libConts_[index].encrOptWd());
      if (index > 0)
        hideWidget_(libConts_[index].encrOptWd());
    }
    if (libConts_[index].isDecrOptWd()) {
      ui->verticalLayoutOptions->addWidget(libConts_[index].decrOptWd());
      if (index > 0)
        hideWidget_(libConts_[index].decrOptWd());
    }
    ui->pushButtonEncr->setEnabled(true);
    ui->pushButtonDecr->setEnabled(true);
  }
}

void MainWindow::hideWidget_(QWidget *widget) const
{
  if (widget != nullptr)
    widget->hide();
}

void MainWindow::showWidget_(QWidget *widget) const
{
  if (widget != nullptr)
    widget->show();
}

QString MainWindow::objectName_(QWidget *widget) const
{
  QString result;
  if (widget != nullptr)
    result = widget->objectName();
  return result;
}

void MainWindow::startEncrypt_(int index)
{
  disableWidgets_();
  isEncrypt_ = true;
  curIndex_ = index;
  ui->textEditEncr->clear();
  worker_->set(libConts_[index].encrFunc(), &strView_.view(), &options_);
  worker_->run();
}

void MainWindow::startDecrypt_(int index)
{
  disableWidgets_();
  isEncrypt_ = false;
  curIndex_ = index;
  ui->textEditDecr->clear();
  worker_->set(libConts_[index].decrFunc(), &strView_.view(), &options_);
  worker_->run();
}

void MainWindow::disableWidgets_() {
  ui->textEditEncr->setReadOnly(true);
  ui->textEditDecr->setReadOnly(true);
  ui->pushButtonEncr->setDisabled(true);
  ui->pushButtonDecr->setDisabled(true);
}

void MainWindow::enableWidgets_() {
  ui->textEditEncr->setReadOnly(false);
  ui->textEditDecr->setReadOnly(false);
  ui->pushButtonEncr->setEnabled(true);
  ui->pushButtonDecr->setEnabled(true);
}

void MainWindow::addToHistory_()
{
  QString strToHis = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss");
  strToHis += ": " + (isEncrypt_ ? tr("шифрование") : tr("дешифрование"));
  strToHis += ", " + *libConts_[curIndex_].name();
  emit signalAddToHistory(strToHis);
}

void MainWindow::StringView::setStr(const QString &str)
{
  str_ = str;
  strView_ = str_;
}

QStringView& MainWindow::StringView::view()
{
  return strView_;
}
