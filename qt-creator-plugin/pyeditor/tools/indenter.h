#ifndef PYEDITOR_INDENTER_H
#define PYEDITOR_INDENTER_H

#include <texteditor/indenter.h>
#include "../pyeditorglobal.h"

namespace PyEditor {

class PYEDITOR_EXPORT Indenter : public TextEditor::Indenter
{
public:
    Indenter();
    virtual ~Indenter();

    virtual bool isElectricCharacter(const QChar &ch) const;
    virtual void indentBlock(
            QTextDocument *doc,
            const QTextBlock &block,
            const QChar &typedChar,
            const TextEditor::TabSettings &settings);

protected:
    bool isElectricLine(const QString &line) const;
    void parsePreviousLine(
            const TextEditor::TabSettings &settings,
            const QString &previousLine,
            const QTextBlock &previousBlock,
            int &indentation) const;
};

} // namespace PyEditor

#endif // PYEDITOR_INDENTER_H
