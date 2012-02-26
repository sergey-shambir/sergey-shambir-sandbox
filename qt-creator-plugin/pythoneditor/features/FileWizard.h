#ifndef PYTHON_EDITOR__FILE_WIZARD__INCLUDED
#define PYTHON_EDITOR__FILE_WIZARD__INCLUDED

#include <coreplugin/basefilewizard.h>

namespace PythonEditor {

/**
  CFileWizard ��������� ������ �������� ������ ��� �������� Python
  */
class CFileWizard
  : public Core::BaseFileWizard
{
    Q_OBJECT

public:
    /**
      �������������, ���������� ������ � ������� �������� �����/�������
      */
    explicit CFileWizard(QObject* parent = 0);
    virtual ~CFileWizard();

protected:
    /**
      ������������� ������� �������� �����
      */
    virtual QWizard* createWizardDialog(QWidget *parent,
                                        const QString &defaultPath,
                                        const WizardPageList &extensionPages) const;

    /**
      �������� ������ �� ����������, ��������� ������������� � �������
      */
    virtual Core::GeneratedFiles generateFiles(const QWizard *dialog,
                                               QString *errorMessage) const;
};

} // namespace PythonEditor

#endif // PYTHON_EDITOR__FILE_WIZARD__INCLUDED
