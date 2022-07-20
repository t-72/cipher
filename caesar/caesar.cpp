#include "interfaces/idynlibrary.h"

#include <algorithm>

#include <QDebug>
#include <QtMath>
#include <QPushButton>
#include <QObject>

#include "cryptography/latinalphabet.h"
#include "options.h"

using LatAlb = cryptography::LatinAlphabet;

bool checkParams(const QStringView *in, const QJsonObject *opt, int &k)
{
  bool result = false;
  if ((in != nullptr) && (opt != nullptr) && (!opt->isEmpty())) {
    QJsonValue jv = opt->value("k");
    if (jv.isDouble()) {
      k = jv.toInt();
      result = true;
    }
  }
  return result;
}

const QString* name()
{
  const static QString name_(QObject::tr("шифр Цезаря"));
  return &name_;
}

QWidget* encryptionOptionsWidget()
{
  Options *widget = new Options(QObject::tr("Параметры шифрования"));
  return widget;
}

QWidget* decryptionOptionsWidget()
{
  Options *widget = new Options(QObject::tr("Параметры дешифрования"));
  return widget;
}

QString* encrypt(const QStringView *in, const QJsonObject *opt)
{
  QString *out = new QString;
  int k;
  if (!checkParams(in, opt, k))
    return out;

  int n = LatAlb::instance().size();
  k = k % n;

  QChar original;
  QChar lower;
  QChar symbol;

  for (int i = 0; i < in->size(); ++i) {
    original = (*in)[i];
    lower = original.toLower();
    bool isUpper = (original != lower);
    int index = LatAlb::instance().index(lower);
    if (index > -1) {
      symbol = LatAlb::instance().symbol((index + k) % n);
      if (!isUpper)
        out->push_back(symbol);
      else
        out->push_back(symbol.toUpper());
    } else {
      out->push_back(original);
    }
  }
  return out;
}

QString* decrypt(const QStringView *in, const QJsonObject *opt)
{
  QString *out = new QString;
  int k;
  if (!checkParams(in, opt, k))
    return out;

  int n = LatAlb::instance().size();
  k = k % n;

  QChar original;
  QChar lower;
  QChar symbol;

  for (int i = 0; i < in->size(); ++i) {
    original = (*in)[i];
    lower = original.toLower();
    bool isUpper = (original != lower);
    int index = LatAlb::instance().index(lower);
    if (index > -1) {
      symbol = LatAlb::instance().symbol((index - k + n) % n);
      if (!isUpper)
        out->push_back(symbol);
      else
        out->push_back(symbol.toUpper());
    } else {
      out->push_back(original);
    }
  }
  return out;
}

