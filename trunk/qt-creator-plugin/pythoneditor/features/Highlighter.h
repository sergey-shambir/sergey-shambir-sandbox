#ifndef HIGHLIGHTER_H_44bbc180_b243_4bd1_bcb7_cba5701262a5
#define HIGHLIGHTER_H_44bbc180_b243_4bd1_bcb7_cba5701262a5

#include <texteditor/syntaxhighlighter.h>
#include <texteditor/fontsettings.h>

namespace PythonEditor {
namespace Internal {

class CHighlighter
  : public TextEditor::SyntaxHighlighter
{
    Q_OBJECT

public:
    explicit CHighlighter(TextEditor::BaseTextDocument *parent);
    virtual ~CHighlighter();

    void SetFontSettings(const TextEditor::FontSettings &fs);

protected:
    virtual void highlightBlock(const QString &text);

private:
    QVector<QTextCharFormat> m_formats;
};

} // namespace Internal
} // namespace PythonEditor

#endif // HIGHLIGHTER_H_44bbc180_b243_4bd1_bcb7_cba5701262a5
