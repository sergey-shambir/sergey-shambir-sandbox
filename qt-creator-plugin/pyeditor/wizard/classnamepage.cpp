#include "classnamepage.h"
#include "../pyeditorconstants.h"

// QtCreator platform & plugins
#include <coreplugin/icore.h>
#include <coreplugin/mimedatabase.h>
#include <utils/newclasswidget.h>

// Qt library
#include <QtGui/QVBoxLayout>

namespace PyEditor {

static const QLatin1String PYTHON_NAMESPACE_DELIMITER = QLatin1String(".");

ClassNamePage::ClassNamePage(QWidget *parent)
    : QWizardPage(parent)
    , _isValid(false)
{
    setTitle(tr("Enter Class Name"));
    setSubTitle(tr("The source file name will be derived from the class name"));

    _newClassWidget = new Utils::NewClassWidget;
    // Order, set extensions first before suggested name is derived
    _newClassWidget->setClassTypeComboVisible(true);
    _newClassWidget->setBaseClassChoices(QStringList()
            << QString()
            << QLatin1String("QObject")
            << QLatin1String("QWidget")
            << QLatin1String("QMainWindow")
            << QLatin1String("QDeclarativeItem"));

    _newClassWidget->setBaseClassEditable(true);
    _newClassWidget->setFormInputVisible(false);
    _newClassWidget->setHeaderInputVisible(false);
    _newClassWidget->setNamespacesEnabled(true);
    _newClassWidget->setNamespaceDelimiter(PYTHON_NAMESPACE_DELIMITER);
    _newClassWidget->setAllowDirectories(true);
    _newClassWidget->setBaseClassInputVisible(true);

    connect(_newClassWidget, SIGNAL(validChanged()), this, SLOT(slotValidChanged()));

    QVBoxLayout *pageLayout = new QVBoxLayout(this);
    pageLayout->addWidget(_newClassWidget);
    QSpacerItem *vSpacer = new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::Expanding);
    pageLayout->addItem(vSpacer);

    initParameters();
}

ClassNamePage::~ClassNamePage()
{
    delete _newClassWidget;
}

// Set up new class widget from settings
void ClassNamePage::initParameters()
{
    /** TODO: at this moment files are always lowercase, add scanning enviroment
     * settings like inside CppEditor plugin
     */
   // Core::ICore *core = Core::ICore::instance();
   // const Core::MimeDatabase *mdb = core->mimeDatabase();
    _newClassWidget->setSourceExtension(Constants::C_PY_EXTENSION);
    //_newClassWidget->setLowerCaseFiles(lowerCaseFiles(core));
}

void ClassNamePage::slotValidChanged()
{
    const bool validNow = _newClassWidget->isValid();
    if (_isValid != validNow) {
        _isValid = validNow;
        emit completeChanged();
    }
}

} // namespace PyEditor
