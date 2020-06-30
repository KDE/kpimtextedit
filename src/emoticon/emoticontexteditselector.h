/*
  SPDX-FileCopyrightText: 2012-2020 Laurent Montel <montel@kde.org>
  based on code from kopete

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
