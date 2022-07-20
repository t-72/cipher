#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <QJsonObject>
#include <QWidget>

#include "interfaces/ioptionsable.h"

#define DEFAULT_KEY 3
#define MIN_KEY 0
#define MAX_KEY 100

class QLabel;
class QLineEdit;

class Options : public QWidget, public interfaces::IOptionsable
{
  Q_OBJECT
public:
  explicit Options(const QString &title,
                   int defaultKey = DEFAULT_KEY,
                   QWidget *parent = nullptr);
  QString options(QJsonObject &doc) const override;

private:
  QLabel *label_;
  QLineEdit *lEdit_;
};

#endif // OPTIONS_H_
