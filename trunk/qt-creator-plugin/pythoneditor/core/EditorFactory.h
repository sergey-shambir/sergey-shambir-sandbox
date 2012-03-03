#ifndef FACTORY_H_0f6e1e32_805b_4321_82ae_122208f582bd
#define FACTORY_H_0f6e1e32_805b_4321_82ae_122208f582bd

#include <coreplugin/editormanager/ieditorfactory.h>

#include <QtCore/QStringList>

namespace PythonEditor {

class EditorFactory : public Core::IEditorFactory
{
    Q_OBJECT

public:
    EditorFactory(QObject* parent);

    /**
      Returns MIME types handled by editor
      */
    virtual QStringList mimeTypes() const;

    /**
      Unique editor class identifier, see Constants::C_PYTHONEDITOR_ID
      */
    virtual Core::Id id() const;
    virtual QString displayName() const;

    /**
      Opens file in new editor
      */
    virtual Core::IFile *open(const QString &fileName);

    /**
      Creates and initializes new editor widget
      */
    virtual Core::IEditor *createEditor(QWidget *parent);

private:
    QStringList m_mimeTypes;
};

} // namespace GLSLEditor

#endif // FACTORY_H_0f6e1e32_805b_4321_82ae_122208f582bd
