/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**************************************************************************/

#include "classnamevalidatinglineedit.h"
#include "classnamevalidator.h"

#include <utils/qtcassert.h>

#include <QDebug>
#include <QRegExp>

/*!
    \class Utils::ClassNameValidatingLineEdit

    \brief A Line edit that validates a C++ class name and emits a signal
    to derive suggested file names from it.
*/

namespace Utils {

struct ClassNameValidatingLineEditPrivate {
    ClassNameValidatingLineEditPrivate();

    ClassNameValidator m_validator;
    bool m_lowerCaseFileName;
};

// Match something like "Namespace1::Namespace2::ClassName".
ClassNameValidatingLineEditPrivate:: ClassNameValidatingLineEditPrivate() :
    m_lowerCaseFileName(true)
{
    m_validator.setNamespacesEnabled(false);
}

// --------------------- ClassNameValidatingLineEdit
ClassNameValidatingLineEdit::ClassNameValidatingLineEdit(QWidget *parent) :
    Utils::BaseValidatingLineEdit(parent),
    d(new ClassNameValidatingLineEditPrivate)
{
}

ClassNameValidatingLineEdit::~ClassNameValidatingLineEdit()
{
    delete d;
}

bool ClassNameValidatingLineEdit::namespacesEnabled() const
{
    return d->m_validator.namespacesEnabled();
}

void ClassNameValidatingLineEdit::setNamespacesEnabled(bool b)
{
    d->m_validator.setNamespacesEnabled(b);
}

const QString &ClassNameValidatingLineEdit::namespaceDelimiter() const
{
    return d->m_validator.namespaceDelimiter();
}

void ClassNameValidatingLineEdit::setNamespaceDelimiter(const QString &delim)
{
    d->m_validator.setNamespaceDelimiter(delim);
}

bool ClassNameValidatingLineEdit::validate(const QString &value, QString *errorMessage) const
{
    if (!d->m_validator.namespacesEnabled()
            && value.contains(d->m_validator.namespaceDelimiter())) {
        if (errorMessage)
            *errorMessage = tr("The class name must not contain namespace delimiters.");
        return false;
    } else if (value.isEmpty()) {
        if (errorMessage)
            *errorMessage = tr("Please enter a class name.");
        return false;
    } else if (!d->m_validator.validate(value)) {
        if (errorMessage)
            *errorMessage = tr("The class name contains invalid characters.");
        return false;
    }
    return true;
}

void ClassNameValidatingLineEdit::slotChanged(const QString &t)
{
    Utils::BaseValidatingLineEdit::slotChanged(t);
    if (isValid()) {
        // Suggest file names, strip namespaces
        QString fileName = d->m_lowerCaseFileName ? t.toLower() : t;
        if (d->m_validator.namespacesEnabled()) {
            const int namespaceIndex = fileName.lastIndexOf(
                        d->m_validator.namespaceDelimiter());
            if (namespaceIndex != -1) {
                const int len = namespaceIndex
                        + d->m_validator.namespaceDelimiter().size();
                fileName.remove(0, namespaceIndex + len);
            }
        }
        emit updateFileName(fileName);
    }
}

QString ClassNameValidatingLineEdit::createClassName(const QString &name)
{
    // Remove spaces and convert the adjacent characters to uppercase
    QString className = name;
    QRegExp spaceMatcher(QLatin1String(" +(\\w)"), Qt::CaseSensitive, QRegExp::RegExp2);
    QTC_CHECK(spaceMatcher.isValid());
    int pos;
    while ((pos = spaceMatcher.indexIn(className)) != -1) {
        className.replace(pos, spaceMatcher.matchedLength(),
                          spaceMatcher.cap(1).toUpper());
    }

    // Filter out any remaining invalid characters
    className.remove(QRegExp(QLatin1String("[^a-zA-Z0-9_]")));

    // If the first character is numeric, prefix the name with a "_"
    if (className.at(0).isNumber()) {
        className.prepend(QLatin1Char('_'));
    } else {
        // Convert the first character to uppercase
        className.replace(0, 1, className.left(1).toUpper());
    }

    return className;
}

bool ClassNameValidatingLineEdit::lowerCaseFileName() const
{
    return d->m_lowerCaseFileName;
}

void ClassNameValidatingLineEdit::setLowerCaseFileName(bool v)
{
    d->m_lowerCaseFileName = v;
}

} // namespace Utils
