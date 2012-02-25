#ifndef FILEWIZARD_H_f54dc1ca_2b85_4bfb_b547_a6e533ee929c
#define FILEWIZARD_H_f54dc1ca_2b85_4bfb_b547_a6e533ee929c

/**
   ласс CFileWizard реализует интерфейс IWizard (QtCreator),
  добавл€€ новый пункт при создании файла/проекта
  и соответствующий мастер создани€
  */

#include <coreplugin/basefilewizard.h>

namespace PythonEditor {
namespace Internal {

class CFileWizard
  : public Core::BaseFileWizard
{
    Q_OBJECT

public:
    typedef Core::BaseFileWizardParameters Parameters;

    /**
      »нициализаци€, добавление пункта в диалоге создани€ файла/проекта
      */
    explicit CFileWizard(const Parameters& params,
                         QObject* parent = 0);
    virtual ~CFileWizard();

protected:
    /**
      »нициализаци€ мастера создани€ файла
      */
    virtual QWizard* createWizardDialog(QWidget *parent,
                                        const QString &defaultPath,
                                        const WizardPageList &extensionPages) const;

    /**
      —оздание файлов по параметрам, указанным пользователем в мастере
      */
    virtual Core::GeneratedFiles generateFiles(const QWizard *dialog,
                                               QString *errorMessage) const;
};

} // namespace Internal
} // namespace PythonEditor

#endif // FILEWIZARD_H_f54dc1ca_2b85_4bfb_b547_a6e533ee929c
