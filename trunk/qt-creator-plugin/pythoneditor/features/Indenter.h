#ifndef PYTHON_EDITOR__INDENTER__INCLUDED
#define PYTHON_EDITOR__INDENTER__INCLUDED

/**
  CIndenter �������� �� ����������� �������� � �������� ���� �� ����� Python
  */

#include <texteditor/indenter.h>

namespace PythonEditor {

class CIndenter : public TextEditor::Indenter
{
public:
    CIndenter();
    virtual ~CIndenter();

    /**
      �������� �� ������ ���������� ������ �������? (��� "{" � ����� C++)
      */
    virtual bool isElectricCharacter(const QChar &ch) const;

    /**
      ������������ ����� ����. ��� ������ ������ ��������������� ���� ������.
      */
    virtual void indentBlock(QTextDocument *doc,
                             const QTextBlock &block,
                             const QChar &typedChar,
                             const TextEditor::TabSettings &tabSettings);

    /**
      ������������ ���� �� ��������� ��������� �������.
      */
    virtual void indent(QTextDocument *doc,
                        const QTextCursor &cursor,
                        const QChar &typedChar,
                        const TextEditor::TabSettings &tabSettings);

    static const int INDENTATION_STEP = 4;
};

}
// PythonEditor

#endif // PYTHON_EDITOR__INDENTER__INCLUDED
