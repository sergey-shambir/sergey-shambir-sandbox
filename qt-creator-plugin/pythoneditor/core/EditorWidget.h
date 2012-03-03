#ifndef TEXTWIDGET_H_4b0236a6_e71d_4252_9360_5631f6fd5b5a
#define TEXTWIDGET_H_4b0236a6_e71d_4252_9360_5631f6fd5b5a

#include <texteditor/basetexteditor.h>
#include <utils/uncommentselection.h>

namespace PythonEditor {

/**
  Parent widget for PythonEditor::Editor class
  */
class EditorWidget : public TextEditor::BaseTextEditorWidget
{
    Q_OBJECT

public:
    EditorWidget(QWidget *parent = 0);
    virtual ~EditorWidget();

    /**
      Comments or uncomments selection using Python commenting syntax
      */
    virtual void unCommentSelection();

public slots:
    /**
      Handles common IDE fonts&colors settings
      (Tools -> Options -> Text editor -> Fonts and colors)
      */
    virtual void setFontSettings(const TextEditor::FontSettings &);

protected:
    TextEditor::BaseTextEditor *createEditor();

private:
    Utils::CommentDefinition m_commentDefinition;
};

} // namespace PythonEditor

#endif // TEXTWIDGET_H_4b0236a6_e71d_4252_9360_5631f6fd5b5a
