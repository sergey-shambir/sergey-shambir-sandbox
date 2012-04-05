#ifndef PYEDITOR_CLASSWIZARDDIALOG_H
#define PYEDITOR_CLASSWIZARDDIALOG_H

#include <utils/wizard.h>
#include <utils/newclasswidget.h>

namespace PyEditor {

class ClassNamePage;

class ClassWizardParameters
{
public:
    QString className;
    QString fileName;
    QString path;
    QString baseClass;
    int classType;
};

class ClassWizardDialog : public Utils::Wizard
{
    Q_OBJECT
public:
    explicit ClassWizardDialog(QWidget *parent = 0);
    virtual ~ClassWizardDialog();

    void setPath(const QString &path);
    ClassWizardParameters parameters() const;

private:
    ClassNamePage *_classNamePage;
};

} // namespace PyEditor

#endif // PYEDITOR_CLASSWIZARDDIALOG_H
