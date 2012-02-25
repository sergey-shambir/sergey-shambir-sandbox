
// QtCreator platform & other plugins
#include <texteditor/fontsettings.h>
#include <texteditor/texteditorconstants.h>
#include <texteditor/basetextdocument.h>

// Self headers
#include "../features/Highlighter.h"
#include "Editor.h"
#include "EditorWidget.h"

namespace PythonEditor
{
namespace Internal
{

static const unsigned UPDATE_DOCUMENT_DEFAULT_INTERVAL = 150u;

CEditorWidget::CEditorWidget(QWidget *parent)
    :TextEditor::BaseTextEditorWidget(parent)
{
    // Допустимы только однострочные комментарии, начинающиеся с "#"
    m_commentDefinition.clearCommentStyles();
    m_commentDefinition.setSingleLine(QLatin1String("#"));

    new CHighlighter(baseTextDocument());
   // m_updateDocumentTimer = new QTimer(this);
   // m_updateDocumentTimer->setInterval(UPDATE_DOCUMENT_DEFAULT_INTERVAL);
   // m_updateDocumentTimer->setSingleShot(true);
   // connect(m_updateDocumentTimer, SIGNAL(timeout()), this, SLOT(updateDocumentNow()));
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

    CHighlighter *highlighter =
            qobject_cast<CHighlighter*>(baseTextDocument()->syntaxHighlighter());
    if (highlighter)
    {
        highlighter->SetFontSettings(fs);
    }
}

TextEditor::BaseTextEditor* CEditorWidget::createEditor()
{
    return new CEditor(this);
}

} // PythonEditor
} // Interval
