#ifndef LEXER_H_e562cd2e_3d8f_4a12_b44d_3333e4551cb2
#define LEXER_H_e562cd2e_3d8f_4a12_b44d_3333e4551cb2

#include <QtCore/QChar>

namespace PythonEditor {

enum Format
{
    Format_NUMBER = 0,
    Format_STRING,
    Format_KEYWORD,
    Format_TYPE,
    Format_CLASS_FIELD,
    Format_METHOD,
    Format_OPERATOR,
    Format_COMMENT,
    Format_DOXYGEN_COMMENT,
    Format_IDENTIFIER,
    Format_WHITESPACE,

    FormatsCount,

    FormatedBlockEnd
};

class CToken
{
public:
    CToken()
    {}

    CToken(Format vFormat, size_t vPosition, size_t vLength)
        :m_format(vFormat)
        ,m_position(vPosition)
        ,m_length(vLength)
    {}

    inline Format format() const { return (m_format); }
    inline size_t begin() const { return (m_position); }
    inline size_t end() const { return (m_position + m_length); }
    inline size_t length() const { return (m_length); }

private:
    Format m_format;
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
    CToken ReadStringLiteral(QChar quoteChar);
    CToken ReadIdentifier();
    CToken ReadNumber();
    CToken ReadComment();
    CToken ReadDoxygenComment();
    CToken ReadWhiteSpace();
    CToken ReadOperator();

    /**
      Достигнут конец входной строки?
      */
    bool IsEOF() const;

    /**
      Просмотр вперёд
      */
    QChar Peek() const;
    QChar GetChar();

    const QChar* m_text;
    size_t m_length;
    size_t m_readPos;
    int m_state;
};

} // namespace PythonEditor

#endif // LEXER_H_e562cd2e_3d8f_4a12_b44d_3333e4551cb2
