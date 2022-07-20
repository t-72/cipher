#ifndef CRYPTOGRAPHY_LATINALPHABET_H_
#define CRYPTOGRAPHY_LATINALPHABET_H_

#include <QChar>
#include <QHash>
#include <QString>

namespace cryptography
{

class LatinAlphabet
{
public:
  static LatinAlphabet& instance();
  ~LatinAlphabet()                                 = default;
  LatinAlphabet(const LatinAlphabet&)              = delete;
  LatinAlphabet(LatinAlphabet&&)                   = delete;
  LatinAlphabet& operator = (const LatinAlphabet&) = delete;
  LatinAlphabet& operator = (LatinAlphabet&&)      = delete;

  int size();
  QChar symbol(int index);
  int index(QChar symbol);
private:
  LatinAlphabet();
  const QString& alphabet_();
  QHash<QChar, int> hash_;
};

} // namespace cryptography

#endif // CRYPTOGRAPHY_LATINALPHABET_H_
