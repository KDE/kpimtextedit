/*
  Copyright (c) 2019 Montel Laurent <montel@kde.org>

  This library is free software; you can redistribute it and/or modify it
  under the terms of the GNU Library General Public License as published by
  the Free Software Foundation; either version 2 of the License, or (at your
  option) any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
  License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to the
  Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301, USA.
*/

#ifndef EMOTICONUNICODETAB_H
#define EMOTICONUNICODETAB_H

#include <QTabWidget>

#include "kpimtextedit_export.h"

namespace KPIMTextEdit {
class KPIMTEXTEDIT_EXPORT EmoticonUnicodeTab : public QTabWidget
{
    Q_OBJECT
public:
    explicit EmoticonUnicodeTab(QWidget *parent = nullptr);
    ~EmoticonUnicodeTab();

    Q_REQUIRED_RESULT bool emojiPlainText() const;
    void setEmojiPlainText(bool emojiPlainText);

    void loadEmoticons();

Q_SIGNALS:
    void itemSelected(const QString &);

private:
    void createPlainTextEmoticonTab(const QString &str, const QList<uint> &emoticons);
    void createHtmlEmoticonTab();
    bool mEmojiPlainText = true;
};
}

#endif // EMOTICONUNICODETAB_H