#ifndef PYTHON_EDITOR__HIGHLIGHTER__INCLUDED
#define PYTHON_EDITOR__HIGHLIGHTER__INCLUDED

/**
  CHighlighter �������� �� ��������� ���������� ����� Python.
  ��������� ��������� �������, ��� ������ ���� - ���� ������ ��������� ����.
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
      � QtCreator ���� ���� ��������� ���������, CHighlighter ������ ��������
      �������� � ��� ������ �� ������� ������������� highlightBlock();.
      ���� ����� ���������� �� TextEditor::BaseTextEditorWidget::setFontSettings();
      */
    void SetFontSettings(const TextEditor::FontSettings &fs);

protected:
    /**
      ��������� ����� ������ ����, ���������� ����������
      */
    virtual void highlightBlock(const QString &text);

private:
    QVector<QTextCharFormat> m_formats;
};

} // namespace PythonEditor

#endif // PYTHON_EDITOR__HIGHLIGHTER__INCLUDED
