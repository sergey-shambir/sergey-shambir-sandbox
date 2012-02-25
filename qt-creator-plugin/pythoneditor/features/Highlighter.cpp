

// Qt Library
#include <QtCore/QDebug>

// QtCreator platform & other plugins
#include <texteditor/basetextdocumentlayout.h>
#include <texteditor/basetextdocument.h>
#include <texteditor/fontsettings.h>
#include <texteditor/texteditorconstants.h>

// Self headers
#include "Lexer.h"
#include "Highlighter.h"

using namespace PythonEditor;
using namespace PythonEditor::Internal;

////////////////////////////////////////////////////////////////////////////////
// Список категорий форматов текста
QVector<QString> InitFormatCategories()
{
    QVector<QString> categories(FormatsCount);
    for (unsigned index = 0; index < FormatsCount; ++index)
    {
        switch (index)
        {
        case Format_NUMBER:
            categories[Format_NUMBER] = TextEditor::Constants::C_NUMBER;
            break;
        case Format_STRING:
            categories[Format_STRING] = TextEditor::Constants::C_STRING;
            break;
        case Format_KEYWORD:
            categories[Format_KEYWORD] = TextEditor::Constants::C_KEYWORD;
            break;
        case Format_OPERATOR:
            categories[Format_OPERATOR] = TextEditor::Constants::C_OPERATOR;
            break;
        case Format_COMMENT:
            categories[Format_COMMENT] = TextEditor::Constants::C_COMMENT;
            break;
        case Format_WHITESPACE:
            categories[Format_WHITESPACE] = TextEditor::Constants::C_VISUAL_WHITESPACE;
            break;
        case Format_IDENTIFIER:
            categories[Format_IDENTIFIER] = TextEditor::Constants::C_LOCAL;
            break;
        }
    }

    return categories;
}

static const QVector<QString> CATEGORIES = InitFormatCategories();
////////////////////////////////////////////////////////////////////////////////

CHighlighter::CHighlighter(TextEditor::BaseTextDocument *parent)
    :TextEditor::SyntaxHighlighter(parent)
{
}

CHighlighter::~CHighlighter()
{
}

void CHighlighter::SetFontSettings(const TextEditor::FontSettings &fs)
{
    m_formats = fs.toTextCharFormats(CATEGORIES);
    rehighlight();
}

/*
#define casestr(arg) case arg: result = QLatin1String( #arg ); break;

QString TkToStr(CToken const& tk)
{
    QString result;
    switch (tk.type())
    {
    casestr(Token_NUMBER)
    casestr(Token_STRING)
    casestr(Token_KEYWORD)
    casestr(Token_OPERATOR)
    casestr(Token_COMMENT)
    casestr(Token_WHITESPACE)
    casestr(Token_IDENTIFIER)
    casestr(Token_NEWLINE)
    casestr(Token_EOF_SYMBOL)
    }
    result += QLatin1String(" begin: ") + QString::number(tk.begin());
    result += QLatin1String(" length: ") + QString::number(tk.length());

    return result;
}
//*/

void CHighlighter::highlightBlock(const QString &text)
{
    int initialState = previousBlockState();
    if (initialState == -1)
    {
        initialState = 0;
    }

    CLexer lexer(text.constData(), text.size());
    lexer.SetState(initialState);

    for (;;)
    {
        CToken tk = lexer.Read();
        //qDebug() << TkToStr(tk);
        if (tk.type() == Token_EOF_SYMBOL)
            break;

        Format currFormat = Format_WHITESPACE;
        switch (tk.type())
        {
        case Token_NUMBER:
            currFormat = Format_NUMBER;
            break;
        case Token_STRING:
            currFormat = Format_STRING;
            break;
        case Token_KEYWORD:
            currFormat = Format_KEYWORD;
            break;
        case Token_OPERATOR:
            currFormat = Format_OPERATOR;
            break;
        case Token_COMMENT:
            currFormat = Format_COMMENT;
            break;
        case Token_IDENTIFIER:
            currFormat = Format_IDENTIFIER;
            break;
        case Token_WHITESPACE:
        case Token_NEWLINE:
            currFormat = Format_WHITESPACE;
            break;
        case Token_EOF_SYMBOL:
            break;
        }

        setFormat(tk.begin(), tk.length(), m_formats[currFormat]);
    }
    setCurrentBlockState(lexer.GetState());
}
