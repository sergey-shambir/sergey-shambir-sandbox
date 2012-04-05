#include "classwizarddialog.h"
#include "classnamepage.h"

#include <utils/newclasswidget.h>
#include <coreplugin/basefilewizard.h>

namespace PyEditor {

ClassWizardDialog::ClassWizardDialog(QWidget *parent)
    : Utils::Wizard(parent)
    , _classNamePage(new ClassNamePage(this))
{
    setWindowTitle(tr("Python Class Wizard"));
    Core::BaseFileWizard::setupWizard(this);
    const int classNameId = addPage(_classNamePage);
    wizardProgress()->item(classNameId)->setTitle(tr("Details"));
}

ClassWizardDialog::~ClassWizardDialog()
{
    delete _classNamePage;
}

ClassWizardParameters ClassWizardDialog::parameters() const
{
    ClassWizardParameters p;
    const Utils::NewClassWidget *ncw = _classNamePage->newClassWidget();
    p.className = ncw->className();
    p.fileName = ncw->sourceFileName();
    p.baseClass = ncw->baseClassName();
    p.path = ncw->path();
    p.classType = ncw->classType();

    return p;
}

void ClassWizardDialog::setPath(const QString &path)
{
    _classNamePage->newClassWidget()->setPath(path);
}



} // namespace PyEditor
