#ifndef LEXER_H_4c5c373b_3564_4c8f_b064_951c8abfdf38
#define LEXER_H_4c5c373b_3564_4c8f_b064_951c8abfdf38

#include <QtCore/QChar>

namespace PythonEditor {
namespace Internal {

enum Token
{
    Token_NUMBER,
    Token_STRING,
    Token_KEYWORD,
    Token_OPERATOR,
    Token_COMMENT,
    Token_WHITESPACE,
    Token_IDENTIFIER,
    Token_NEWLINE,
    Token_EOF_SYMBOL
};

enum Format
{
    Format_NUMBER = 0,
    Format_STRING,
    Format_KEYWORD,
    Format_OPERATOR,
    Format_COMMENT,
    Format_IDENTIFIER,
    Format_WHITESPACE,

    FormatsCount
};

class CToken
{
public:
    CToken()
    {}

    CToken(Token vType, size_t vPosition, size_t vLength)
        :m_type(vType)
        ,m_position(vPosition)
        ,m_length(vLength)
    {}

    inline Token type() const { return (m_type); }
    inline size_t begin() const { return (m_position); }
    inline size_t end() const { return (m_position + m_length); }
    inline size_t length() const { return (m_length); }

private:
    Token m_type;
    size_t m_position;
    size_t m_length;
};

class CLexer
{
public:
    CLexer(const QChar* text, const size_t length);

    void SetState(int state);
    int GetState() const;
    CToken Read();

private:
    /**
      Возвращает true, если литерал заканчивается завершающей кавычкой
      */
    bool PassStringLiteral(QChar quote);

    CToken ReadStringLiteral();
    CToken ReadIdentifier();
    CToken ReadNumber();
    CToken ReadComment();
    CToken ReadWhiteSpace();
    CToken ReadOperator();

    /**
      Достигнут конец входной строки?
      */
    bool IsEOF() const;

    QChar Peek() const;
    QChar GetChar();

    const QChar* m_text;
    size_t m_length;
    size_t m_readPos;
    int m_state;
};

} // namespace Internal
} // namespace PythonEditor

#endif // LEXER_H_4c5c373b_3564_4c8f_b064_951c8abfdf38
