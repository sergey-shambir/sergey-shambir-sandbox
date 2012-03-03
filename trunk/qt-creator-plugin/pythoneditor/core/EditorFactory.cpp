
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

EditorFactory::EditorFactory(QObject *parent)
    : Core::IEditorFactory(parent)
{
    m_mimeTypes << QLatin1String(C_PY_MIMETYPE);
}

Core::Id EditorFactory::id() const
{
    return C_PYTHONEDITOR_ID;
}

QString EditorFactory::displayName() const
{
    return "stub for CEditorFactory::displayName()";
}

Core::IFile *EditorFactory::open(const QString &fileName)
{
    Core::IEditor *iface = Core::EditorManager::instance()->openEditor(fileName, id());
    if (!iface) {
        qWarning() << "CEditorFactory::open: openEditor failed for " << fileName;
        return 0;
    }
    return iface->file();
}

Core::IEditor *EditorFactory::createEditor(QWidget *parent)
{
    EditorWidget* widget = new EditorWidget(parent);
    Plugin::initializeEditor(widget);

    return widget->editor();
}

QStringList EditorFactory::mimeTypes() const
{
    return m_mimeTypes;
}

} // PythonEditor
