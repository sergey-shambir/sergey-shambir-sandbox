
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

    p.setId(QLatin1String(C_PY_SOURCE_WIZARD_ID));
    p.setCategory(QLatin1String(C_PY_WIZARD_CATEGORY));
    p.setDisplayCategory(QLatin1String(C_PY_DISPLAY_CATEGORY));
    p.setDisplayName(
                QObject::tr(EN_PY_SOURCE_DISPLAY_NAME));
    p.setDescription(
                QObject::tr(EN_PY_SOURCE_DESCRIPTION));

    return p;
}

FileWizard::FileWizard(QObject *parent)
    :Core::BaseFileWizard(GetDefaultParams(), parent)
{
}

FileWizard::~FileWizard()
{
}

QWizard* FileWizard::createWizardDialog(QWidget *parent,
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

Core::GeneratedFiles FileWizard::generateFiles(const QWizard *dialog,
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
    file.setContents(QLatin1String(C_PY_SOURCE_CONTENT));
    file.setAttributes(Core::GeneratedFile::OpenEditorAttribute);

    return (Core::GeneratedFiles() << file);
}

} // PythonEditor
