/*
  SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QTabWidget>

#include "emoticonunicodeutils.h"
#include "kpimtextedit_export.h"

namespace KPIMTextEdit
{
class EmoticonUnicodeProxyModel;
class EmoticonUnicodeModel;
/**
 * @brief The EmoticonUnicodeTab class
 * @author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_EXPORT EmoticonUnicodeTab : public QTabWidget
{
    Q_OBJECT
public:
    explicit EmoticonUnicodeTab(QWidget *parent = nullptr);
    ~EmoticonUnicodeTab() override;

    void loadEmoticons();

    void searchUnicode(const QString &str);
Q_SIGNALS:
    void itemSelected(const QString &);

private:
    void createEmoticonTab(const QString &str, const QVector<EmoticonUnicodeUtils::EmoticonStruct> &emoticons);
    void createSearchTab();
    EmoticonUnicodeModel *const mEmoticonUnicodeModel;
    EmoticonUnicodeProxyModel *mEmoticonUnicodeSearchProxyModel = nullptr;
    int mSearchTabIndex = -1;
};
}

