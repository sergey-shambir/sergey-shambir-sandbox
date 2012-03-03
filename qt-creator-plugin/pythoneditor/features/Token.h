#ifndef TOKEN_H_320d39e5_c970_4242_9239_61776ba9f386
#define TOKEN_H_320d39e5_c970_4242_9239_61776ba9f386

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

class Token
{
public:
    Token()
    {}

    Token(Format vFormat, size_t vPosition, size_t vLength)
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

} // PythonEditor

#endif // TOKEN_H_320d39e5_c970_4242_9239_61776ba9f386
