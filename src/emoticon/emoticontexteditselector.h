/*
  SPDX-FileCopyrightText: 2012-2021 Laurent Montel <montel@kde.org>
  based on code from kopete

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QWidget>
namespace KPIMTextEdit
{
class EmoticonUnicodeTab;
class EmoticonTextEditSelector : public QWidget
{
    Q_OBJECT
public:
    explicit EmoticonTextEditSelector(QWidget *parent = nullptr);
    ~EmoticonTextEditSelector() override;

public Q_SLOTS:
    void loadEmoticons();

Q_SIGNALS:
    void itemSelected(const QString &);

private:
    void slotItemSelected(const QString &str);
    EmoticonUnicodeTab *mUnicodeTab = nullptr;
};
}

