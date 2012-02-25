
// Qt Library
#include <QtGui/QWizard>

// QtCreator platform & other plugins
#include <utils/filewizarddialog.h>

// Self headers
#include "../core/constants.h"
#include "FileWizard.h"

using namespace PythonEditor;
using namespace PythonEditor::Internal;
using namespace PythonEditor::Constants;

CFileWizard::CFileWizard(const Parameters &params, QObject *parent)
    :Core::BaseFileWizard(params, parent)
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
    QString path = pWizard->path();
    QString name = pWizard->fileName();

    name = Core::BaseFileWizard::buildFileName(
               path, name, QLatin1String(".py"));
    Core::GeneratedFile file(name);
    file.setContents(QLatin1String(C_NEW_FILE_CONTENT));
    file.setAttributes(Core::GeneratedFile::OpenEditorAttribute);

    return (Core::GeneratedFiles() << file);
}
