#include "interfaces/idynlibrary.h"

#include <QPushButton>
#include <QObject>

#include "cryptography/latinalphabet.h"

using LatAlb = cryptography::LatinAlphabet;

const QString* name()
{
  const static QString name_(QObject::tr("шифр Атбаш"));
  return &name_;
}

QWidget* encryptionOptionsWidget()
{
  return nullptr;
}

QWidget* decryptionOptionsWidget()
{
  return nullptr;
}

QString* encrypt(const QStringView *in, const QJsonObject*)
{
  QString *out = new QString;
  if (in == nullptr)
    return out;

  int n = LatAlb::instance().size();
  QChar original;
  QChar lower;
  QChar symbol;

  for (int i = 0; i < in->size(); ++i) {
    original = (*in)[i];
    lower = original.toLower();
    bool isUpper = (original != lower);
    int index = LatAlb::instance().index(lower);
    if (index > -1) {
      symbol = LatAlb::instance().symbol(n - index - 1);
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

QString* decrypt(const QStringView *in, const QJsonObject*)
{
  return encrypt(in);
}
