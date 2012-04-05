#ifndef PYEDITOR_FORMATTOKEN_H
#define PYEDITOR_FORMATTOKEN_H

#include <stdlib.h>

namespace PyEditor {
namespace Internal {

enum Format {
    Format_NUMBER = 0,
    Format_STRING,
    Format_KEYWORD,
    Format_TYPE,
    Format_CLASS_FIELD,
    Format_MAGIC_ATTRIB, // magic class attribute/method, like __name__, __init__
    Format_OPERATOR,
    Format_COMMENT,
    Format_DOXYGEN_COMMENT,
    Format_IDENTIFIER,
    Format_WHITESPACE,
    Format_IMPORTED_MODULE,

    Format_FormatsAmount,
    Format_EndOfBlock
};

class FormatToken
{
public:
    FormatToken() {}

    FormatToken(Format format, size_t position, size_t length)
        :_format(format)
        ,_position(position)
        ,_length(length)
    {}

    inline Format format() const { return (_format); }
    inline size_t begin() const { return (_position); }
    inline size_t end() const { return (_position + _length); }
    inline size_t length() const { return (_length); }

private:
    Format _format;
    size_t _position;
    size_t _length;
};

} // namespace Internal
} // namespace PyEditor

#endif // PYEDITOR_FORMATTOKEN_H
