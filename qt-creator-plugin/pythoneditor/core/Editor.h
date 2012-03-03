#ifndef EDITABLE_H_3b9a2bb6_b26a_432e_889a_f108615aa584
#define EDITABLE_H_3b9a2bb6_b26a_432e_889a_f108615aa584

#include <texteditor/basetexteditor.h>

namespace PythonEditor {

class EditorWidget;

/**
  Implements interface IEditor, makes possible to edit Python source
  */
class Editor : public TextEditor::BaseTextEditor
{
    Q_OBJECT

public:
    explicit Editor(EditorWidget* editorWidget);
    virtual ~Editor();

    virtual bool duplicateSupported() const { return true; }
    virtual Core::IEditor* duplicate(QWidget *parent);

    /**
      Unique editor class identifier, see Constants::C_PYTHONEDITOR_ID
      */
    virtual QString id() const;
    virtual bool isTemporary() const { return false; }
    virtual QString preferredModeType() const;

    /**
      Opens file for editing, actual work performed by base class
      */
    virtual bool open(QString *errorString,
                      const QString &fileName,
                      const QString &realFileName);
};

} // PythonEditor

#endif // EDITABLE_H_3b9a2bb6_b26a_432e_889a_f108615aa584
