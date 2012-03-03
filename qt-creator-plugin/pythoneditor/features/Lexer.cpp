
// Self headers
#include "../core/constants.h"
#include "Lexer.h"

// Qt Library
#include <QtCore/QString>
#include <QtCore/QSet>
#include <QtCore/QDebug>

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

static const QSet<QString> KEYWORDS_SET = INIT_SET(LIST_OF_PYTHON_KEYWORDS);
static const QSet<QString> MAGIC_METHODS_SET = INIT_SET(LIST_OF_PYTHON_MAGICS);
static const QSet<QString> BUILTINS_SET = INIT_SET(LIST_OF_PYTHON_BUILTINS);

////////////////////////////////////////////////////////////////////////////////

Lexer::Lexer(const QChar *text, const size_t length)
    :m_src(text, length)
    ,m_state(0)
{
}

void Lexer::setState(int state)
{
    m_state = state;
}

int Lexer::getState() const
{
    return m_state;
}

Token Lexer::read()
{
    m_src.setAnchor();
    if (m_src.isEnd()) {
        return Token(FormatedBlockEnd, m_src.anchor(), 0);
    }

    if (m_state != 0) {
        return readMultiLineStringLiteral();
    }

    QChar first = m_src.peek();
    m_src.move();

    if ((first == '\\') && (m_src.peek() == '\n')) {
        m_src.move();
        return Token(Format_WHITESPACE, m_src.anchor(), 2);
    }

    if ((first == QChar('.')) && m_src.peek().isDigit())
        return readFloatNumber();

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

QString Lexer::value(const Token &tk) const
{
    return m_src.value(tk);
}

/**
  reads single-line string literal, surrounded by ' or " quotes
  */
Token Lexer::readStringLiteral(QChar quoteChar)
{
    QChar ch = m_src.peek();
    if ((ch == quoteChar) && (m_src.peek(1) == quoteChar)) {
        m_state = quoteChar.unicode();
        readMultiLineStringLiteral();
    }

    while ((ch != quoteChar) && !ch.isNull()) {
        m_src.move();
        ch = m_src.peek();
    }
    m_src.move();
    return Token(Format_STRING, m_src.anchor(), m_src.length());
}

/**
  reads multi-line string literal, surrounded by ''' or """ sequencies
  */
Token Lexer::readMultiLineStringLiteral()
{
    QChar quoteChar = static_cast<QChar>(m_state);
    for (;;) {
        QChar ch = m_src.peek();
        if (ch.isNull()) {
            break;
        }
        if ((ch == quoteChar)
                && (m_src.peek(1) == quoteChar)
                && (m_src.peek(2) == quoteChar)) {
            m_state = 0;
            m_src.move();
            m_src.move();
            m_src.move();
            break;
        }
        m_src.move();
    }

    return Token(Format_STRING, m_src.anchor(), m_src.length());
}

/**
  reads identifier and classifies it
  */
Token Lexer::readIdentifier()
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
    return Token(tkFormat, m_src.anchor(), m_src.length());
}

inline static bool isHexDigit(QChar ch)
{
    return (ch.isDigit()
            || ((ch >= 'a') && (ch <= 'f'))
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

Token Lexer::readNumber()
{
    if (!m_src.isEnd())
    {
        QChar ch = m_src.peek();
        if (ch.toLower() == 'b') {
            m_src.move();
            while (isBinaryDigit(m_src.peek())) {
                m_src.move();
            }
        }
        else if (ch.toLower() == 'o') {
            m_src.move();
            while (isOctalDigit(m_src.peek())) {
                m_src.move();
            }
        }
        else if (ch.toLower() == 'x') {
            m_src.move();
            while (isHexDigit(m_src.peek())) {
                m_src.move();
            }
        }
        else {  // either integer or float number
           // while (m_src.peek().isDigit()) {
            //    m_src.move();
           // }
            return readFloatNumber();
        }
        if (isValidIntegerSuffix(m_src.peek())) {
            m_src.move();
        }
    }
    return Token(Format_NUMBER, m_src.anchor(), m_src.length());
}

Token Lexer::readFloatNumber() {
    enum {
        State_INTEGER,
        State_FRACTION,
        State_EXPONENT
    } state;
    state = (m_src.peek(-1) == '.') ? State_FRACTION : State_INTEGER;

    for (;;)
    {
        QChar ch = m_src.peek();
        if (ch.isNull())
            break;

        if (state == State_INTEGER) {
            if (ch == '.') {
                state = State_FRACTION;
            } else if (!ch.isDigit())
                break;
        } else if (state == State_FRACTION) {
            if ((ch == 'e') || (ch == 'E')) {
                QChar next = m_src.peek(1);
                QChar next2 = m_src.peek(2);
                bool isExp = next.isDigit()
                        || (((next == '-') || (next == '+')) && next2.isDigit());
                if (isExp) {
                    m_src.move();
                    state = State_EXPONENT;
                } else {
                    break;
                }
            } else if (!ch.isDigit())
                break;
        } else {
            if (!ch.isDigit())
                break;
        }
        m_src.move();
    }
    return Token(Format_NUMBER, m_src.anchor(), m_src.length());
}

/**
  reads single-line python comment, started with "#"
  */
Token Lexer::readComment()
{
    QChar ch = m_src.peek();
    while ((ch != '\n') && !ch.isNull()) {
        m_src.move();
        ch = m_src.peek();
    }
    return Token(Format_COMMENT, m_src.anchor(), m_src.length());
}

/**
  reads single-line python doxygen comment, started with "##"
  */
Token Lexer::readDoxygenComment()
{
    QChar ch = m_src.peek();
    while ((ch != '\n') && !ch.isNull()) {
        m_src.move();
        ch = m_src.peek();
    }
    return Token(Format_DOXYGEN_COMMENT, m_src.anchor(), m_src.length());
}

/**
  reads whitespace
  */
Token Lexer::readWhiteSpace()
{
    while (m_src.peek().isSpace()) {
        m_src.move();
    }
    return Token(Format_WHITESPACE, m_src.anchor(), m_src.length());
}

/**
  reads punctuation symbols, excluding quotes (' and ")
  */
Token Lexer::readOperator()
{
    QChar ch = m_src.peek();
    while (ch.isPunct() && (ch != '\'') && (ch != '\"')) {
        m_src.move();
        ch = m_src.peek();
    }
    return Token(Format_OPERATOR, m_src.anchor(), m_src.length());
}

} // PythonEditor
