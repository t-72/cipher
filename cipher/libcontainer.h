#ifndef LIBCONTAINER_H_
#define LIBCONTAINER_H_

#include <memory>
#include <utility>

#include <QJsonObject>
#include <QLibrary>
#include <QString>
#include <QWidget>

#include "interfaces/ioptionsable.h"

class LibContainer
{
public:
  using NameFuncType = const QString* (*)();
  using OptionsWidgetFuncType = QWidget* (*)();
  using EncrDecrFuncType = QString* (*)(const QStringView*,
                                        const QJsonObject*);

  LibContainer(const LibContainer&) = delete;
  LibContainer& operator = (const LibContainer&) = delete;

  LibContainer(QLibrary *lib);
  LibContainer(LibContainer&&) noexcept;
  LibContainer& operator = (LibContainer&&) noexcept;

  bool isValid() const;
  bool isEncrOptWd() const;
  bool isDecrOptWd() const;
  const QString* name() const;
  EncrDecrFuncType encrFunc() const;
  EncrDecrFuncType decrFunc() const;
  QWidget *encrOptWd() const;
  QWidget *decrOptWd() const;
  QString encrOptions(QJsonObject &doc) const;
  QString decrOptions(QJsonObject &doc) const;

private:
  using IOpts = interfaces::IOptionsable;

  struct Pointers {
    NameFuncType nameFunc;
    EncrDecrFuncType encrFunc;
    EncrDecrFuncType decrFunc;
    QWidget *encrOptWd;
    QWidget *decrOptWd;
    IOpts *encrIOpts;
    IOpts *decrIOpts;
  };

  void resetPtrs_();

  std::unique_ptr<QLibrary> lib_;
  Pointers ptrs_;
  bool isValid_;
};

#endif // LIBCONTAINER_H_
