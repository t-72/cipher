#include "options.h"

#include <QFormLayout>
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QString>
#include <QIntValidator>
#include <QVBoxLayout>

Options::Options(const QString &title,
                 int defaultKey,
                 QWidget *parent)
  : QWidget(parent)
{
  setObjectName(title);
  label_ = new QLabel(tr("ключ k"));
  lEdit_ = new QLineEdit;
  lEdit_->setValidator(new QIntValidator(MIN_KEY, MAX_KEY, this));
  lEdit_->setText(QString::number(defaultKey));
  QFormLayout *fLayout = new QFormLayout;
  fLayout->addRow(label_, lEdit_);
  QGroupBox *gBox = new QGroupBox;
  gBox->setLayout(fLayout);
  gBox->setTitle(objectName());
  QVBoxLayout *vbLayout = new QVBoxLayout;
  vbLayout->addWidget(gBox);
  setLayout(vbLayout);
}

QString Options::options(QJsonObject &doc) const
{
  if (lEdit_->text().isEmpty())
    return tr("Введите число в поле") + " \"" + label_->text() + "\"";
  doc.insert("k", lEdit_->text().toInt());
  return QString();
}
