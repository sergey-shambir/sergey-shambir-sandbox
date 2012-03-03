#ifndef SOURCE_STREAM_H_d5631977_5e75_4ac8_b6b5_d1ba92936d11
#define SOURCE_STREAM_H_d5631977_5e75_4ac8_b6b5_d1ba92936d11

#include <QtCore/QString>
#include "Token.h"

namespace PythonEditor {
class SourceStream
{
public:
    SourceStream(const QChar* text, size_t length)
        :m_text(text)
        ,m_textLength(length)
        ,m_position(0)
        ,m_markedPosition(0)
    {}

    inline void setAnchor()
    {
        m_markedPosition = m_position;
    }

    inline void move()
    {
        ++m_position;
    }

    inline size_t length() const
    {
        return (m_position - m_markedPosition);
    }

    inline size_t anchor() const
    {
        return (m_markedPosition);
    }

    inline bool isEnd() const
    {
        return (m_position >= m_textLength);
    }

    inline QChar peek(int offset = 0) const
    {
        size_t pos = static_cast<size_t>(m_position + offset);
        if (pos >= m_textLength) {
            return '\0';
        }
        return m_text[pos];
    }

    inline QString value() const
    {
        const QChar* start = m_text + m_markedPosition;
        return QString(start, length());
    }

    inline QString value(const Token& tk) const
    {
        return QString(m_text + tk.begin(), tk.length());
    }

private:
    const QChar* m_text;
    size_t m_textLength;
    size_t m_position;
    size_t m_markedPosition;
};

} // PythonEditor

#endif // SOURCE_STREAM_H_d5631977_5e75_4ac8_b6b5_d1ba92936d11
