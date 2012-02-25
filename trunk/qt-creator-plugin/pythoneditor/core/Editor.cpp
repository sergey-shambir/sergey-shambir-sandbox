

// Qt Library
#include <QtCore/QFileInfo>

// QtCreator platform & other plugins
#include <coreplugin/icore.h>
#include <coreplugin/mimedatabase.h>
#include <texteditor/texteditorconstants.h>
#include <texteditor/texteditorsettings.h>

// Self headers
#include "constants.h"
#include "EditorWidget.h"
#include "Editor.h"

using namespace PythonEditor::Internal;
using namespace PythonEditor::Constants;

CEditor::CEditor(CEditorWidget *editorWidget)
    :BaseTextEditor(editorWidget)
{
    setContext(Core::Context(C_PYTHONEDITOR_ID,
                             TextEditor::Constants::C_TEXTEDITOR));
}

CEditor::~CEditor()
{
}

Core::IEditor* CEditor::duplicate(QWidget *parent)
{
    CEditorWidget* pEditor = new CEditorWidget(parent);
    pEditor->duplicateFrom(editorWidget());
    TextEditor::TextEditorSettings::instance()->initializeEditor(pEditor);

    return pEditor->editor();
}

QString CEditor::id() const
{
    return QLatin1String(C_PYTHONEDITOR_ID);
}

QString CEditor::preferredModeType() const
{
    return QString();
}

bool CEditor::open(QString *errorString,
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