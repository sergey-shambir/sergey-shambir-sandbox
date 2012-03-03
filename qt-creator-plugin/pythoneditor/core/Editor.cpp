

// Qt Library
#include <QtCore/QFileInfo>

// QtCreator platform & other plugins
#include <coreplugin/icore.h>
#include <coreplugin/mimedatabase.h>
#include <texteditor/texteditorconstants.h>
#include <texteditor/texteditorsettings.h>

// Self headers
#include "constants.h"
#include "Plugin.h"
#include "EditorWidget.h"
#include "Editor.h"

using namespace PythonEditor::Constants;

namespace PythonEditor {

Editor::Editor(EditorWidget *editorWidget)
    :BaseTextEditor(editorWidget)
{
    setContext(Core::Context(C_PYTHONEDITOR_ID,
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

QString Editor::id() const
{
    return QLatin1String(C_PYTHONEDITOR_ID);
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

} // PythonEditor
