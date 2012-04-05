#include "classnamevalidatingcombobox.h"
#include "classnamevalidator.h"

namespace Utils {

class ClassNameValidatingComboBoxPrivate
{
public:
    ClassNameValidatingComboBoxPrivate();
    ClassNameValidator m_validator;
    QString m_errorMessage;
};

ClassNameValidatingComboBoxPrivate::ClassNameValidatingComboBoxPrivate()
{
}

ClassNameValidatingComboBox::ClassNameValidatingComboBox(QWidget *parent) :
    QComboBox(parent) ,
    d(new ClassNameValidatingComboBoxPrivate())
{
}

ClassNameValidatingComboBox::~ClassNameValidatingComboBox()
{
    delete d;
}

bool ClassNameValidatingComboBox::namespacesEnabled() const
{
    return d->m_validator.namespacesEnabled();
}

void ClassNameValidatingComboBox::setNamespacesEnabled(bool b)
{
    d->m_validator.setNamespacesEnabled(b);
}

const QString &ClassNameValidatingComboBox::namespacesDelimiter() const
{
    return d->m_validator.namespaceDelimiter();
}

void ClassNameValidatingComboBox::setNamespaceDelimiter(const QString &delim)
{
    d->m_validator.setNamespaceDelimiter(delim);
}

bool ClassNameValidatingComboBox::isValid() const
{
    const QString baseClass = currentText().trimmed();
    if (!baseClass.isEmpty())
    {
        if (!d->m_validator.validate(baseClass)) {
            d->m_errorMessage = tr("Invalid base class name");
            return false;
        }
    }
    d->m_errorMessage.clear();
    return true;
}

const QString &ClassNameValidatingComboBox::errorMessage() const
{
    return d->m_errorMessage;
}

} // namespace Utils
