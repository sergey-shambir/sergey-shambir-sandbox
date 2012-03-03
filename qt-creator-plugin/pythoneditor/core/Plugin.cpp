
// Qt Library
#include <QtCore/QtPlugin>
#include <QtCore/QCoreApplication>

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

// Self headers
#include "constants.h"
#include "features.h"
#include "EditorWidget.h"
#include "EditorFactory.h"
#include "Plugin.h"

using namespace PythonEditor::Constants;

namespace PythonEditor {

Plugin* Plugin::m_instance = 0;

Plugin::Plugin()
    :m_factory(0)
    ,m_actionHandler(0)
{
    m_instance = this;
}

Plugin::~Plugin()
{
    removeObject(m_factory);
    if (m_actionHandler)
    {
        delete m_actionHandler;
    }
    m_instance = 0;
}

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

    m_factory = new EditorFactory(this);
    addObject(m_factory);

    ////////////////////////////////////////////////////////////////////////////
    // Initialize editor actions handler
    ////////////////////////////////////////////////////////////////////////////
    m_actionHandler = new TextEditor::TextEditorActionHandler(
                C_PYTHONEDITOR_ID,
                TextEditor::TextEditorActionHandler::Format
                | TextEditor::TextEditorActionHandler::UnCommentSelection
                | TextEditor::TextEditorActionHandler::UnCollapseAll);
    m_actionHandler->initializeActions();

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
    // Add Python source file creation dialog
    ////////////////////////////////////////////////////////////////////////////
#ifdef PYTHON_EDITOR__FILE_WIZARD__INCLUDED
    addAutoReleasedObject(new FileWizard(Core::ICore::instance()));
#endif

    return true;
}

void Plugin::extensionsInitialized()
{
}

void Plugin::initializeEditor(EditorWidget *widget)
{
    instance()->m_actionHandler->setupActions(widget);
    TextEditor::TextEditorSettings::instance()->initializeEditor(widget);
}

} // PythonEditor

Q_EXPORT_PLUGIN(PythonEditor::Plugin)

