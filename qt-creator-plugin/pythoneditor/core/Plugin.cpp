
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
#include "EditorFactory.h"
#include "Plugin.h"

using namespace PythonEditor::Constants;

namespace PythonEditor {

CPlugin* CPlugin::m_instance = 0;

CPlugin::CPlugin()
{
    m_instance = this;
}

CPlugin::~CPlugin()
{
    removeObject(m_factory);
    m_instance = 0;
}

bool CPlugin::initialize(
        const QStringList &arguments, QString *errorMessage)
{
    Q_UNUSED(arguments)

    Core::ICore* pCore = Core::ICore::instance();

    if (! pCore->mimeDatabase()->addMimeTypes(
            QLatin1String(RC_PYTHON_MIME_XML),
            errorMessage))
    {
        return false;
    }

    m_factory = new CEditorFactory(this);
    addObject(m_factory);

    ////////////////////////////////////////////////////////////////////////////
    // Добавляем MIME-иконки файлов
    ////////////////////////////////////////////////////////////////////////////
    Core::FileIconProvider *iconProv = Core::FileIconProvider::instance();
    Core::MimeDatabase *mimeDB = Core::ICore::instance()->mimeDatabase();
    iconProv->registerIconOverlayForMimeType(
                  QIcon(QLatin1String(RC_PYTHON_MIME_ICON)),
                  mimeDB->findByType(QLatin1String(C_PYTHON_MIMETYPE))
              );

    ////////////////////////////////////////////////////////////////////////////
    // Добавляем диалог создания файла
    ////////////////////////////////////////////////////////////////////////////
#ifdef PYTHON_EDITOR__FILE_WIZARD__INCLUDED
    addAutoReleasedObject(new CFileWizard(Core::ICore::instance()));
#endif

    return true;
}
void CPlugin::extensionsInitialized()
{
}

} // PythonEditor

Q_EXPORT_PLUGIN(PythonEditor::CPlugin)

