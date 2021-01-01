/*
  SPDX-FileCopyrightText: 2019-2021 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef EMOTICONUNICODETAB_H
#define EMOTICONUNICODETAB_H

#include <QTabWidget>

#include "emoticonunicodeutils.h"
#include "kpimtextedit_export.h"

namespace KPIMTextEdit {
/**
 * @brief The EmoticonUnicodeTab class
 * @author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_EXPORT EmoticonUnicodeTab : public QTabWidget
{
    Q_OBJECT
public:
    explicit EmoticonUnicodeTab(QWidget *parent = nullptr);
    ~EmoticonUnicodeTab();

    void loadEmoticons();

Q_SIGNALS:
    void itemSelected(const QString &);

private:
    void createPlainTextEmoticonTab(const QString &str, const QVector<EmoticonUnicodeUtils::EmoticonStruct> &emoticons);
};
}

#endif // EMOTICONUNICODETAB_H
