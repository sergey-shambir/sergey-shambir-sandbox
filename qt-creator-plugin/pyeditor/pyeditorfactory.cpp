
// Qt Library
#include <QtCore/QDebug>

// QtCreator platform & other plugins
#include <coreplugin/icore.h>
#include <coreplugin/editormanager/editormanager.h>
#include <texteditor/texteditorsettings.h>

// Self headers
#include "pyeditorconstants.h"
#include "pyeditorwidget.h"
#include "pyeditorplugin.h"
#include "pyeditorfactory.h"

namespace PyEditor {

EditorFactory::EditorFactory(QObject *parent)
    : Core::IEditorFactory(parent)
{
    _mimeTypes << QLatin1String(Constants::C_PY_MIMETYPE);
}

Core::Id EditorFactory::id() const
{
    return Constants::C_PYEDITOR_ID;
}

QString EditorFactory::displayName() const
{
    return tr(Constants::C_EDITOR_DISPLAY_NAME);
}

Core::IDocument *EditorFactory::open(const QString &fileName)
{
    Core::IEditor *iface = Core::EditorManager::instance()->openEditor(fileName, id());
    if (!iface) {
        qWarning() << "CEditorFactory::open: openEditor failed for " << fileName;
        return 0;
    }
    return iface->document();
}

Core::IEditor *EditorFactory::createEditor(QWidget *parent)
{
    EditorWidget* widget = new EditorWidget(parent);
    Plugin::initializeEditor(widget);

    return widget->editor();
}

QStringList EditorFactory::mimeTypes() const
{
    return _mimeTypes;
}

} // namespace PyEditor
