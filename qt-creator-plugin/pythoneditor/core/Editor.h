#ifndef EDITABLE_H_3b9a2bb6_b26a_432e_889a_f108615aa584
#define EDITABLE_H_3b9a2bb6_b26a_432e_889a_f108615aa584

#include <texteditor/basetexteditor.h>

namespace PythonEditor {

class CEditorWidget;

class CEditor : public TextEditor::BaseTextEditor
{
    Q_OBJECT

public:
    explicit CEditor(CEditorWidget* editorWidget);
    virtual ~CEditor();

    virtual bool duplicateSupported() const { return true; }
    virtual Core::IEditor* duplicate(QWidget *parent);

    virtual QString id() const;
    virtual bool isTemporary() const { return false; }
    virtual QString preferredModeType() const;

    virtual bool open(QString *errorString,
                      const QString &fileName,
                      const QString &realFileName);
};

} // PythonEditor

#endif // EDITABLE_H_3b9a2bb6_b26a_432e_889a_f108615aa584
