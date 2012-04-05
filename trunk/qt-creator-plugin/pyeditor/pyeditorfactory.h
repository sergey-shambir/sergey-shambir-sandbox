#ifndef PYEDITOR_EDITORFACTORY_H
#define PYEDITOR_EDITORFACTORY_H

#include "pyeditorglobal.h"
#include <coreplugin/editormanager/ieditorfactory.h>
#include <QtCore/QStringList>

namespace PyEditor {

class PYEDITOR_EXPORT EditorFactory : public Core::IEditorFactory
{
    Q_OBJECT

public:
    EditorFactory(QObject* parent);

    /**
      Returns MIME types handled by editor
      */
    /*virtual*/ QStringList mimeTypes() const;

    /**
      Unique editor class identifier, see Constants::C_PYEDITOR_ID
      */
    /*virtual*/ Core::Id id() const;
    /*virtual*/ QString displayName() const;

    /**
      Opens file in new editor
      */
    /*virtual*/ Core::IDocument *open(const QString &fileName);

    /**
      Creates and initializes new editor widget
      */
    /*virtual*/ Core::IEditor *createEditor(QWidget *parent);

private:
    QStringList _mimeTypes;
};

} // namespace PyEditor

#endif // PYEDITOR_EDITORFACTORY_H
