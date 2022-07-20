#ifndef INTERFACES_IMODELABLE_H_
#define INTERFACES_IMODELABLE_H_

namespace interfaces
{

struct IModelable
{
  virtual ~IModelable() = default;
  virtual void doBeginResetModel() = 0;
  virtual void doEndResetModel() = 0;
  virtual void doBeginInsertRows() = 0;
  virtual void doEndInsertRows() = 0;
};

} // namespace interfaces

#endif // INTERFACES_IMODELABLE_H_
