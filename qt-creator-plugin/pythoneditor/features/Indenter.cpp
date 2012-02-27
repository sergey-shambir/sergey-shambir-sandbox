
// Qt Library
#include <QtGui/QTextCursor>
#include <QtGui/QTextDocument>
#include <QtCore/QSet>

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

////////////////////////////////////////////////////////////////////////////////
// Set of keywords those decrease indent level, example
// def sum(a, b):
//     return a + b
// def another():

static QSet<QString> InitBackstepSet()
{
    static const char* const WORDS[] = {
        "return",
        "break",
        "continue",
        "raise"
    };
    size_t amount = sizeof(WORDS) / sizeof(const char* const);
    QSet<QString> result;
    for (size_t index = 0; index < amount; ++index)
        result.insert(WORDS[index]);

    return result;
}

static const QSet<QString> BACKSTEP_KEYWORDS_SET = InitBackstepSet();

////////////////////////////////////////////////////////////////////////////////

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
    if (previous.isValid()) {
        QString text = previous.text();
        indentDepth = tabSettings.indentationColumn(text);
        size_t index = text.length() - 1;
        while ((index > 0) && text[index].isSpace())
            --index;

        ////////////////////////////////////////////////////////////////////////
        // Increase intendation after colon, example
        // class Employee:
        //    def __init__(a):
        //        l = 1
        ////////////////////////////////////////////////////////////////////////
        if (text[index] == ':') {
            indentDepth += INDENTATION_STEP;
        } else {
            CLexer lexer(text.constData(), text.length());
            CToken tk = lexer.read();
            while (tk.format() != FormatedBlockEnd) {
                if (tk.format() == Format_KEYWORD) {
                    QString value = text.mid(tk.begin(), tk.length());
                    if (BACKSTEP_KEYWORDS_SET.contains(value)) {
                        indentDepth = qMax(0, indentDepth - INDENTATION_STEP);
                        break;
                    }
                }
                tk = lexer.read();
            }
        }
    } else {
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
    if (!cursor.hasSelection()) {
        indentBlock(doc, cursor.block(), typedChar, tabSettings);
    } else {
#ifdef USE_CPP_ENGINE
        QTextBlock block = doc->findBlock(cursor.selectionStart());
        const QTextBlock end = doc->findBlock(cursor.selectionEnd()).next();

        // TODO: do something with it
        CppTools::QtStyleCodeFormatter codeFormatter(tabSettings,
                  CppTools::CppToolsSettings::instance()->cppCodeStyle()->codeStyleSettings());
        codeFormatter.updateStateUntil(block);

        QTextCursor tc = cursor;
        tc.beginEditBlock();
        do {
            int indent;
            int padding;
            codeFormatter.indentFor(block, &indent, &padding);
            tabSettings.indentLine(block, indent + padding, padding);
            codeFormatter.updateLineStateChange(block);
            block = block.next();
        } while (block.isValid() && block != end);
        tc.endEditBlock();
#endif
    }
}

} // PythonEditor
