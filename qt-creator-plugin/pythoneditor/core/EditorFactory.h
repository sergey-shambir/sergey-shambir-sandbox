#ifndef FACTORY_H_0f6e1e32_805b_4321_82ae_122208f582bd
#define FACTORY_H_0f6e1e32_805b_4321_82ae_122208f582bd

#include <coreplugin/editormanager/ieditorfactory.h>

#include <QtCore/QStringList>

namespace PythonEditor {
namespace Internal {

class CEditorFactory : public Core::IEditorFactory
{
    Q_OBJECT

public:
    CEditorFactory(QObject* parent);

    virtual QStringList mimeTypes() const;
    virtual Core::Id id() const;
    virtual QString displayName() const;
    virtual Core::IFile *open(const QString &fileName);
    virtual Core::IEditor *createEditor(QWidget *parent);

private:
    QStringList m_mimeTypes;
};

} // namespace Internal
} // namespace GLSLEditor

#endif // FACTORY_H_0f6e1e32_805b_4321_82ae_122208f582bd
