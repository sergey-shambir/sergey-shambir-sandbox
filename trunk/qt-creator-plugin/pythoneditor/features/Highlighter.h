#ifndef PYTHON_EDITOR__HIGHLIGHTER__INCLUDED
#define PYTHON_EDITOR__HIGHLIGHTER__INCLUDED

/**
  CHighlighter отвечает за подсветку синтаксиса языка Python.
  Подсветка оперирует блоками, где каждый блок - одна строка исходного кода.
  */

#include <texteditor/syntaxhighlighter.h>
#include <texteditor/fontsettings.h>

namespace PythonEditor {

class CHighlighter : public TextEditor::SyntaxHighlighter
{
    Q_OBJECT

public:
    explicit CHighlighter(TextEditor::BaseTextDocument *parent);
    virtual ~CHighlighter();

    /**
      В QtCreator есть свои настройки подсветки, CHighlighter должен единожды
      получить к ним доступ до первого использования highlightBlock();.
      Этот метод вызывается из TextEditor::BaseTextEditorWidget::setFontSettings();
      */
    void SetFontSettings(const TextEditor::FontSettings &fs);

protected:
    /**
      Подсветка одной строки кода, переданной редактором
      */
    virtual void highlightBlock(const QString &text);

private:
    QVector<QTextCharFormat> m_formats;
};

} // namespace PythonEditor

#endif // PYTHON_EDITOR__HIGHLIGHTER__INCLUDED
