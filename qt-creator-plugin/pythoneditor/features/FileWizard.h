#ifndef PYTHON_EDITOR__FILE_WIZARD__INCLUDED
#define PYTHON_EDITOR__FILE_WIZARD__INCLUDED

#include <coreplugin/basefilewizard.h>

namespace PythonEditor {

/**
  CFileWizard добавляет мастер создания файлов для скриптов Python
  */
class CFileWizard
  : public Core::BaseFileWizard
{
    Q_OBJECT

public:
    /**
      Инициализация, добавление пункта в диалоге создания файла/проекта
      */
    explicit CFileWizard(QObject* parent = 0);
    virtual ~CFileWizard();

protected:
    /**
      Инициализация мастера создания файла
      */
    virtual QWizard* createWizardDialog(QWidget *parent,
                                        const QString &defaultPath,
                                        const WizardPageList &extensionPages) const;

    /**
      Создание файлов по параметрам, указанным пользователем в мастере
      */
    virtual Core::GeneratedFiles generateFiles(const QWizard *dialog,
                                               QString *errorMessage) const;
};

} // namespace PythonEditor

#endif // PYTHON_EDITOR__FILE_WIZARD__INCLUDED
