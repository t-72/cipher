#include "libcontainer.h"

#include <cstring>

LibContainer::LibContainer(QLibrary *lib)
  : lib_(lib)
  , isValid_(false)
{
  if (lib_ != nullptr) {
    ptrs_.nameFunc = reinterpret_cast<NameFuncType>(lib->resolve("name"));
    ptrs_.encrFunc = reinterpret_cast<EncrDecrFuncType>(lib->resolve("encrypt"));
    ptrs_.decrFunc = reinterpret_cast<EncrDecrFuncType>(lib->resolve("decrypt"));
    OptionsWidgetFuncType encrOptWdFunc = reinterpret_cast<OptionsWidgetFuncType>(
          lib->resolve("encryptionOptionsWidget"));
    OptionsWidgetFuncType decrOptWdFunc = reinterpret_cast<OptionsWidgetFuncType>(
          lib->resolve("decryptionOptionsWidget"));

    isValid_ = (ptrs_.nameFunc != nullptr) &&
        (ptrs_.encrFunc != nullptr) &&
        (ptrs_.decrFunc != nullptr) &&
        (encrOptWdFunc != nullptr) &&
        (decrOptWdFunc != nullptr);

    if (isValid_) {
      ptrs_.encrOptWd = encrOptWdFunc();
      ptrs_.decrOptWd = decrOptWdFunc();
      ptrs_.encrIOpts = ((ptrs_.encrOptWd != nullptr) ?
                           dynamic_cast<IOpts*>(ptrs_.encrOptWd) :
                           nullptr);
      ptrs_.decrIOpts = ((ptrs_.decrOptWd != nullptr) ?
                           dynamic_cast<IOpts*>(ptrs_.decrOptWd) :
                           nullptr);
    }
  }
}

LibContainer::LibContainer(LibContainer &&other) noexcept
  : lib_(other.lib_.release())
  , ptrs_(other.ptrs_)
{
  other.resetPtrs_();
}

LibContainer& LibContainer::operator = (LibContainer &&other) noexcept
{
  lib_ = std::move(other.lib_);
  ptrs_ = other.ptrs_;
  other.resetPtrs_();
  return *this;
}

bool LibContainer::isValid() const
{
  return isValid_;
}

bool LibContainer::isEncrOptWd() const
{
  return ptrs_.encrOptWd != nullptr;
}

bool LibContainer::isDecrOptWd() const
{
  return ptrs_.decrOptWd != nullptr;
}

const QString* LibContainer::name() const
{
  return ptrs_.nameFunc();
}

LibContainer::EncrDecrFuncType LibContainer::encrFunc() const
{
  return ptrs_.encrFunc;
}

LibContainer::EncrDecrFuncType LibContainer::decrFunc() const
{
  return ptrs_.decrFunc;
}

QWidget* LibContainer::encrOptWd() const
{
  return ptrs_.encrOptWd;
}

QWidget* LibContainer::decrOptWd() const
{
  return ptrs_.decrOptWd;
}

QString LibContainer::encrOptions(QJsonObject &doc) const
{
  QString result;
  if (ptrs_.encrIOpts != nullptr)
    result = ptrs_.encrIOpts->options(doc);
  return result;
}

QString LibContainer::decrOptions(QJsonObject &doc) const
{
  QString result;
  if (ptrs_.decrIOpts != nullptr)
    result = ptrs_.decrIOpts->options(doc);
  return result;
}

void LibContainer::resetPtrs_()
{
  std::memset(&ptrs_, 0, sizeof (ptrs_));
}
