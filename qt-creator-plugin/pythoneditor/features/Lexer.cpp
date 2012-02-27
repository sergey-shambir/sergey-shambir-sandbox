
// Qt Library
#include <QtCore/QString>
#include <QtCore/QSet>
#include <QtCore/QDebug>

// Self headers
#include "../core/constants.h"
#include "Lexer.h"

using namespace PythonEditor::Constants;

namespace PythonEditor
{

////////////////////////////////////////////////////////////////////////////////
// Various word sets: keywords set, magic methods set

static QSet<QString> InitWordsSet(const char* const words[], size_t amount)
{
    QSet<QString> result;
    for (size_t index = 0; index < amount; ++index)
        result.insert(words[index]);

    return result;
}

#define INIT_SET(arr) InitWordsSet(arr, (sizeof(arr) / sizeof(const char* const)))

static const QSet<QString> KEYWORDS_SET = INIT_SET(C_PYTHON_KEYWORDS);

static const QSet<QString> MAGIC_METHODS_SET = INIT_SET(C_PYTHON_MAGIC_METHODS);

static const QSet<QString> BUILTINS_SET = INIT_SET(C_PYTHON_BUILTINS);

////////////////////////////////////////////////////////////////////////////////

CLexer::CLexer(const QChar *text, const size_t length)
    :m_text(text)
    ,m_length(length)
    ,m_readPos(0)
    ,m_state(0)
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
        return CToken(FormatedBlockEnd, m_readPos, 0);
    }
    QChar ch = GetChar();

    if (ch == '\n')
        return CToken(Format_WHITESPACE, m_readPos - 1, 1);

    if ((ch == '\\') && !IsEOF() && (Peek() == '\n'))
    {
        GetChar();
        return CToken(Format_WHITESPACE, m_readPos - 2, 2);
    }

    if ((ch == '\'') || (ch == '\"'))
        return ReadStringLiteral(ch);

    if (ch.isLetter() || (ch == '_'))
        return ReadIdentifier();

    if (ch.isDigit())
        return ReadNumber();

    if (ch == '#')
    {
        if (!IsEOF() && (Peek() == '#'))
        {
            return ReadDoxygenComment();
        }
        return ReadComment();
    }

    if (ch.isSpace())
        return ReadWhiteSpace();

    return ReadOperator();
}

/**
  Строковые литералы в Python обрамляются символами ' или "
  */
CToken CLexer::ReadStringLiteral(QChar quoteChar)
{
    const size_t initialPos = m_readPos - 1;
    while (!IsEOF() && (Peek() != quoteChar))
    {
        GetChar();
    }
    GetChar();
    return CToken(Format_STRING, initialPos, m_readPos - initialPos);
}

/**
  Идентификатор начинается с <буквы> и продолжается замыканием символов
  <буква или цифра>, причём <буква> - это символ английского алфавита
  либо поджопник: '_'.
  В Python есть 32 ключевых слова, они внесены в KEYWORDS_SET
  */
CToken CLexer::ReadIdentifier()
{
    const size_t initialPos = m_readPos - 1;
    while (!IsEOF() && (Peek().isLetterOrNumber() || (Peek() == '_')))
    {
        GetChar();
    }
    QString value(m_text + initialPos, m_readPos - initialPos);

    Format tkFormat = Format_IDENTIFIER;
    if (BUILTINS_SET.contains(value))
    {
        tkFormat = Format_TYPE;
    }
    if (MAGIC_METHODS_SET.contains(value))
    {
        tkFormat = Format_METHOD;
    }
    else if (KEYWORDS_SET.contains(value))
    {
        tkFormat = Format_KEYWORD;
    }
    return CToken(tkFormat, initialPos, m_readPos - initialPos);
}

/**
  Числа в специальных системах счисления имеют префиксы:
  0o... и 0O... - восьмиричные
  0x... и 0X... - шестнадцатиричные
  0b... и 0B... - двоичные
  остальные числа представлены в десятичной системе.
  Суффиксы l и L также допустимы.

  Числа с плавающей запятой:
    floatnumber   ::=  pointfloat | exponentfloat
    pointfloat    ::=  [intpart] fraction | intpart "."
    exponentfloat ::=  (intpart | pointfloat) exponent
    intpart       ::=  digit+
    fraction      ::=  "." digit+
    exponent      ::=  ("e" | "E") ["+" | "-"] digit+

  Комплексные числа:
    imagnumber ::=  (floatnumber | intpart) ("j" | "J")
  */
CToken CLexer::ReadNumber()
{
    const size_t initialPos = m_readPos - 1;
    while (!IsEOF() && (Peek().isDigit()))
    {
        GetChar();
    }
    return CToken(Format_NUMBER, initialPos, m_readPos - initialPos);
}

/**
  Комментарии начинаются с символа # и бывают только однострочными
  */
CToken CLexer::ReadComment()
{
    const size_t initialPos = m_readPos - 1;
    while (!IsEOF() && (Peek() != QChar('\n')))
    {
        GetChar();
    }
    return CToken(Format_COMMENT, initialPos, m_readPos - initialPos);
}

/**
  Однострочные комментарии Doxygen начинаются с ##
  */
CToken CLexer::ReadDoxygenComment()
{
    const size_t initialPos = m_readPos - 1;
    while (!IsEOF() && (Peek() != QChar('\n')))
    {
        GetChar();
    }
    return CToken(Format_DOXYGEN_COMMENT, initialPos, m_readPos - initialPos);
}

CToken CLexer::ReadWhiteSpace()
{
    const size_t initialPos = m_readPos - 1;
    while (!IsEOF() && (Peek().isSpace()) && (Peek() != QChar('\n')))
    {
        GetChar();
    }
    return CToken(Format_WHITESPACE, initialPos, m_readPos - initialPos);
}

CToken CLexer::ReadOperator()
{
    const size_t initialPos = m_readPos - 1;
    while (!IsEOF() && (Peek().isPunct()) && Peek() != '\'' && Peek() != '\"')
    {
        GetChar();
    }
    return CToken(Format_OPERATOR, initialPos, m_readPos - initialPos);
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

} // PythonEditor
