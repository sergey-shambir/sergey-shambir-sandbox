
// Self headers
#include "pyeditorplugin.h"
#include "pyeditorconstants.h"
#include "pyeditorfeatures.h"
#include "pyeditorwidget.h"
#include "pyeditorfactory.h"

// QtCreator platform & other plugins
#include <coreplugin/icore.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/mimedatabase.h>
#include <coreplugin/fileiconprovider.h>
#include <coreplugin/id.h>
#include <coreplugin/editormanager/editormanager.h>
#include <extensionsystem/pluginmanager.h>
#include <texteditor/texteditorconstants.h>
#include <texteditor/texteditorsettings.h>

// Qt Library
#include <QtCore/QtPlugin>
#include <QtCore/QCoreApplication>

using namespace PyEditor::Constants;

namespace PyEditor {

Plugin* Plugin::_instance = 0;

/*! Constructs the plugin. Normally plugins don't do anything in
    their constructor except for initializing their member variables. The
    actual work is done later, in the initialize() and extensionsInitialized()
    methods.
*/
Plugin::Plugin()
    :_factory(0)
    ,_actionHandler(0)
{
    _instance = this;
}

/*! Plugins are responsible for deleting objects they created on the heap, and
    to unregister objects from the plugin manager that they registered there.
*/
Plugin::~Plugin()
{
    removeObject(_factory);
    if (_actionHandler)
    {
        delete _actionHandler;
    }
    _instance = 0;
}

/*! Initializes the plugin. Returns true on success.
    Plugins want to register objects with the plugin manager here.

    @param errorMessage can be used to pass an error message to the plugin system,
       if there was any.
*/
bool Plugin::initialize(
        const QStringList &arguments, QString *errorMessage)
{
    Q_UNUSED(arguments)

    Core::ICore* pCore = Core::ICore::instance();

    if (! pCore->mimeDatabase()->addMimeTypes(
            QLatin1String(RC_PY_MIME_XML),
            errorMessage))
    {
        return false;
    }

    _factory = new EditorFactory(this);
    addObject(_factory);

    ////////////////////////////////////////////////////////////////////////////
    // Initialize editor actions handler
    ////////////////////////////////////////////////////////////////////////////
    _actionHandler = new TextEditor::TextEditorActionHandler(
                C_PYEDITOR_ID,
                TextEditor::TextEditorActionHandler::Format
                | TextEditor::TextEditorActionHandler::UnCommentSelection
                | TextEditor::TextEditorActionHandler::UnCollapseAll);
    _actionHandler->initializeActions();

    ////////////////////////////////////////////////////////////////////////////
    // Add MIME overlay icons (these icons displayed at Project dock panel)
    ////////////////////////////////////////////////////////////////////////////
    Core::FileIconProvider *iconProv = Core::FileIconProvider::instance();
    Core::MimeDatabase *mimeDB = Core::ICore::instance()->mimeDatabase();
    iconProv->registerIconOverlayForMimeType(
                  QIcon(QLatin1String(RC_PY_MIME_ICON)),
                  mimeDB->findByType(QLatin1String(C_PY_MIMETYPE))
              );

    ////////////////////////////////////////////////////////////////////////////
    // Add Python files and classes creation dialogs
    ////////////////////////////////////////////////////////////////////////////
#ifdef PYEDITOR_FILEWIZARD_H
    addAutoReleasedObject(new FileWizard(Core::ICore::instance()));
#endif
#ifdef PYEDITOR_CLASSWIZARD_H
    addAutoReleasedObject(new ClassWizard(Core::ICore::instance()));
#endif

    return true;
}

/*! Notification that all extensions that this plugin depends on have been
    initialized. The dependencies are defined in the plugins .pluginspec file.

    Normally this method is used for things that rely on other plugins to have
    added objects to the plugin manager, that implement interfaces that we're
    interested in. These objects can now be requested through the
    PluginManagerInterface.
*/
void Plugin::extensionsInitialized()
{
}

void Plugin::initializeEditor(EditorWidget *widget)
{
    instance()->_actionHandler->setupActions(widget);
    TextEditor::TextEditorSettings::instance()->initializeEditor(widget);
}

} // namespace PyEditor

Q_EXPORT_PLUGIN(PyEditor::Plugin)

