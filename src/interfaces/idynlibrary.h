#ifndef INTERFACES_IDYNLIBRARY_H_
#define INTERFACES_IDYNLIBRARY_H_

#include <QJsonObject>
#include <QString>
#include <QWidget>

#if defined(LIBRARY)
#  define EXPORT Q_DECL_EXPORT
#else
#  define EXPORT Q_DECL_IMPORT
#endif

extern "C" {
EXPORT const QString* name();
EXPORT QWidget* encryptionOptionsWidget();
EXPORT QWidget* decryptionOptionsWidget();
EXPORT QString* encrypt(const QStringView *in,
                        const QJsonObject *opt = nullptr);
EXPORT QString* decrypt(const QStringView *in,
                        const QJsonObject *opt = nullptr);
}

#endif // INTERFACES_IDYNLIBRARY_H_
