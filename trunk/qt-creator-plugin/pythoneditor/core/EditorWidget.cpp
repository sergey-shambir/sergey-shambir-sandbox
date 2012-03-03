
// QtCreator platform & other plugins
#include <texteditor/fontsettings.h>
#include <texteditor/texteditorconstants.h>
#include <texteditor/basetextdocument.h>
#include <texteditor/indenter.h>
#include <texteditor/autocompleter.h>

// Self headers
#include "features.h"
#include "Editor.h"
#include "EditorWidget.h"

namespace PythonEditor {

EditorWidget::EditorWidget(QWidget *parent)
    :TextEditor::BaseTextEditorWidget(parent)
{
    m_commentDefinition.setMultiLineStart(QLatin1String("\"\"\""));
    m_commentDefinition.setMultiLineEnd(QLatin1String("\"\"\""));
    m_commentDefinition.setSingleLine(QLatin1String("#"));

    setParenthesesMatchingEnabled(true);
    setMarksVisible(true);
    setCodeFoldingSupported(true);

#ifdef PYTHON_EDITOR__INDENTER__INCLUDED
    setIndenter(new Indenter());
#endif

#ifdef PYTHON_EDITOR__HIGHLIGHTER__INCLUDED
    new Highlighter(baseTextDocument());
#endif
}

EditorWidget::~EditorWidget()
{
}

void EditorWidget::unCommentSelection()
{
    Utils::unCommentSelection(this, m_commentDefinition);
}

void EditorWidget::setFontSettings(const TextEditor::FontSettings &fs)
{
    TextEditor::BaseTextEditorWidget::setFontSettings(fs);

#ifdef PYTHON_EDITOR__HIGHLIGHTER__INCLUDED
    Highlighter *highlighter =
            qobject_cast<Highlighter*>(baseTextDocument()->syntaxHighlighter());
    if (highlighter)
    {
        highlighter->setFontSettings(fs);
    }
#endif
}

TextEditor::BaseTextEditor* EditorWidget::createEditor()
{
    return new Editor(this);
}

} // PythonEditor
