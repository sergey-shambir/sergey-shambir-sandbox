
// Qt Library
#include <QtGui/QTextCursor>
#include <QtGui/QTextDocument>
#include <QtCore/QDebug>

// QtCreator platform & other plugins
#include <cpptools/cppcodeformatter.h>
#include <cpptools/cpptoolssettings.h>
#include <cpptools/cppcodestylepreferences.h>
#include <texteditor/basetextdocumentlayout.h>
#include <texteditor/tabsettings.h>

// Self headers
#include "Lexer.h"
#include "Indenter.h"

namespace PythonEditor {

CIndenter::CIndenter()
{
}

CIndenter::~CIndenter()
{
}

bool CIndenter::isElectricCharacter(const QChar &ch) const
{
    return (ch == ':');
}

/**
  Выравнивание блока кода. Под блоком обычно подразумевается одна строка.
  \todo следует выделить класс CodeFormater, возможно,
        по образу и подобию CppTools::QtStyleCodeFormatter
  */
void CIndenter::indentBlock(QTextDocument *doc,
                            const QTextBlock &block,
                            const QChar &typedChar,
                            const TextEditor::TabSettings &tabSettings)
{
    Q_UNUSED(doc)
    Q_UNUSED(typedChar)

    QTextBlock previous = block.previous();
    int indentDepth;
    if (previous.isValid())
    {
        QString text = previous.text();
        indentDepth = tabSettings.indentationColumn(text);
        size_t index = text.length() - 1;
        while ((index > 0) && text[index].isSpace())
            --index;

        if (text[index] == ':')
        {
            indentDepth += 2;
        }
        else
        {
            CLexer lexer(text.constData(), text.length());
            CToken tk;
            do
            {
                tk = lexer.Read();
                if (tk.format() == Format_KEYWORD)
                {
                    QString value = text.mid(tk.begin(), tk.length());
                    if (value == "return")
                    {
                        indentDepth = qMax(0, indentDepth - 2);
                    }
                }
            }
            while (tk.format() != FormatedBlockEnd);
        }
    }
    else
    {
        indentDepth = 0;
    }

    tabSettings.indentLine(block, indentDepth);
}

/**
  Выравнивание кода по заданному положению курсора.
  \todo добавить обработку ветки if (cursor.hasSelection())
 */
void CIndenter::indent(QTextDocument *doc,
                       const QTextCursor &cursor,
                       const QChar &typedChar,
                       const TextEditor::TabSettings &tabSettings)
{
    if (!cursor.hasSelection())
    {
        indentBlock(doc, cursor.block(), typedChar, tabSettings);
    }
    else
    {
#ifdef USE_CPP_ENGINE
        QTextBlock block = doc->findBlock(cursor.selectionStart());
        const QTextBlock end = doc->findBlock(cursor.selectionEnd()).next();

        // TODO: do something with it
        CppTools::QtStyleCodeFormatter codeFormatter(tabSettings,
                  CppTools::CppToolsSettings::instance()->cppCodeStyle()->codeStyleSettings());
        codeFormatter.updateStateUntil(block);

        QTextCursor tc = cursor;
        tc.beginEditBlock();
        do
        {
            int indent;
            int padding;
            codeFormatter.indentFor(block, &indent, &padding);
            tabSettings.indentLine(block, indent + padding, padding);
            codeFormatter.updateLineStateChange(block);
            block = block.next();
        }
        while (block.isValid() && block != end);
        tc.endEditBlock();
#endif
    }
}

} // PythonEditor
