/*
  SPDX-FileCopyrightText: 2019-2023 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QTabWidget>

#include "kpimtextedit_private_export.h"

namespace KPIMTextEdit
{
class EmoticonUnicodeProxyModel;
class EmoticonRecentUsedFilterProxyModel;
/**
 * @brief The EmoticonUnicodeTab class
 * @author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_TESTS_EXPORT EmoticonUnicodeTab : public QTabWidget
{
    Q_OBJECT
public:
    explicit EmoticonUnicodeTab(QWidget *parent = nullptr);
    ~EmoticonUnicodeTab() override;

    void loadEmoticons();

    void searchUnicode(const QString &str);
Q_SIGNALS:
    void itemSelected(const QString &str);

private:
    KPIMTEXTEDIT_NO_EXPORT void createSearchTab();
    KPIMTEXTEDIT_NO_EXPORT void createRecentTab();
    KPIMTEXTEDIT_NO_EXPORT void slotInsertEmoticons(const QString &str, const QString &identifier);
    KPIMTEXTEDIT_NO_EXPORT void slotUsedIdentifierChanged(const QStringList &lst);
    EmoticonUnicodeProxyModel *const mEmoticonUnicodeSearchProxyModel;
    EmoticonRecentUsedFilterProxyModel *const mEmoticonUnicodeRecentProxyModel;
    int mSearchTabIndex = -1;
    int mRecentTabIndex = -1;
};
}
