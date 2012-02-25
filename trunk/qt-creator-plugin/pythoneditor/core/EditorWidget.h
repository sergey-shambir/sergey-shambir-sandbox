#ifndef TEXTWIDGET_H_4b0236a6_e71d_4252_9360_5631f6fd5b5a
#define TEXTWIDGET_H_4b0236a6_e71d_4252_9360_5631f6fd5b5a

/**
  QtCreator объявляет в пространстве имён Core интерфейсы IFile и IEditor
  Чтобы позволить средствами плагина редактировать некоторый тип файлов,
  необходимо реализовать интерфейсы IFile и IEditor.
  Эти интерфейсы не привязаны к обычному текстовому полю, и
  способ редактирования файлов может быть любым, например, как в Firebug.

  Здесь редактор унаследован от BaseTextEditorWidget.
  */

#include <texteditor/basetexteditor.h>
#include <utils/uncommentselection.h>

// Preliminary definitions
QT_BEGIN_NAMESPACE
class QTimer;
QT_END_NAMESPACE
// end

namespace PythonEditor {
namespace Internal {

class CEditorWidget : public TextEditor::BaseTextEditorWidget
{
    Q_OBJECT

public:
    CEditorWidget(QWidget *parent = 0);
    virtual ~CEditorWidget();

    virtual void unCommentSelection();

public slots:
    virtual void setFontSettings(const TextEditor::FontSettings &);

protected:
    TextEditor::BaseTextEditor *createEditor();
    /*
    bool event(QEvent *e);
    void createToolBar(Internal::GLSLEditorEditable *editable);
    */

private slots:
   // void updateDocument();
   // void updateDocumentNow();

private:
    Utils::CommentDefinition m_commentDefinition;
  //  QTimer *m_updateDocumentTimer;
   // QComboBox *m_outlineCombo;
   // Document::Ptr m_document;
};

} // namespace Internal
} // namespace PythonEditor

#endif // TEXTWIDGET_H_4b0236a6_e71d_4252_9360_5631f6fd5b5a
