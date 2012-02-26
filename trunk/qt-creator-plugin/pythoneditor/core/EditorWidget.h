#ifndef TEXTWIDGET_H_4b0236a6_e71d_4252_9360_5631f6fd5b5a
#define TEXTWIDGET_H_4b0236a6_e71d_4252_9360_5631f6fd5b5a

/**
  QtCreator объявляет в пространстве имён Core интерфейсы IFile и IEditor
  Чтобы позволить средствами плагина редактировать некоторый тип файлов,
  необходимо реализовать интерфейсы IFile и IEditor.
  Эти интерфейсы не привязаны к обычному текстовому полю, и
  способ редактирования файлов может быть любым, например, как в Firebug.

********************************************************************************
  Редактор унаследован от BaseTextEditorWidget.
  Базовый класс является платформой, а не самостоятельным редактором,
  включать те или иные возможности нужно явно, по умолчанию отключена даже
  история редактора (Do/Undo).
  */

#include <texteditor/basetexteditor.h>
#include <utils/uncommentselection.h>

namespace PythonEditor {

class CEditorWidget : public TextEditor::BaseTextEditorWidget
{
    Q_OBJECT

public:
    CEditorWidget(QWidget *parent = 0);
    virtual ~CEditorWidget();

    virtual void unCommentSelection();

public slots:
    /**
      Обработка общих для IDE настроек подсветки кода, ошибок, предупреждений
      */
    virtual void setFontSettings(const TextEditor::FontSettings &);

protected:
    TextEditor::BaseTextEditor *createEditor();

private:
    Utils::CommentDefinition m_commentDefinition;
};

} // namespace PythonEditor

#endif // TEXTWIDGET_H_4b0236a6_e71d_4252_9360_5631f6fd5b5a
