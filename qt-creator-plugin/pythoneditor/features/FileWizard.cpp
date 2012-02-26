
// Qt Library
#include <QtGui/QWizard>

// QtCreator platform & other plugins
#include <utils/filewizarddialog.h>
#include <texteditor/textfilewizard.h>

// Self headers
#include "../core/constants.h"
#include "FileWizard.h"

using namespace PythonEditor::Constants;

namespace PythonEditor {

static const Core::BaseFileWizardParameters GetDefaultParams()
{
    Core::BaseFileWizardParameters p(Core::IWizard::FileWizard);

    p.setId(QLatin1String(C_WIZARD_ID_PYTHON));
    p.setCategory(QLatin1String(C_WIZARD_CATEGORY_PYTHON));
    p.setDisplayCategory(QLatin1String(C_DISPLAY_CATEGORY_PYTHON));
    p.setDisplayName(
                QObject::tr("Python script"));
    p.setDescription(
                QObject::tr("Creates an empty python script with utf-8 charset"));

    return p;
}

CFileWizard::CFileWizard(QObject *parent)
    :Core::BaseFileWizard(GetDefaultParams(), parent)
{
}

CFileWizard::~CFileWizard()
{
}

QWizard* CFileWizard::createWizardDialog(QWidget *parent,
                                         const QString &defaultPath,
                                         const WizardPageList &extensionPages) const
{
    Utils::FileWizardDialog* pDialog = new Utils::FileWizardDialog(parent);
    pDialog->setWindowTitle(tr("New %1").arg(displayName()));
    setupWizard(pDialog);
    pDialog->setPath(defaultPath);
    foreach (QWizardPage *p, extensionPages)
    {
        applyExtensionPageShortTitle(pDialog, pDialog->addPage(p));
    }

    return pDialog;
}

Core::GeneratedFiles CFileWizard::generateFiles(const QWizard *dialog,
                                                QString *errorMessage) const
{
    Q_UNUSED(errorMessage)

    const Utils::FileWizardDialog* pWizard =
            qobject_cast<const Utils::FileWizardDialog*>(dialog);

    QString folder = pWizard->path();
    QString name = pWizard->fileName();

    name = Core::BaseFileWizard::buildFileName(
               folder, name, QLatin1String(".py"));
    Core::GeneratedFile file(name);
    file.setContents(QLatin1String(C_NEW_FILE_CONTENT));
    file.setAttributes(Core::GeneratedFile::OpenEditorAttribute);

    return (Core::GeneratedFiles() << file);
}

} // PythonEditor
