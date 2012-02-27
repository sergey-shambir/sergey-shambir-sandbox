
// Qt Library
#include <QtCore/QDebug>

// QtCreator platform & other plugins
#include <coreplugin/icore.h>
#include <coreplugin/editormanager/editormanager.h>
#include <texteditor/texteditorsettings.h>

// Self headers
#include "constants.h"
#include "EditorWidget.h"
#include "Plugin.h"
#include "EditorFactory.h"

using namespace PythonEditor::Constants;

namespace PythonEditor {

CEditorFactory::CEditorFactory(QObject *parent)
    : Core::IEditorFactory(parent)
{
    m_mimeTypes << QLatin1String(C_PYTHON_MIMETYPE);
}

Core::Id CEditorFactory::id() const
{
    return C_PYTHONEDITOR_ID;
}

QString CEditorFactory::displayName() const
{
    return "stub for CEditorFactory::displayName()";
}

Core::IFile *CEditorFactory::open(const QString &fileName)
{
    Core::IEditor *iface = Core::EditorManager::instance()->openEditor(fileName, id());
    if (!iface) {
        qWarning() << "CEditorFactory::open: openEditor failed for " << fileName;
        return 0;
    }
    return iface->file();
}

Core::IEditor *CEditorFactory::createEditor(QWidget *parent)
{
    CEditorWidget* widget = new CEditorWidget(parent);
    CPlugin::initializeEditor(widget);

    return widget->editor();
}

QStringList CEditorFactory::mimeTypes() const
{
    return m_mimeTypes;
}

} // PythonEditor
