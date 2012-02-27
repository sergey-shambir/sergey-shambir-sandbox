
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

static QSet<QString> initWordsSet(const char* const words[], size_t amount)
{
    QSet<QString> result;
    for (size_t index = 0; index < amount; ++index)
        result.insert(words[index]);

    return result;
}

#define INIT_SET(arr) initWordsSet(arr, (sizeof(arr) / sizeof(const char* const)))

static const QSet<QString> KEYWORDS_SET = INIT_SET(C_PYTHON_KEYWORDS);
static const QSet<QString> MAGIC_METHODS_SET = INIT_SET(C_PYTHON_MAGIC_METHODS);
static const QSet<QString> BUILTINS_SET = INIT_SET(C_PYTHON_BUILTINS);

////////////////////////////////////////////////////////////////////////////////

CLexer::CLexer(const QChar *text, const size_t length)
    :m_src(text, length)
    ,m_state(0)
{
}

void CLexer::setState(int state)
{
    m_state = state;
}

int CLexer::getState() const
{
    return m_state;
}

CToken CLexer::read()
{
    m_src.setAnchor();
    if (m_src.isEnd()) {
        return CToken(FormatedBlockEnd, m_src.anchor(), 0);
    }
    QChar first = m_src.peek();
    m_src.move();

    if ((first == '\\') && (m_src.peek() == '\n')) {
        m_src.move();
        return CToken(Format_WHITESPACE, m_src.anchor(), 2);
    }

    //if ((first == '.') && peek().isDigit())
    //    return readFloatNumber();

    if ((first == '\'') || (first == '\"'))
        return readStringLiteral(first);

    if (first.isLetter() || (first == '_'))
        return readIdentifier();

    if (first.isDigit())
        return readNumber();

    if (first == '#') {
        if (m_src.peek() == '#') {
            return readDoxygenComment();
        }
        return readComment();
    }

    if (first.isSpace())
        return readWhiteSpace();

    return readOperator();
}

/**
  reads single-line string literal, surrounded by ' or " quotes
  */
CToken CLexer::readStringLiteral(QChar quoteChar)
{
    QChar ch = m_src.peek();
    while ((ch != quoteChar) && !ch.isNull()) {
        m_src.move();
        ch = m_src.peek();
    }
    m_src.move();
    return CToken(Format_STRING, m_src.anchor(), m_src.length());
}

/**
  reads identifier and classifies it
  */
CToken CLexer::readIdentifier()
{
    QChar ch = m_src.peek();
    while (ch.isLetterOrNumber() || (ch == '_')) {
        m_src.move();
        ch = m_src.peek();
    }
    QString value = m_src.value();

    Format tkFormat = Format_IDENTIFIER;
    if (value == QLatin1String("self")) {
        tkFormat = Format_CLASS_FIELD;
    }
    else if (BUILTINS_SET.contains(value)) {
        tkFormat = Format_TYPE;
    }
    else if (MAGIC_METHODS_SET.contains(value)) {
        tkFormat = Format_METHOD;
    }
    else if (KEYWORDS_SET.contains(value)) {
        tkFormat = Format_KEYWORD;
    }
    return CToken(tkFormat, m_src.anchor(), m_src.length());
}

inline static bool isHexDigit(QChar ch)
{
    return (ch.isDigit()
            || ((ch >= 'a' && ch <= 'f'))
            || ((ch >= 'A') && (ch <= 'F')));
}

inline static bool isOctalDigit(QChar ch)
{
    return (ch.isDigit() && (ch != '8') && (ch != '9'));
}

inline static bool isBinaryDigit(QChar ch)
{
    return ((ch == '0') || (ch == '1'));
}

inline static bool isValidIntegerSuffix(QChar ch)
{
    return ((ch == 'l') || (ch == 'L'));
}

inline static bool isValidComplexSuffix(QChar ch)
{
    return ((ch == 'j') || (ch == 'J'));
}

CToken CLexer::readNumber()
{
    while (m_src.peek().isDigit()) {
        m_src.move();
    }
    /*
    if (!isEOF())
    {
        QChar ch = peek();
        if (ch.toLower() == 'b') {
            getChar();
            while (isBinaryDigit(peek())) {
                getChar();
            }
        }
        if (ch.toLower() == 'o') {
            getChar();
            while (isOctalDigit(peek())) {
                getChar();
            }
        }
        if (ch.toLower() == 'x') {
            getChar();
            while (isHexDigit(peek())) {
                getChar();
            }
        }
        else {  // either integer or float number
            getChar();
            while (peek().isDigit()) {
                getChar();
            }
            //return readFloatNumber();
        }
       // if (isValidIntegerSuffix(peek())) {
        //    getChar();
        //}
    }
    //*/
    return CToken(Format_NUMBER, m_src.anchor(), m_src.length());
}

CToken CLexer::readFloatNumber() {
    /*
    enum {
        State_INTEGER,
        State_FRACTION,
        State_EXPONENT
    } state;
    state = (m_text[initialPos] != '.') ? State_INTEGER : State_FRACTION;

    for (;;)
    {
        if (isEOF())
            break;
        QChar ch = peek();
        if (state == State_INTEGER) {
            if (ch == '.') {
                state = State_FRACTION;
            } else if (!ch.isDigit())
                break;
        } else if (state == State_FRACTION) {
            if ((ch == 'e') || (ch == 'E')) {
                ch = peek(1);
                if (ch == '-' || '+') {
                    getChar();
                    state = State_EXPONENT;
                }
            } else if (!ch.isDigit())
                break;
        } else {
            if (!ch.isDigit())
                break;
        }
        getChar();
    }
    */
    return CToken(Format_NUMBER, m_src.anchor(), m_src.length());
}

/**
  reads single-line python comment, started with "#"
  */
CToken CLexer::readComment()
{
    QChar ch = m_src.peek();
    while ((ch != '\n') && !ch.isNull()) {
        m_src.move();
        ch = m_src.peek();
    }
    return CToken(Format_COMMENT, m_src.anchor(), m_src.length());
}

/**
  reads single-line python doxygen comment, started with "##"
  */
CToken CLexer::readDoxygenComment()
{
    QChar ch = m_src.peek();
    while ((ch != '\n') && !ch.isNull()) {
        m_src.move();
        ch = m_src.peek();
    }
    return CToken(Format_DOXYGEN_COMMENT, m_src.anchor(), m_src.length());
}

/**
  reads whitespace
  */
CToken CLexer::readWhiteSpace()
{
    while (m_src.peek().isSpace()) {
        m_src.move();
    }
    return CToken(Format_WHITESPACE, m_src.anchor(), m_src.length());
}

/**
  reads punctuation symbols, excluding quotes (' and ")
  */
CToken CLexer::readOperator()
{
    QChar ch = m_src.peek();
    while (ch.isPunct() && (ch != '\'') && (ch != '\"')) {
        m_src.move();
        ch = m_src.peek();
    }
    return CToken(Format_OPERATOR, m_src.anchor(), m_src.length());
}

} // PythonEditor
