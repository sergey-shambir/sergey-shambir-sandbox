#ifndef PYTHON_EDITOR__INDENTER__INCLUDED
#define PYTHON_EDITOR__INDENTER__INCLUDED

/**
  CIndenter �������� �� ����������� �������� � �������� ���� �� ����� Python
  */

#include <texteditor/indenter.h>

namespace PythonEditor {

class Indenter : public TextEditor::Indenter
{
public:
    Indenter();
    virtual ~Indenter();

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

private:
    bool isElectricLine(const QString &ch) const;
};

}
// PythonEditor

#endif // PYTHON_EDITOR__INDENTER__INCLUDED
