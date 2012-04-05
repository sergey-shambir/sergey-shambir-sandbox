#ifndef PYEDITOR_SCANNER_H
#define PYEDITOR_SCANNER_H

#include "formattoken.h"
#include "sourcecodestream.h"

namespace PyEditor {
namespace Internal {

/**
 * @brief The Scanner class - scans source code for highlighting only
 */
class Scanner
{
    Scanner(const Scanner &other);
    void operator =(const Scanner &other);

public:
    enum State {
        State_Default,
        State_String,
        State_MultiLineString
    };

    Scanner(const QChar* text, const size_t length);
    ~Scanner();

    void setState(int state);
    int getState() const;
    FormatToken read();
    QString value(const FormatToken& tk) const;

private:
    FormatToken onDefaultState();

    void checkEscapeSequence(QChar quoteChar);
    FormatToken readStringLiteral(QChar quoteChar);
    FormatToken readMultiLineStringLiteral(QChar quoteChar);
    FormatToken readIdentifier();
    FormatToken readNumber();
    FormatToken readFloatNumber();
    FormatToken readComment();
    FormatToken readDoxygenComment();
    FormatToken readWhiteSpace();
    FormatToken readOperator();

    void clearState();
    void saveState(State state, QChar savedData);
    void parseState(State &state, QChar &savedData) const;

    SourceCodeStream _src;
    int _state;
};

} // namespace Internal
} // namespace PyEditor

#endif // PYEDITOR_SCANNER_H
