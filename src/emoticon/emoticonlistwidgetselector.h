/*
  SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "emoticonunicodeutils.h"
#include "kpimtextedit_private_export.h"
#include <QListWidget>

namespace KPIMTextEdit
{
class KPIMTEXTEDIT_TESTS_EXPORT EmoticonTextEditItem : public QListWidgetItem
{
public:
    explicit EmoticonTextEditItem(const QString &emoticonText, const QString &tooltip, QListWidget *parent);
    Q_REQUIRED_RESULT QString text() const;

private:
    const QString mText;
};

class KPIMTEXTEDIT_TESTS_EXPORT EmoticonListWidgetSelector : public QListWidget
{
    Q_OBJECT
public:
    explicit EmoticonListWidgetSelector(QWidget *parent = nullptr);
    ~EmoticonListWidgetSelector() override;

    void setEmoticons(const QVector<EmoticonUnicodeUtils::EmoticonStruct> &emoticons);
Q_SIGNALS:
    void itemSelected(const QString &);

private:
    void slotMouseOverItem(QListWidgetItem *item);
    void slotEmoticonClicked(QListWidgetItem *item);
};
}

