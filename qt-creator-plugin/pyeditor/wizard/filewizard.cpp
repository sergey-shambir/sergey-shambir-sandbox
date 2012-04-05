/**
 * @brief The FileWizard class - adds wizard for creating new Python source file
 */

#include "filewizard.h"
#include "../pyeditorconstants.h"

// QtCreator platform & other plugins
#include <utils/filewizarddialog.h>
#include <texteditor/textfilewizard.h>

// Qt Library
#include <QtGui/QWizard>

namespace PyEditor {

/**
 * @brief GetDefaultParams
 * @return Default parameters for menu item "Files&Classes->Python->Python file"
 */
static const Core::BaseFileWizardParameters GetDefaultParams()
{
    Core::BaseFileWizardParameters p(Core::IWizard::FileWizard);

    p.setId(QLatin1String(Constants::C_PY_SOURCE_WIZARD_ID));
    p.setCategory(QLatin1String(Constants::C_PY_WIZARD_CATEGORY));
    p.setDisplayCategory(QLatin1String(Constants::C_PY_DISPLAY_CATEGORY));
    p.setDisplayName(
                QObject::tr(Constants::EN_PY_SOURCE_DISPLAY_NAME));
    p.setDescription(
                QObject::tr(Constants::EN_PY_SOURCE_DESCRIPTION));

    return p;
}

/**
 * @brief FileWizard::FileWizard Initialize wizard and add new option
 * to "New..." dialog.
 * @param parent
 */
FileWizard::FileWizard(QObject *parent)
    :Core::BaseFileWizard(GetDefaultParams(), parent)
{
}

FileWizard::~FileWizard()
{
}

/**
 * @brief FileWizard::createWizardDialog
 * @param parent
 * @param params
 * @return
 */
QWizard* FileWizard::createWizardDialog(QWidget *parent,
                                        const Core::WizardDialogParameters &params) const
{
    Utils::FileWizardDialog* pDialog = new Utils::FileWizardDialog(parent);
    pDialog->setWindowTitle(tr("New %1").arg(displayName()));
    setupWizard(pDialog);
    pDialog->setPath(params.defaultPath());
    foreach (QWizardPage *p, params.extensionPages())
    {
        applyExtensionPageShortTitle(pDialog, pDialog->addPage(p));
    }

    return pDialog;
}

Core::GeneratedFiles FileWizard::generateFiles(const QWizard *dialog,
                                                QString *errorMessage) const
{
    Q_UNUSED(errorMessage)

    const Utils::FileWizardDialog* pWizard =
            qobject_cast<const Utils::FileWizardDialog*>(dialog);

    QString folder = pWizard->path();
    QString name = pWizard->fileName();

    name = Core::BaseFileWizard::buildFileName(
                folder, name, QLatin1String(Constants::C_PY_EXTENSION));
    Core::GeneratedFile file(name);
    file.setContents(QLatin1String(Constants::C_PY_SOURCE_CONTENT));
    file.setAttributes(Core::GeneratedFile::OpenEditorAttribute);

    return (Core::GeneratedFiles() << file);
}

} // namespace PyEditor
