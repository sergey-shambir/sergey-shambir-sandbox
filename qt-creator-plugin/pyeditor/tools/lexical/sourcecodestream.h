#ifndef PYEDITOR_SOURCECODESTREAM_H
#define PYEDITOR_SOURCECODESTREAM_H

#include <QtCore/QString>

namespace PyEditor {
namespace Internal {

class SourceCodeStream
{
public:
    SourceCodeStream(const QChar* text, size_t length)
        :_text(text)
        ,_textLength(length)
        ,_position(0)
        ,_markedPosition(0)
    {}

    inline void setAnchor()
    {
        _markedPosition = _position;
    }

    inline void move()
    {
        ++_position;
    }

    inline size_t length() const
    {
        return (_position - _markedPosition);
    }

    inline size_t anchor() const
    {
        return (_markedPosition);
    }

    inline bool isEnd() const
    {
        return (_position >= _textLength);
    }

    inline QChar peek(int offset = 0) const
    {
        size_t pos = static_cast<size_t>(_position + offset);
        if (pos >= _textLength) {
            return '\0';
        }
        return _text[pos];
    }

    inline QString value() const
    {
        const QChar* start = _text + _markedPosition;
        return QString(start, length());
    }

    inline QString value(size_t begin, size_t length) const
    {
        return QString(_text + begin, length);
    }

private:
    const QChar* _text;
    size_t _textLength;
    size_t _position;
    size_t _markedPosition;
};

} // namespace Internal
} // namespace PyEditor

#endif // PYEDITOR_SOURCECODESTREAM_H
