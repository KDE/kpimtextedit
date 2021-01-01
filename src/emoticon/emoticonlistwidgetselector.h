/*
  SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef EMOTICONLISTWIDGETSELECTOR_H
#define EMOTICONLISTWIDGETSELECTOR_H

#include <QListWidget>
#include "kpimtextedit_private_export.h"

namespace KPIMTextEdit {
class KPIMTEXTEDIT_TESTS_EXPORT EmoticonTextEditItem : public QListWidgetItem
{
public:
    explicit EmoticonTextEditItem(const QString &emoticonText, QListWidget *parent);
    Q_REQUIRED_RESULT QString text() const;

private:
    const QString mText;
};

class KPIMTEXTEDIT_TESTS_EXPORT EmoticonListWidgetSelector : public QListWidget
{
    Q_OBJECT
public:
    explicit EmoticonListWidgetSelector(QWidget *parent = nullptr);
    ~EmoticonListWidgetSelector();

    void setEmoticons(const QList<uint> &lst);
    void setEmoticons(const QStringList &lst);
Q_SIGNALS:
    void itemSelected(const QString &);

private:
    void slotMouseOverItem(QListWidgetItem *item);
    void slotEmoticonClicked(QListWidgetItem *item);
};
}

#endif // EMOTICONLISTWIDGETSELECTOR_H
