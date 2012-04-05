#ifndef PYEDITOR_FILEWIZARD_H
#define PYEDITOR_FILEWIZARD_H

#include <coreplugin/basefilewizard.h>

namespace PyEditor {

class FileWizard : public Core::BaseFileWizard
{
    Q_OBJECT

public:
    explicit FileWizard(QObject* parent = 0);
    virtual ~FileWizard();

protected:
    virtual QWizard* createWizardDialog(
            QWidget *parent,
            const Core::WizardDialogParameters &params) const;

    virtual Core::GeneratedFiles generateFiles(
            const QWizard *dialog,
            QString *errorMessage) const;
};

} // namespace PyEditor

#endif // PYEDITOR_FILEWIZARD_H
