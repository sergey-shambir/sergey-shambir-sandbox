
// Qt Library
#include <QtCore/QString>
#include <QtCore/QSet>

#include "Lexer.h"

namespace PythonEditor
{
namespace Internal
{

////////////////////////////////////////////////////////////////////////////////
// Utility

static QSet<QString> InitKeywordsSet()
{
    const char* const KEYWORDS[] = {
        "and",
        "as",
        "assert",
        "break",
        "class",
        "continue",
        "def",
        "del",
        "elif",
        "else",
        "except",
        "exec",
        "finally",
        "for",
        "from",
        "global",
        "if",
        "import",
        "in",
        "is",
        "lambda",
        "not",
        "or",
        "pass",
        "print",
        "raise",
        "return",
        "tru",
        "while",
        "with",
        "yield",
        "None"
    };
    QSet<QString> result;
    size_t amount = (sizeof(KEYWORDS) / sizeof(const char* const));
    for (size_t index = 0; index < amount; ++index)
        result.insert(KEYWORDS[index]);

    return result;
}

static const QSet<QString> KEYWORDS_SET = InitKeywordsSet();

enum State {
    State_NORMAL,
    State_COMMENT,
    State_STRING_LITERAL
};

////////////////////////////////////////////////////////////////////////////////

CLexer::CLexer(const QChar *text, const size_t length)
    :m_text(text)
    ,m_length(length)
    ,m_readPos(0)
    ,m_state(State_NORMAL)
{
}

void CLexer::SetState(int state)
{
    m_state = state;
}

int CLexer::GetState() const
{
    return m_state;
}

CToken CLexer::Read()
{
    if (IsEOF())
    {
        return CToken(Token_EOF_SYMBOL, m_readPos, 0);
    }
    QChar ch = GetChar();

    if (ch == QLatin1Char('\n'))
    {
        return CToken(Token_NEWLINE, m_readPos - 1, 1);
    }
    if ((ch == QLatin1Char('\\'))
             && !IsEOF() && (Peek() == QLatin1Char('\n')))
    {
        GetChar();
        return CToken(Token_NEWLINE, m_readPos - 2, 2);
    }
    if (ch == QLatin1Char('\''))
        return ReadStringLiteral();
    if (ch.isLetter())
        return ReadIdentifier();
    if (ch.isDigit())
        return ReadNumber();
    if (ch == QLatin1Char('#'))
        return ReadComment();
    if (ch.isSpace())
        return ReadWhiteSpace();
    return ReadOperator();
}

CToken CLexer::ReadStringLiteral()
{
    size_t initialPos = m_readPos - 1;
    QChar ch = m_text[m_readPos - 1];
    while (!IsEOF() && (Peek() != ch))
    {
        GetChar();
    }
    return CToken(Token_STRING, initialPos, m_readPos - initialPos);
}

CToken CLexer::ReadIdentifier()
{
    size_t initialPos = m_readPos - 1;
    while (!IsEOF() && (Peek().isLetterOrNumber()))
    {
        GetChar();
    }
    QString value(m_text + initialPos, m_readPos - initialPos);
    if (KEYWORDS_SET.contains(value))
    {
        return CToken(Token_KEYWORD, initialPos, m_readPos - initialPos);
    }
    return CToken(Token_IDENTIFIER, initialPos, m_readPos - initialPos);
}

CToken CLexer::ReadNumber()
{
    size_t initialPos = m_readPos - 1;
    while (!IsEOF() && (Peek().isDigit()))
    {
        GetChar();
    }
    return CToken(Token_NUMBER, initialPos, m_readPos - initialPos);
}

CToken CLexer::ReadComment()
{
    size_t initialPos = m_readPos - 1;
    while (!IsEOF() && (Peek() != QLatin1Char('\n')))
    {
        GetChar();
    }
    return CToken(Token_COMMENT, initialPos, m_readPos - initialPos);
}

CToken CLexer::ReadWhiteSpace()
{
    size_t initialPos = m_readPos - 1;
    while (!IsEOF() && (Peek().isSpace()) && (Peek() != QLatin1Char('\n')))
    {
        GetChar();
    }
    return CToken(Token_WHITESPACE, initialPos, m_readPos - initialPos);
}

CToken CLexer::ReadOperator()
{
    size_t initialPos = m_readPos - 1;
    while (!IsEOF() && (Peek().isPunct()))
    {
        GetChar();
    }
    return CToken(Token_OPERATOR, initialPos, m_readPos - initialPos);
}

bool CLexer::IsEOF() const
{
    return (m_readPos >= m_length);
}

QChar CLexer::Peek() const
{
    return m_text[m_readPos];
}

QChar CLexer::GetChar()
{
    return m_text[m_readPos++];
}

} // Internal
} // PythonEditor
