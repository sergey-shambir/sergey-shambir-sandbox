#ifndef PYEDITOR_HIGHLIGHTER_H
#define PYEDITOR_HIGHLIGHTER_H

#include <QtCore/QMap>

#include <texteditor/syntaxhighlighter.h>
#include <texteditor/fontsettings.h>
#include "../pyeditorglobal.h"

namespace PyEditor {

namespace Internal { class Scanner; }

class PYEDITOR_EXPORT Highlighter : public TextEditor::SyntaxHighlighter
{
    Q_OBJECT
public:
    explicit Highlighter(TextEditor::BaseTextDocument *parent = 0);
    virtual ~Highlighter();

    void setFontSettings(const TextEditor::FontSettings &fs);

protected:
    virtual void highlightBlock(const QString &text);

private:
    int highlightLine(const QString &text, int initialState);
    void highlightImport(Internal::Scanner &scanner);
    QVector<QTextCharFormat> _formats;
    QMap<int, QString> _userTypes;
};

} // namespace PyEditor

#endif // PYEDITOR_HIGHLIGHTER_H
