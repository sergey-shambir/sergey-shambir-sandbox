#ifndef UTILS_CLASSNAMEVALIDATOR_H
#define UTILS_CLASSNAMEVALIDATOR_H

#include "utils_global.h"
#include <QtCore/QRegExp>

namespace Utils {

/**
 * @brief The ClassNameValidator class - checks if string is valid class
 * name using given conventions about modules (namespaces in C++)
 * and modules (namespaces) delimiters
 */
class QTCREATOR_UTILS_EXPORT ClassNameValidator
{
public:
    ClassNameValidator();

    bool namespacesEnabled() const;
    void setNamespacesEnabled(bool b);

    // in C++ delimiter is "::", but Python uses dot
    const QString &namespaceDelimiter() const;
    void setNamespaceDelimiter(const QString &delimiter);

    bool validate(const QString &value) const;

private:
    QRegExp m_nameRegexp;
    QString m_namespaceDelimiter;
    bool m_namespacesEnabled;
};

} // namespace Utils

#endif // UTILS_CLASSNAMEVALIDATOR_H
