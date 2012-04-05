#include "indenter.h"
#include "lexical/scanner.h"

#include <texteditor/tabsettings.h>

#include <QtCore/QSet>

namespace PyEditor {

static const char* const JUMP_KEYWORDS[] = {
    "return",
    "yield",
    "break",
    "continue",
    "raise"
};

static const char* const BACKSTEP_KEYWORDS[] = {
    "else",
    "elif",
    "except",
    "finally"
};

#define InitSpecialSet(words) \
    InitSpecialSet_impl(words, sizeof(words) / sizeof(const char* const))

static QSet<QString> InitSpecialSet_impl(const char* const words[], size_t amount)
{
    QSet<QString> ret;
    for (size_t index = 0; index < amount; ++index)
        ret.insert(words[index]);
    return ret;
}

static const QSet<QString> JUMP_STATEMENTS_SET = InitSpecialSet(JUMP_KEYWORDS);
static const QSet<QString> BACKSTEP_KEYWORDS_SET = InitSpecialSet(BACKSTEP_KEYWORDS);

////////////////////////////////////////////////////////////////////////////////

// FIXME: Despite the fact that 4 spaces per indent required by PEP8,
// much better is use python-specific IDE settings
static const int TAB_SIZE = 4;

Indenter::Indenter()
{
}

Indenter::~Indenter()
{
}

/**
 * @brief isElectricCharacter Does given character increase indentation level?
 * @param ch Any value
 * @return True if character increases indentation level at next line
 */
bool Indenter::isElectricCharacter(const QChar &ch) const
{
    return (ch == ':');
}

/**
 * @brief Indenter::indentBlock Indents one block (usually one line) of code
 * @param block
 * @param typedChar
 * @param tabSettings An IDE tabulation settings
 *
 * Usually this method called once when you begin new line of code by pressing
 * Enter. If Indenter reimplements indent() method, than indentBlock() may be
 * called in other cases.
 */
void Indenter::indentBlock(QTextDocument */*doc*/,
                           const QTextBlock &block,
                           const QChar &/*typedChar*/,
                           const TextEditor::TabSettings &settings)
{
    QTextBlock previousBlock = block.previous();
    if (previousBlock.isValid()) {
        QString previousLine = previousBlock.text();
        int indentation = settings.indentationColumn(previousLine);

        if (isElectricLine(previousLine)) {
            indentation += TAB_SIZE;
        }
        parsePreviousLine(settings, previousLine, previousBlock, indentation);

        settings.indentLine(block, indentation);
    } else {
        // First line in whole document
        settings.indentLine(block, 0);
    }
}

//! Returns true if electric character is last non-space character at given string
bool Indenter::isElectricLine(const QString &line) const
{
    size_t index = line.length() - 1;
    while ((index > 0) && line[index].isSpace()) {
        --index;
    }
    return (isElectricCharacter(line[index]));
}

void Indenter::parsePreviousLine(
        const TextEditor::TabSettings &settings,
        const QString &previousLine,
        const QTextBlock &previousBlock,
        int &indentation) const
{
    // TODO: replace this dirty code with true AST-based indentation
    Internal::Scanner sc(previousLine.constData(), previousLine.length());
    for (;;)
    {
        Internal::FormatToken tk = sc.read();
        if (tk.format() == Internal::Format_KEYWORD) {
            QString value = sc.value(tk);

            if (JUMP_STATEMENTS_SET.contains(value)) {
                indentation = qMax<int>(0, indentation - TAB_SIZE);
            } else if (BACKSTEP_KEYWORDS_SET.contains(value)) {
                indentation = qMax<int>(0, indentation - TAB_SIZE);
                settings.reindentLine(previousBlock, -TAB_SIZE);
            }
        }
        if (tk.format() != Internal::Format_WHITESPACE) {
            break;
        }
    }
}

} // namespace PyEditor
