
// Self headers
#include "Lexer.h"
#include "Indenter.h"

// QtCreator platform & other plugins
#include <cpptools/cppcodeformatter.h>
#include <cpptools/cpptoolssettings.h>
#include <cpptools/cppcodestylepreferences.h>
#include <texteditor/basetextdocumentlayout.h>
#include <texteditor/tabsettings.h>

// Qt Library
#include <QtGui/QTextCursor>
#include <QtGui/QTextDocument>
#include <QtCore/QSet>
#include <QtCore/QDebug>

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

Indenter::Indenter()
{
}

Indenter::~Indenter()
{
}

////////////////////////////////////////////////////////////////////////
// Increase intendation after colon, example
// class Employee:
//    def __init__(a):
//        name = a
////////////////////////////////////////////////////////////////////////
bool Indenter::isElectricCharacter(const QChar &ch) const
{
    return (ch == ':');
}

/**
  ������������ ����� ����. ��� ������ ������ ��������������� ���� ������.
  \todo ������� �������� ����� CodeFormater, ��������,
        �� ������ � ������� CppTools::QtStyleCodeFormatter
  */
void Indenter::indentBlock(QTextDocument *doc,
                            const QTextBlock &block,
                            const QChar &typedChar,
                            const TextEditor::TabSettings &tabSettings)
{
    Q_UNUSED(doc)
    Q_UNUSED(typedChar)

    QTextBlock prevBlock = block.previous();
    if (prevBlock.isValid()) {
        const int TAB_SIZE = tabSettings.m_tabSize;
        QString prevLine = prevBlock.text();
        int indentation = tabSettings.indentationColumn(prevLine);

        if (isElectricLine(prevLine)) {
            indentation += TAB_SIZE;
        } else {
            Lexer lexer(prevLine.constData(), prevLine.length());
            for (;;)
            {
                Token tk = lexer.read();
                if (tk.format() == FormatedBlockEnd) {
                    break;
                }
                if (tk.format() == Format_KEYWORD) {
                    QString value = lexer.value(tk);
                    if (BACKSTEP_KEYWORDS_SET.contains(value)) {
                        indentation = qMax<int>(0, indentation - TAB_SIZE);
                        break;
                    }
                }
            }
        }
        tabSettings.indentLine(block, indentation);
    } else {
        // First line in whole document
        tabSettings.indentLine(block, 0);
    }
}

bool Indenter::isElectricLine(const QString &line) const
{
    size_t index = line.length() - 1;
    while ((index > 0) && line[index].isSpace()) {
        --index;
    }
    return (isElectricCharacter(line[index]));
}

} // PythonEditor
