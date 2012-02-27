#ifndef PYTHON_EDITOR__INDENTER__INCLUDED
#define PYTHON_EDITOR__INDENTER__INCLUDED

/**
  CIndenter отвечает за расстановку отступов в исходном коде на €зыке Python
  */

#include <texteditor/indenter.h>

namespace PythonEditor {

class CIndenter : public TextEditor::Indenter
{
public:
    CIndenter();
    virtual ~CIndenter();

    /**
      ¬ызывает ли символ увеличение уровн€ отступа? (как "{" в €зыке C++)
      */
    virtual bool isElectricCharacter(const QChar &ch) const;

    /**
      ¬ыравнивание блока кода. ѕод блоком обычно подразумеваетс€ одна строка.
      */
    virtual void indentBlock(QTextDocument *doc,
                             const QTextBlock &block,
                             const QChar &typedChar,
                             const TextEditor::TabSettings &tabSettings);

    /**
      ¬ыравнивание кода по заданному положению курсора.
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
