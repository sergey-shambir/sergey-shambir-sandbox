#ifndef PYTHON_EDITOR__HIGHLIGHTER__INCLUDED
#define PYTHON_EDITOR__HIGHLIGHTER__INCLUDED

/**
  CHighlighter higlights Python syntax using simple lexical scanner.
  Base class SyntaxHighlighter is specific for text-mode IEditor implementation.
  */

#include <texteditor/syntaxhighlighter.h>
#include <texteditor/fontsettings.h>

namespace PythonEditor {

class CHighlighter : public TextEditor::SyntaxHighlighter
{
    Q_OBJECT

public:
    /**
      Passes \b{*parent} in base class.
      */
    explicit CHighlighter(TextEditor::BaseTextDocument *parent);
    virtual ~CHighlighter();

    /**
      QtCreator has own fonts&color settings. CHighlighter wants get access to
      this settings before highlightBlock() called first time.
      Settings provided by CEditorWidget.
      */
    void setFontSettings(const TextEditor::FontSettings &fs);

protected:
    /**
      Highlights single line.
      */
    virtual void highlightBlock(const QString &text);

private:
    QVector<QTextCharFormat> m_formats;
};

} // namespace PythonEditor

#endif // PYTHON_EDITOR__HIGHLIGHTER__INCLUDED
