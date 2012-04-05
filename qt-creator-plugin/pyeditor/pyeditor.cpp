/**
  \class PyEditor::Editor implements interface Core::IEditor
  This editor makes possible to edit Python source files
  */

// Qt Library
#include <QtCore/QFileInfo>

// QtCreator platform & other plugins
#include <coreplugin/icore.h>
#include <coreplugin/mimedatabase.h>
#include <texteditor/texteditorconstants.h>
#include <texteditor/texteditorsettings.h>

// Self headers
#include "pyeditorconstants.h"
#include "pyeditorplugin.h"
#include "pyeditorwidget.h"
#include "pyeditor.h"

namespace PyEditor {

Editor::Editor(EditorWidget *editorWidget)
    :BaseTextEditor(editorWidget)
{
    setContext(Core::Context(Constants::C_PYEDITOR_ID,
                             TextEditor::Constants::C_TEXTEDITOR));
}

Editor::~Editor()
{
}

Core::IEditor* Editor::duplicate(QWidget *parent)
{
    EditorWidget* widget = new EditorWidget(parent);
    widget->duplicateFrom(editorWidget());
    Plugin::initializeEditor(widget);

    return widget->editor();
}

Core::Id Editor::id() const
{
    return Core::Id(Constants::C_PYEDITOR_ID);
}

QString Editor::preferredModeType() const
{
    return QString();
}

bool Editor::open(QString *errorString,
                   const QString &fileName,
                   const QString &realFileName)
{
    Core::MimeType mimeType;
    Core::MimeDatabase* mimeDB = Core::ICore::instance()->mimeDatabase();

    mimeType = mimeDB->findByFile(QFileInfo(fileName));
    editorWidget()->setMimeType(mimeType.type());

    bool status = TextEditor::BaseTextEditor::open(errorString,
                                                   fileName,
                                                   realFileName);
    return status;
}

} // namespace PyEditor
