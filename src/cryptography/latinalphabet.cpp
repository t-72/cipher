#include "latinalphabet.h"

namespace cryptography
{

LatinAlphabet& LatinAlphabet::instance() {
  static LatinAlphabet inst;
  return inst;
}

LatinAlphabet::LatinAlphabet()
{
  const QString &alb = alphabet_();
  for (int i = 0; i < alb.size(); ++i)
    hash_.insert(alb[i], i);
}

int LatinAlphabet::size() {
  return alphabet_().size();
}

QChar LatinAlphabet::symbol(int index)
{
  const QString &alb = alphabet_();
  return alb[index];
}

int LatinAlphabet::index(QChar symbol)
{
  return hash_.value(symbol, -1);
}

const QString& LatinAlphabet::alphabet_() {
  const static QString alb("abcdefghijklmnopqrstuvwxyz");
  return alb;
}

} // namespace cryptography
