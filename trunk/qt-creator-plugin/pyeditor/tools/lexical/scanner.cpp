// Self headers
#include "../../pyeditorconstants.h"
#include "scanner.h"

// Qt Library
#include <QtCore/QString>
#include <QtCore/QSet>
#include <QtCore/QDebug>

namespace PyEditor {
namespace Internal {

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

static const QSet<QString> KEYWORDS_SET = INIT_SET(Constants::LIST_OF_PYTHON_KEYWORDS);
static const QSet<QString> MAGICS_SET = INIT_SET(Constants::LIST_OF_PYTHON_MAGICS);
static const QSet<QString> BUILTINS_SET = INIT_SET(Constants::LIST_OF_PYTHON_BUILTINS);

////////////////////////////////////////////////////////////////////////////////

Scanner::Scanner(const QChar *text, const size_t length)
    :_src(text, length)
    ,_state(0)
{
}

Scanner::~Scanner()
{
}

void Scanner::setState(int state)
{
    _state = state;
}

int Scanner::getState() const
{
    return _state;
}

FormatToken Scanner::read()
{
    _src.setAnchor();
    if (_src.isEnd()) {
        return FormatToken(Format_EndOfBlock, _src.anchor(), 0);
    }

    State state;
    QChar saved;
    parseState(state, saved);
    switch (state)
    {
    case State_String:
        return readStringLiteral(saved);
    case State_MultiLineString:
        return readMultiLineStringLiteral(saved);
    default:
        return onDefaultState();
    }
}

QString Scanner::value(const FormatToken &tk) const
{
    return _src.value(tk.begin(), tk.length());
}

FormatToken Scanner::onDefaultState()
{
    QChar first = _src.peek();
    _src.move();

    if ((first == '\\') && (_src.peek() == '\n')) {
        _src.move();
        return FormatToken(Format_WHITESPACE, _src.anchor(), 2);
    }

    if ((first == QChar('.')) && _src.peek().isDigit())
        return readFloatNumber();

    if ((first == '\'') || (first == '\"'))
        return readStringLiteral(first);

    if (first.isLetter() || (first == '_'))
        return readIdentifier();

    if (first.isDigit())
        return readNumber();

    if (first == '#') {
        if (_src.peek() == '#') {
            return readDoxygenComment();
        }
        return readComment();
    }

    if (first.isSpace())
        return readWhiteSpace();

    return readOperator();
}

/**
 * @brief Lexer::passEscapeCharacter
 * @return returns true if escape sequence doesn't end with newline
 */
void Scanner::checkEscapeSequence(QChar quoteChar)
{
    if (_src.peek() == '\\')
    {
        _src.move();
        QChar ch = _src.peek();
        if ((ch == '\n') || (ch.isNull()))
        {
            saveState(State_String, quoteChar);
        }
    }
}

/**
  reads single-line string literal, surrounded by ' or " quotes
  */
FormatToken Scanner::readStringLiteral(QChar quoteChar)
{
    QChar ch = _src.peek();
    if ((ch == quoteChar) && (_src.peek(1) == quoteChar)) {
        saveState(State_MultiLineString, quoteChar);
        return readMultiLineStringLiteral(quoteChar);
    }

    while ((ch != quoteChar) && !ch.isNull()) {
        checkEscapeSequence(quoteChar);
        _src.move();
        ch = _src.peek();
    }
    if (ch == quoteChar)
    {
        clearState();
    }
    _src.move();
    return FormatToken(Format_STRING, _src.anchor(), _src.length());
}

/**
  reads multi-line string literal, surrounded by ''' or """ sequencies
  */
FormatToken Scanner::readMultiLineStringLiteral(QChar quoteChar)
{
    for (;;) {
        QChar ch = _src.peek();
        if (ch.isNull()) {
            break;
        }
        if ((ch == quoteChar)
                && (_src.peek(1) == quoteChar)
                && (_src.peek(2) == quoteChar)) {
            clearState();
            _src.move();
            _src.move();
            _src.move();
            break;
        }
        _src.move();
    }

    return FormatToken(Format_STRING, _src.anchor(), _src.length());
}

/**
  reads identifier and classifies it
  */
FormatToken Scanner::readIdentifier()
{
    QChar ch = _src.peek();
    while (ch.isLetterOrNumber() || (ch == '_')) {
        _src.move();
        ch = _src.peek();
    }
    QString value = _src.value();

    Format tkFormat = Format_IDENTIFIER;
    if (value == QLatin1String("self")) {
        tkFormat = Format_CLASS_FIELD;
    } else if (BUILTINS_SET.contains(value)) {
        tkFormat = Format_TYPE;
    } else if (MAGICS_SET.contains(value)) {
        tkFormat = Format_MAGIC_ATTRIB;
    } else if (KEYWORDS_SET.contains(value)) {
        tkFormat = Format_KEYWORD;
    }
    return FormatToken(tkFormat, _src.anchor(), _src.length());
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

FormatToken Scanner::readNumber()
{
    if (!_src.isEnd())
    {
        QChar ch = _src.peek();
        if (ch.toLower() == 'b') {
            _src.move();
            while (isBinaryDigit(_src.peek())) {
                _src.move();
            }
        }
        else if (ch.toLower() == 'o') {
            _src.move();
            while (isOctalDigit(_src.peek())) {
                _src.move();
            }
        }
        else if (ch.toLower() == 'x') {
            _src.move();
            while (isHexDigit(_src.peek())) {
                _src.move();
            }
        }
        else {  // either integer or float number
            return readFloatNumber();
        }
        if (isValidIntegerSuffix(_src.peek())) {
            _src.move();
        }
    }
    return FormatToken(Format_NUMBER, _src.anchor(), _src.length());
}

FormatToken Scanner::readFloatNumber() {
    enum {
        State_INTEGER,
        State_FRACTION,
        State_EXPONENT
    } state;
    state = (_src.peek(-1) == '.') ? State_FRACTION : State_INTEGER;

    for (;;)
    {
        QChar ch = _src.peek();
        if (ch.isNull())
            break;

        if (state == State_INTEGER) {
            if (ch == '.') {
                state = State_FRACTION;
            } else if (!ch.isDigit())
                break;
        } else if (state == State_FRACTION) {
            if ((ch == 'e') || (ch == 'E')) {
                QChar next = _src.peek(1);
                QChar next2 = _src.peek(2);
                bool isExp = next.isDigit()
                        || (((next == '-') || (next == '+')) && next2.isDigit());
                if (isExp) {
                    _src.move();
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
        _src.move();
    }
    {
        QChar ch = _src.peek();
        if (((state == State_INTEGER) && (ch == 'l' || ch == 'L'))
                || (ch == 'j' || ch == 'J'))
            _src.move();
    }
    return FormatToken(Format_NUMBER, _src.anchor(), _src.length());
}

/**
  reads single-line python comment, started with "#"
  */
FormatToken Scanner::readComment()
{
    QChar ch = _src.peek();
    while ((ch != '\n') && !ch.isNull()) {
        _src.move();
        ch = _src.peek();
    }
    return FormatToken(Format_COMMENT, _src.anchor(), _src.length());
}

/**
  reads single-line python doxygen comment, started with "##"
  */
FormatToken Scanner::readDoxygenComment()
{
    QChar ch = _src.peek();
    while ((ch != '\n') && !ch.isNull()) {
        _src.move();
        ch = _src.peek();
    }
    return FormatToken(Format_DOXYGEN_COMMENT, _src.anchor(), _src.length());
}

/**
  reads whitespace
  */
FormatToken Scanner::readWhiteSpace()
{
    while (_src.peek().isSpace()) {
        _src.move();
    }
    return FormatToken(Format_WHITESPACE, _src.anchor(), _src.length());
}

/**
  reads punctuation symbols, excluding some special
  */
FormatToken Scanner::readOperator()
{
    static const QString EXCLUDED_CHARS = "\'\"_#";
    QChar ch = _src.peek();
    while (ch.isPunct() && !EXCLUDED_CHARS.contains(ch)) {
        _src.move();
        ch = _src.peek();
    }
    return FormatToken(Format_OPERATOR, _src.anchor(), _src.length());
}

void Scanner::clearState()
{
    _state = 0;
}

void Scanner::saveState(State state, QChar savedData)
{
    _state = (state << 16) | static_cast<int>(savedData.unicode());
}

void Scanner::parseState(State &state, QChar &savedData) const
{
    state = static_cast<State>(_state >> 16);
    savedData = static_cast<ushort>(_state);
}

} // namespace Internal
} // namespace PyEditor
