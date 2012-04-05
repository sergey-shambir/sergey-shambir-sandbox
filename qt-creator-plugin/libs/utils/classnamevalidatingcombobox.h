#ifndef UTILS_CLASSNAMEVALIDATINGCOMBOBOX_H
#define UTILS_CLASSNAMEVALIDATINGCOMBOBOX_H

#include <QtGui/QComboBox>

namespace Utils {

class ClassNameValidatingComboBoxPrivate;

class ClassNameValidatingComboBox : public QComboBox
{
    Q_OBJECT
    Q_PROPERTY(bool namespacesEnabled READ namespacesEnabled WRITE setNamespacesEnabled DESIGNABLE true)
    Q_PROPERTY(QString namespacesDelimiter READ namespacesDelimiter WRITE setNamespaceDelimiter DESIGNABLE true)

public:
    explicit ClassNameValidatingComboBox(QWidget *parent = 0);
    virtual ~ClassNameValidatingComboBox();

    bool namespacesEnabled() const;
    void setNamespacesEnabled(bool b);

    const QString &namespacesDelimiter() const;
    void setNamespaceDelimiter(const QString &delim);

    bool isValid() const;
    const QString &errorMessage() const;

private:
    ClassNameValidatingComboBoxPrivate *d;
};

} // namespace Utils

#endif // UTILS_CLASSNAMEVALIDATINGCOMBOBOX_H
