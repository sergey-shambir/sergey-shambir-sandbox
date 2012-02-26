
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

CEditorWidget::CEditorWidget(QWidget *parent)
    :TextEditor::BaseTextEditorWidget(parent)
{
    m_commentDefinition.setMultiLineStart(QLatin1String("\"\"\""));
    m_commentDefinition.setMultiLineEnd(QLatin1String("\"\"\""));
    m_commentDefinition.setSingleLine(QLatin1String("#"));

#ifdef PYTHON_EDITOR__INDENTER__INCLUDED
    setIndenter(new CIndenter());
#endif

#ifdef PYTHON_EDITOR__HIGHLIGHTER__INCLUDED
    new CHighlighter(baseTextDocument());
#endif
}

CEditorWidget::~CEditorWidget()
{
}

void CEditorWidget::unCommentSelection()
{
    Utils::unCommentSelection(this, m_commentDefinition);
}

void CEditorWidget::setFontSettings(const TextEditor::FontSettings &fs)
{
    TextEditor::BaseTextEditorWidget::setFontSettings(fs);

#ifdef PYTHON_EDITOR__HIGHLIGHTER__INCLUDED
    CHighlighter *highlighter =
            qobject_cast<CHighlighter*>(baseTextDocument()->syntaxHighlighter());
    if (highlighter)
    {
        highlighter->SetFontSettings(fs);
    }
#endif
}

TextEditor::BaseTextEditor* CEditorWidget::createEditor()
{
    return new CEditor(this);
}

} // PythonEditor
