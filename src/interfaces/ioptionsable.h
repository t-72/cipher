#ifndef INTERFACES_IOPTIONSABLE_H_
#define INTERFACES_IOPTIONSABLE_H_

#include <QJsonObject>
#include <QString>

namespace interfaces
{

struct IOptionsable
{
  virtual ~IOptionsable() = default;
  virtual QString options(QJsonObject &doc) const = 0;
};

} // namespace interfaces

#endif // INTERFACES_IOPTIONSABLE_H_
