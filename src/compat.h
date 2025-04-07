/***************************************************************************
 *  Copyright (C) 2015-2023 by Mihai Moldovan <ionic@ionic.de>             *
 *                                                                         *
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program; if not, write to the                          *
 *  Free Software Foundation, Inc.,                                        *
 *  59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.              *
 ***************************************************************************/

#ifndef COMPAT_H
#define COMPAT_H

#include <QtCore/qglobal.h>
#if QT_VERSION >= QT_VERSION_CHECK (5, 0, 0)
#include <QtCore/qflags.h>
#endif /* QT_VERSION >= QT_VERSION_CHECK (5, 0, 0) */
#include <QString>
#include <QStringList>

#ifdef Q_OS_DARWIN
/*
 * strndup() is not available on 10.6 and below, define a compat version here.
 * Shameless copy from libiberty.
 */
#if MAC_OS_X_VERSION_MIN_REQUIRED < 1070
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

char *strndup (const char *s, size_t n);
#endif /* MAC_OS_X_VERSION_MIN_REQUIRED */
#endif /* defined (Q_OS_DARWIN) */

#if QT_VERSION < QT_VERSION_CHECK (5, 14, 0)
namespace Qt {
  enum SplitBehaviorFlags {
    SkipEmptyParts = QString::SkipEmptyParts,
    KeepEmptyParts = QString::KeepEmptyParts,
  };

  Q_DECLARE_FLAGS (SplitBehavior, SplitBehaviorFlags)
  Q_DECLARE_OPERATORS_FOR_FLAGS (SplitBehavior)
}

class QStringCompatWrapper : public QString {
  public:
    QStringCompatWrapper () {
    }

    QStringCompatWrapper (const QLatin1String &str) : QString (str) {
    }

    QStringCompatWrapper (const QString &str) : QString (str) {
    }

    QStringCompatWrapper (const char *str) : QString (str) {
    }

    Q_REQUIRED_RESULT
    QStringList split(const QString &sep, Qt::SplitBehavior behavior = Qt::KeepEmptyParts,
                      Qt::CaseSensitivity cs = Qt::CaseSensitive) const {
      return (QString::split (sep, QStringCompatWrapper::mapSplitBehavior (behavior), cs));
    }
    Q_REQUIRED_RESULT
    QStringList split(QChar sep, Qt::SplitBehavior behavior = Qt::KeepEmptyParts,
                      Qt::CaseSensitivity cs = Qt::CaseSensitive) const {
      return (QString::split (sep, QStringCompatWrapper::mapSplitBehavior (behavior), cs));
    }
#if QT_VERSION >= QT_VERSION_CHECK (5, 0, 0)
    Q_REQUIRED_RESULT
    QVector<QStringRef> splitRef(const QString &sep,
                                 Qt::SplitBehavior behavior = Qt::KeepEmptyParts,
                                 Qt::CaseSensitivity cs = Qt::CaseSensitive) const {
      return (QString::splitRef (sep, QStringCompatWrapper::mapSplitBehavior (behavior), cs));
    }
    Q_REQUIRED_RESULT
    QVector<QStringRef> splitRef(QChar sep, Qt::SplitBehavior behavior = Qt::KeepEmptyParts,
                                 Qt::CaseSensitivity cs = Qt::CaseSensitive) const {
      return (QString::splitRef (sep, QStringCompatWrapper::mapSplitBehavior (behavior), cs));
    }
#endif /* QT_VERSION >= QT_VERSION_CHECK (5, 0, 0) */
#ifndef QT_NO_REGEXP
    Q_REQUIRED_RESULT
    QStringList split(const QRegExp &sep,
                      Qt::SplitBehavior behavior = Qt::KeepEmptyParts) const {
      return (QString::split (sep, QStringCompatWrapper::mapSplitBehavior (behavior)));
    }
#if QT_VERSION >= QT_VERSION_CHECK (5, 0, 0)
    Q_REQUIRED_RESULT
    QVector<QStringRef> splitRef(const QRegExp &sep,
                                 Qt::SplitBehavior behavior = Qt::KeepEmptyParts) const {
      return (QString::splitRef (sep, QStringCompatWrapper::mapSplitBehavior (behavior)));
    }
#endif /* QT_VERSION >= QT_VERSION_CHECK (5, 0, 0) */
#endif /* !defined (QT_NO_REGEXP) */
#if QT_VERSION >= QT_VERSION_CHECK (5, 0, 0)
#ifndef QT_NO_REGULAREXPRESSION
    Q_REQUIRED_RESULT
    QStringList split(const QRegularExpression &sep,
                      Qt::SplitBehavior behavior = Qt::KeepEmptyParts) const {
      return (QString::split (sep, QStringCompatWrapper::mapSplitBehavior (behavior)));
    }
    Q_REQUIRED_RESULT
    QVector<QStringRef> splitRef(const QRegularExpression &sep,
                                 Qt::SplitBehavior behavior = Qt::KeepEmptyParts) const {
      return (QString::splitRef (sep, QStringCompatWrapper::mapSplitBehavior (behavior)));
    }
#endif /* !defined (QT_NO_REGULAREXPRESSION) */
#endif /* QT_VERSION >= QT_VERSION_CHECK (5, 0, 0) */

  private:
    static QString::SplitBehavior mapSplitBehavior (Qt::SplitBehavior behavior) {
      QString::SplitBehavior ret = QString::KeepEmptyParts;

      if (behavior & Qt::SkipEmptyParts) {
        ret = QString::SkipEmptyParts;
      }

      return (ret);
    }
};

class QStringListCompatWrapper : public QStringList {
  public:
    QStringListCompatWrapper () {
    }

    QStringListCompatWrapper (const QStringList &list) : QStringList (list) {
    }

    QStringListCompatWrapper (const QStringCompatWrapper &str) : QStringList (str) {
    }

    const QStringCompatWrapper& operator[] (int i) const {
      return (dynamic_cast<const QStringCompatWrapper&> (this->at (i)));
    }

    QStringCompatWrapper& operator[] (int i) {
      return (dynamic_cast<QStringCompatWrapper&> (this->at (i)));
    }
};

#define QString QStringCompatWrapper
#define QStringList QStringListCompatWrapper
#endif /* QT_VERSION < QT_VERSION_CHECK (5, 14, 0) */

#endif /* !defined (COMPAT_H) */
