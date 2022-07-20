#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <memory>
#include <vector>

#include <QMainWindow>
#include <QtConcurrent>

#include "libcontainer.h"
#include "historymodel.h"
#include "worker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

signals:
  void signalAddToHistory(QString record);

private slots:
  void slotPushButtonEncrClicked_();
  void slotPushButtonDecrClicked_();
  void slotComboBoxCiphersCurrentIndexChanged_(int index);
  void slotFinished_(bool isEnd, QString *str);
  void slotResetProgressBar_();
  void slotSizeChanged_(int size);
  void slotPageCountChanged_(qsizetype count);
  void slotPageChanged_(qsizetype number);

private:
  class StringView {
  public:
    StringView() = default;
    void setStr(const QString &str);
    QStringView& view();
  private:
    QString str_;
    QStringView strView_;
  };

  void connect_();
  void loadSettings_();
  void saveSettings_();
  void loadLibraries_(const QStringList &ciphers);
  void addLibrary_(const QString &name);
  void hideWidget_(QWidget *widget) const;
  void showWidget_(QWidget *widget) const;
  QString objectName_(QWidget *widget) const;
  void startEncrypt_(int index);
  void startDecrypt_(int index);
  void disableWidgets_();
  void enableWidgets_();
  void addToHistory_();

  Ui::MainWindow *ui;
  std::vector<LibContainer> libConts_;
  Worker *worker_;
  HistoryModel *hisModel_;
  StringView strView_;
  int newIndex_;
  int oldIndex_;
  int curIndex_;
  bool isEncrypt_;
  QJsonObject options_;
};

#endif // MAINWINDOW_H_
