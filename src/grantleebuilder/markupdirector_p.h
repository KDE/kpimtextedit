/*
  This file is part of the Grantlee template system.

  SPDX-FileCopyrightText: 2008 Stephen Kelly <steveire@gmail.com>

  SPDX-License-Identifier: LGPL-2.1-or-later

*/

#ifndef GRANTLEE_MARKUPDIRECTOR_P_H
#define GRANTLEE_MARKUPDIRECTOR_P_H

#include "markupdirector.h"

#include <QSet>

//@cond PRIVATE

namespace KPIMTextEdit
{
/**
  @internal
  Maintainability class for MarkupDirector
*/
class MarkupDirectorPrivate
{
    MarkupDirectorPrivate(MarkupDirector *md)
        : q_ptr(md)
    {
    }

    Q_DECLARE_PUBLIC(MarkupDirector)
    MarkupDirector *const q_ptr;

    QString m_openAnchorHref;
    QString m_anchorHrefToOpen;
    QString m_openAnchorName;

    QBrush m_openForeground;
    QBrush m_foregroundToOpen;
    QBrush m_openBackground;
    QBrush m_backgroundToOpen;
    int m_openFontPointSize;
    int m_fontPointSizeToOpen;
    QString m_openFontFamily;
    QString m_fontFamilyToOpen;

    // An ordered list containing the order elements were opened in.
    QList<int> m_openElements;

    // Elements that have yet to be opened. Used while determine the order to
    // open them.
    QSet<int> m_elementsToOpen;
};
}

#endif
//@endcond
