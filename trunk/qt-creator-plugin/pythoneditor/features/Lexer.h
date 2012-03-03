#ifndef LEXER_H_e562cd2e_3d8f_4a12_b44d_3333e4551cb2
#define LEXER_H_e562cd2e_3d8f_4a12_b44d_3333e4551cb2

#include "SourceStream.h"
#include "Token.h"

namespace PythonEditor {

class Lexer
{
public:
    Lexer(const QChar* text, const size_t length);

    void setState(int state);
    int getState() const;
    Token read();
    QString value(const Token& tk) const;

private:
    Token readStringLiteral(QChar quoteChar);
    Token readMultiLineStringLiteral();
    Token readIdentifier();
    Token readNumber();
    Token readFloatNumber();
    Token readComment();
    Token readDoxygenComment();
    Token readWhiteSpace();
    Token readOperator();

    SourceStream m_src;
    int m_state;
};

} // namespace PythonEditor

#endif // LEXER_H_e562cd2e_3d8f_4a12_b44d_3333e4551cb2
