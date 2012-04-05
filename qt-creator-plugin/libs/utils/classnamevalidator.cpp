#include "classnamevalidator.h"
#include <utils/qtcassert.h>

namespace Utils {

ClassNameValidator::ClassNameValidator()
    :m_namespacesEnabled(true)
{
    setNamespaceDelimiter("::");
}

bool ClassNameValidator::namespacesEnabled() const
{
    return m_namespacesEnabled;
}

void ClassNameValidator::setNamespacesEnabled(bool b)
{
    m_namespacesEnabled = b;
}

const QString &ClassNameValidator::namespaceDelimiter() const
{
    return m_namespaceDelimiter;
}

void ClassNameValidator::setNamespaceDelimiter(const QString &delimiter)
{
    m_namespaceDelimiter = delimiter;
    m_nameRegexp = QRegExp(QString("[a-zA-Z_][a-zA-Z0-9_]*("
                                   "%1"
                                   "[a-zA-Z_][a-zA-Z0-9_]*)*")
            .arg(m_namespaceDelimiter));
    QTC_ASSERT(m_nameRegexp.isValid(), return);
}

bool ClassNameValidator::validate(const QString &value) const
{
    return m_nameRegexp.exactMatch(value);
}

} // namespace Utils
