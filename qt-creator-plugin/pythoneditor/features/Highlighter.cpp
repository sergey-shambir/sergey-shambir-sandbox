
// QtCreator platform & other plugins
#include <texteditor/basetextdocumentlayout.h>
#include <texteditor/basetextdocument.h>
#include <texteditor/fontsettings.h>
#include <texteditor/texteditorconstants.h>

// Self headers
#include "Lexer.h"
#include "Highlighter.h"

namespace PythonEditor {

////////////////////////////////////////////////////////////////////////////////
// List of available text highlighting formats
QVector<QString> initFormatCategories()
{
    QVector<QString> categories(FormatsCount);
    categories[Format_NUMBER] = TextEditor::Constants::C_NUMBER;
    categories[Format_STRING] = TextEditor::Constants::C_STRING;
    categories[Format_KEYWORD] = TextEditor::Constants::C_KEYWORD;
    categories[Format_TYPE] = TextEditor::Constants::C_TYPE;
    categories[Format_CLASS_FIELD] = TextEditor::Constants::C_FIELD;
    categories[Format_METHOD] = TextEditor::Constants::C_VIRTUAL_METHOD;
    categories[Format_OPERATOR] = TextEditor::Constants::C_OPERATOR;
    categories[Format_COMMENT] = TextEditor::Constants::C_COMMENT;
    categories[Format_DOXYGEN_COMMENT] = TextEditor::Constants::C_DOXYGEN_COMMENT;
    categories[Format_WHITESPACE] = TextEditor::Constants::C_VISUAL_WHITESPACE;
    categories[Format_IDENTIFIER] = TextEditor::Constants::C_TEXT;

    return categories;
}

static const QVector<QString> FORMAT_CATEGORIES = initFormatCategories();
////////////////////////////////////////////////////////////////////////////////

CHighlighter::CHighlighter(TextEditor::BaseTextDocument *parent)
    :TextEditor::SyntaxHighlighter(parent)
{
}

CHighlighter::~CHighlighter()
{
}

void CHighlighter::setFontSettings(const TextEditor::FontSettings &fs)
{
    m_formats = fs.toTextCharFormats(FORMAT_CATEGORIES);
    rehighlight();
}

void CHighlighter::highlightBlock(const QString &text)
{
    int initialState = previousBlockState();
    if (initialState == -1)
    {
        initialState = 0;
    }

    CLexer lexer(text.constData(), text.size());
    lexer.setState(initialState);

    CToken tk;
    while ((tk = lexer.read()).format() != FormatedBlockEnd)
    {
        setFormat(tk.begin(), tk.length(), m_formats[tk.format()]);
    }
    setCurrentBlockState(lexer.getState());
}

} // PythonEditor
