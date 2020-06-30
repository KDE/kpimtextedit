/*
  SPDX-FileCopyrightText: 2012-2020 Laurent Montel <montel@kde.org>
  based on code from kopete

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KPIMTEXTEDIT_EMOTICONTEXTEDITSELECTOR_H
#define KPIMTEXTEDIT_EMOTICONTEXTEDITSELECTOR_H

#include <QWidget>
namespace KPIMTextEdit {
class EmoticonUnicodeTab;
class EmoticonTextEditSelector : public QWidget
{
    Q_OBJECT
public:
    explicit EmoticonTextEditSelector(QWidget *parent = nullptr);
    ~EmoticonTextEditSelector();

public Q_SLOTS:
    void loadEmoticons();

Q_SIGNALS:
    void itemSelected(const QString &);

private:
    void slotItemSelected(const QString &str);
    EmoticonUnicodeTab *mUnicodeTab = nullptr;
};
}

#endif /* KPIMTEXTEDIT_EMOTICONTEXTEDITSELECTOR_H */
