/**
  \class EditorWidget
  Graphical representation and parent widget for PyEditor::Editor class.
  Represents editor as plain text editor.
  */

// QtCreator platform & other plugins
#include <texteditor/fontsettings.h>
#include <texteditor/texteditorconstants.h>
#include <texteditor/basetextdocument.h>
#include <texteditor/indenter.h>
#include <texteditor/autocompleter.h>

// Self headers
#include "pyeditorfeatures.h"
#include "pyeditor.h"
#include "pyeditorwidget.h"

namespace PyEditor {

EditorWidget::EditorWidget(QWidget *parent)
    :TextEditor::BaseTextEditorWidget(parent)
{
    _commentDefinition.setMultiLineStart("");
    _commentDefinition.setMultiLineEnd("");
    _commentDefinition.setSingleLine(QLatin1String("#"));

    setParenthesesMatchingEnabled(true);
    setMarksVisible(true);
    setCodeFoldingSupported(true);

#ifdef PYEDITOR_INDENTER_H
    setIndenter(new Indenter());
#endif

#ifdef PYEDITOR_AUTOCOMPLETER_H
    setAutoCompleter(new Autocompleter());
#endif

#ifdef PYEDITOR_HIGHLIGHTER_H
    new Highlighter(baseTextDocument());
#endif
}

EditorWidget::~EditorWidget()
{
}

/**
  Comments or uncomments selection using Python commenting syntax
  */
void EditorWidget::unCommentSelection()
{
    Utils::unCommentSelection(this, _commentDefinition);
}

/**
  Handles common IDE fonts&colors settings
  (Tools -> Options -> Text editor -> Fonts and colors)
  */
void EditorWidget::setFontSettings(const TextEditor::FontSettings &fs)
{
    TextEditor::BaseTextEditorWidget::setFontSettings(fs);

#ifdef PYEDITOR_HIGHLIGHTER_H
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

} // namespace PyEditor
