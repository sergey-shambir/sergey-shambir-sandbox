
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
#include <texteditor/textfilewizard.h>

// Self headers
#include "constants.h"
#include "../features/FileWizard.h"
#include "EditorFactory.h"
#include "Plugin.h"

using namespace PythonEditor::Internal;
using namespace PythonEditor::Constants;

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
            QLatin1String(":/pythoneditor/pythoneditor.mimetypes.xml"),
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
                  QIcon(QLatin1String(C_PYTHON_ICON)),
                  mimeDB->findByType(QLatin1String(C_PYTHON_MIMETYPE))
              );

    ////////////////////////////////////////////////////////////////////////////
    // Добавляем диалог создания файла
    ////////////////////////////////////////////////////////////////////////////
    Core::BaseFileWizardParameters params(Core::IWizard::FileWizard);
    params.setId(QLatin1String(C_WIZARD_ID_PYTHON));
    params.setCategory(QLatin1String(C_WIZARD_CATEGORY_PYTHON));
    params.setDisplayCategory(QLatin1String(C_DISPLAY_CATEGORY_PYTHON));
    params.setDisplayName(QObject::tr("Python script"));
    params.setIcon(QIcon(QLatin1String(C_PYTHON_ICON)));
    params.setDescription
        (QObject::tr("Creates an empty python script with utf-8 charset"));

    addAutoReleasedObject(new CFileWizard(params, Core::ICore::instance()));

    return true;
}
void CPlugin::extensionsInitialized()
{
}

Q_EXPORT_PLUGIN(PythonEditor::Internal::CPlugin)
