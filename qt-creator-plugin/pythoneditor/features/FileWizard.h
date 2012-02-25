#ifndef FILEWIZARD_H_f54dc1ca_2b85_4bfb_b547_a6e533ee929c
#define FILEWIZARD_H_f54dc1ca_2b85_4bfb_b547_a6e533ee929c

/**
  ����� CFileWizard ��������� ��������� IWizard (QtCreator),
  �������� ����� ����� ��� �������� �����/�������
  � ��������������� ������ ��������
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
      �������������, ���������� ������ � ������� �������� �����/�������
      */
    explicit CFileWizard(const Parameters& params,
                         QObject* parent = 0);
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

} // namespace Internal
} // namespace PythonEditor

#endif // FILEWIZARD_H_f54dc1ca_2b85_4bfb_b547_a6e533ee929c
