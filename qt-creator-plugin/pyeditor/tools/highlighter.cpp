/**
 * @brief The Highlighter class pre-highlights Python source using simple scanner.
 *
 * Highlighter doesn't highlight user types (classes and enumerations), syntax
 * and semantic errors, unnecessary code, etc. It's implements only
 * basic highlight mechanism.
 *
 * Main highlight procedure is highlightBlock().
 */

// Self headers
#include "highlighter.h"
#include "lexical/scanner.h"

// QtCreator platform & other plugins
#include <texteditor/basetextdocumentlayout.h>
#include <texteditor/basetextdocument.h>
#include <texteditor/fontsettings.h>
#include <texteditor/texteditorconstants.h>

// Qt Library
#include <QtCore/QDebug>

namespace PyEditor {

using namespace PyEditor::Internal;

////////////////////////////////////////////////////////////////////////////////
// List of available text highlighting formats
QVector<QString> initFormatCategories()
{
    QVector<QString> categories(Format_FormatsAmount);
    categories[Format_NUMBER] = TextEditor::Constants::C_NUMBER;
    categories[Format_STRING] = TextEditor::Constants::C_STRING;
    categories[Format_KEYWORD] = TextEditor::Constants::C_KEYWORD;
    categories[Format_TYPE] = TextEditor::Constants::C_TYPE;
    categories[Format_CLASS_FIELD] = TextEditor::Constants::C_FIELD;
    categories[Format_MAGIC_ATTRIB] = TextEditor::Constants::C_STATIC;
    categories[Format_OPERATOR] = TextEditor::Constants::C_OPERATOR;
    categories[Format_COMMENT] = TextEditor::Constants::C_COMMENT;
    categories[Format_DOXYGEN_COMMENT] = TextEditor::Constants::C_DOXYGEN_COMMENT;
    categories[Format_WHITESPACE] = TextEditor::Constants::C_VISUAL_WHITESPACE;
    categories[Format_IDENTIFIER] = TextEditor::Constants::C_TEXT;
    categories[Format_IMPORTED_MODULE] = TextEditor::Constants::C_JS_IMPORT_VAR;

    return categories;
}

static const QVector<QString> FORMAT_CATEGORIES = initFormatCategories();
////////////////////////////////////////////////////////////////////////////////

Highlighter::Highlighter(TextEditor::BaseTextDocument *parent) :
    TextEditor::SyntaxHighlighter(parent)
{
}

Highlighter::~Highlighter()
{
}

/**
  QtCreator has own fonts&color settings. Highlighter wants get access to
  this settings before highlightBlock() called first time.
  Settings provided by PyEditor::EditorWidget class.
  */
void Highlighter::setFontSettings(const TextEditor::FontSettings &fs)
{
    _formats = fs.toTextCharFormats(FORMAT_CATEGORIES);
    rehighlight();
}

enum HState
{
    HState_NORMAL,
    HState_ON_CLASS,
    HState_AFTER_CLASS
};

/**
 * @brief Highlighter::highlightBlock highlights single line of Python code
 * @param text is single line without EOLN symbol. Access to all block data
 * can be obtained through inherited currentBlock() method.
 *
 * This method receives state (int number) from previously highlighted block,
 * scans block using received state and sets initial highlighting for current
 * block. At the end, it saves internal state in current block.
 */
void Highlighter::highlightBlock(const QString &text)
{
    int initialState = previousBlockState();
    if (initialState == -1)
    {
        initialState = 0;
    }
    setCurrentBlockState(highlightLine(text, initialState));
}

/**
 * Returns true if this keyword is acceptable at start of import line
 */
static inline
bool isImportKeyword(const QString &keyword)
{
    return (keyword == QLatin1String("import")
            || keyword == QLatin1String("from"));
}

/**
 * @brief Highlighter::highlight_impl
 * @param text - source code to highlight
 * @param initialState - initial state of scanner, retrieved from previous block
 * @return final state of scanner, should be saved with current block
 */
int Highlighter::highlightLine(const QString &text, int initialState)
{
    Scanner scanner(text.constData(), text.size());
    scanner.setState(initialState);

    HState hstate = HState_NORMAL;

    int blockNo = currentBlock().blockNumber();
    QMap<int, QString>::iterator it = _userTypes.find(blockNo);
    if (it != _userTypes.end())
        _userTypes.erase(it);

    FormatToken tk;
    bool hasOnlyWhitespace = true;
    while ((tk = scanner.read()).format() != Format_EndOfBlock)
    {
        Format format = tk.format();
        switch (hstate)
        {
        default: // HState_NORMAL
            if (format == Format_KEYWORD) {
                QString value = scanner.value(tk);
                if (value == "class") {
                    hstate = HState_ON_CLASS;
                } else if (isImportKeyword(value) && hasOnlyWhitespace) {
                    setFormat(tk.begin(), tk.length(), _formats[format]);
                    highlightImport(scanner);
                    return scanner.getState();
                }
            }
            if (format == Format_IDENTIFIER) {
                QString value = scanner.value(tk);
                QMap<int, QString>::const_iterator it;
                for (it = _userTypes.begin(); it != _userTypes.end(); ++it) {
                    if (value == it.value()) {
                        format = Format_TYPE;
                        break;
                    }
                }
            }
            break;

        case HState_ON_CLASS:
            if (format == Format_WHITESPACE)
                hstate = HState_AFTER_CLASS;
            else
                hstate = HState_NORMAL;
            break;

        case HState_AFTER_CLASS:
            hstate = HState_NORMAL;
            if (format == Format_IDENTIFIER) {
                _userTypes[blockNo] = scanner.value(tk);
                format = Format_TYPE;
            }
            break;
        }
        setFormat(tk.begin(), tk.length(), _formats[format]);
        if (format != Format_WHITESPACE)
            hasOnlyWhitespace = false;
    }
    return scanner.getState();
}

void Highlighter::highlightImport(Scanner &scanner)
{
    FormatToken tk;
    while ((tk = scanner.read()).format() != Format_EndOfBlock)
    {
        Format format = tk.format();
        if (tk.format() == Format_IDENTIFIER)
            format = Format_IMPORTED_MODULE;
        setFormat(tk.begin(), tk.length(), _formats[format]);
    }
}

} // namespace PyEditor
