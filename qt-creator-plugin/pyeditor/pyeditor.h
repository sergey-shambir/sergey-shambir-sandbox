#ifndef PYEDITOR_EDITOR_H
#define PYEDITOR_EDITOR_H

#include "pyeditorglobal.h"
#include <texteditor/basetexteditor.h>

namespace PyEditor {

class EditorWidget;

class PYEDITOR_EXPORT Editor : public TextEditor::BaseTextEditor
{
    Q_OBJECT

public:
    explicit Editor(EditorWidget* editorWidget);
    virtual ~Editor();

    /*virtual*/ bool duplicateSupported() const { return true; }
    /*virtual*/ Core::IEditor* duplicate(QWidget *parent);

    /**
      Unique editor class identifier, see Constants::C_PYEDITOR_ID
      */
    /*virtual*/ Core::Id id() const;
    /*virtual*/ bool isTemporary() const { return false; }
    /*virtual*/ QString preferredModeType() const;

    /**
      Opens file for editing, actual work performed by base class
      */
    bool open(QString *errorString,
              const QString &fileName,
              const QString &realFileName);
};

} // namespace PyEditor

#endif // PYEDITOR_EDITOR_H
