#ifndef PYEDITOR_EDITORWIDGET_H
#define PYEDITOR_EDITORWIDGET_H

#include <texteditor/basetexteditor.h>
#include <utils/uncommentselection.h>

#include "pyeditorglobal.h"

namespace PyEditor {

class PYEDITOR_EXPORT EditorWidget : public TextEditor::BaseTextEditorWidget
{
    Q_OBJECT

public:
    EditorWidget(QWidget *parent = 0);
    virtual ~EditorWidget();

    virtual void unCommentSelection();

public slots:
    virtual void setFontSettings(const TextEditor::FontSettings &);

protected:
    TextEditor::BaseTextEditor *createEditor();

private:
    Utils::CommentDefinition _commentDefinition;
};

} // namespace PyEditor

#endif // PYEDITOR_EDITORWIDGET_H
