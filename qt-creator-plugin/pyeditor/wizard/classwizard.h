#ifndef PYEDITOR_CLASSWIZARD_H
#define PYEDITOR_CLASSWIZARD_H

#include <coreplugin/basefilewizard.h>
#include <utils/wizard.h>

namespace Utils { class NewClassWidget; }

namespace PyEditor {

class ClassWizard : public Core::BaseFileWizard
{
    Q_OBJECT

public:
    explicit ClassWizard(QObject *parent = 0);
    virtual ~ClassWizard();
    
protected:
    /*virtual*/ QWizard *createWizardDialog(
            QWidget *parent,
            const Core::WizardDialogParameters &params) const;

    /*virtual*/ Core::GeneratedFiles generateFiles(const QWizard *w,
                                               QString *errorMessage) const;
};

} // namespace PyEditor

#endif // PYEDITOR_CLASSWIZARD_H
