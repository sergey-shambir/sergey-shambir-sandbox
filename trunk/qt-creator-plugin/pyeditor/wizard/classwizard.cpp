#include "classwizard.h"
#include "classwizarddialog.h"
#include "classnamepage.h"
#include "../pyeditorconstants.h"

namespace PyEditor {

static inline
Core::BaseFileWizardParameters getDefaultParams()
{
    Core::BaseFileWizardParameters p(Core::IWizard::FileWizard);

    p.setId(QLatin1String(Constants::C_PY_CLASS_WIZARD_ID));
    p.setCategory(QLatin1String(Constants::C_PY_WIZARD_CATEGORY));
    p.setDisplayCategory(QLatin1String(Constants::C_PY_DISPLAY_CATEGORY));
    p.setDisplayName(
                QObject::tr(Constants::EN_PY_CLASS_DISPLAY_NAME));
    p.setDescription(
                QObject::tr(Constants::EN_PY_CLASS_DESCRIPTION));

    return p;
}

ClassWizard::ClassWizard(QObject *parent) :
    Core::BaseFileWizard(getDefaultParams(), parent)
{
}

ClassWizard::~ClassWizard()
{
}

QWizard *ClassWizard::createWizardDialog(
        QWidget *parent,
        const Core::WizardDialogParameters &params) const
{
    ClassWizardDialog *wizard = new ClassWizardDialog(parent);
    foreach (QWizardPage *p, params.extensionPages())
        BaseFileWizard::applyExtensionPageShortTitle(wizard, wizard->addPage(p));
    wizard->setPath(params.defaultPath());
    return wizard;
}

static inline
bool generateSourceFile(const ClassWizardParameters &params,
                        QString &content)
{
    content += QLatin1String("# -*- coding: utf-8 -*-\n\n");

    QString base;
    QString import;
    if (params.classType) {
        switch (params.classType) {
        case Utils::NewClassWidget::ClassInheritsQObject:
            import = "from PySide import QtCore";
            base = "QtCore.QObject";
            break;
        case Utils::NewClassWidget::ClassInheritsQWidget:
            import = "from PySide import QtCore, QtGui";
            base = "QtGui.QWidget";
            break;
        default: // QtDeclarative
            import = "from PySide import QtCore, QtDeclarative";
            base = "QtDeclarative.QDeclarative";
            break;
        }
    } else {
        int dotIndex = params.baseClass.lastIndexOf('.');
        if (dotIndex != -1) {
            base = params.baseClass;
            import = QLatin1String("import ") + params.baseClass.left(dotIndex);
        } else {
            base = params.baseClass;
        }
    }

    if (!import.isEmpty()) {
        content += import;
        content += QLatin1String("\n\n");
    }

    if (base.isEmpty()) {
        content += QLatin1String("class ")
                + params.className
                + QLatin1String(":\n");
    } else {
        content += QLatin1String("class ")
                + params.className
                + QLatin1String("(")
                + base
                + QLatin1String("):\n");
    }
    content += QLatin1String("    def __init__(self):\n");
    content += QLatin1String("        pass\n");

    return true;
}

Core::GeneratedFiles ClassWizard::generateFiles(const QWizard *w,
                                                QString *errorMessage) const
{
    const ClassWizardDialog *wizard = qobject_cast<const ClassWizardDialog *>(w);
    const ClassWizardParameters params = wizard->parameters();

    const QString fileName = Core::BaseFileWizard::buildFileName(
                params.path, params.fileName, Constants::C_PY_EXTENSION);
    Core::GeneratedFile sourceFile(fileName);

    QString source;
    if (!generateSourceFile(params, source)) {
        *errorMessage = tr("Error while generating file contents.");
        return Core::GeneratedFiles();
    }
    sourceFile.setContents(source);
    sourceFile.setAttributes(Core::GeneratedFile::OpenEditorAttribute);
    return Core::GeneratedFiles() << sourceFile;
}

} // namespace PyEditor
