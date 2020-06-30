/*
  SPDX-FileCopyrightText: 2012-2020 Laurent Montel <montel@kde.org>

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

#ifndef KPIMTEXTEDIT_INSERTIMAGEDIALOG_H
#define KPIMTEXTEDIT_INSERTIMAGEDIALOG_H

#include <QDialog>
#include <QUrl>

namespace KPIMTextEdit {
class InsertImageDialogPrivate;

class InsertImageDialog : public QDialog
{
    Q_OBJECT
public:
    explicit InsertImageDialog(QWidget *parent = nullptr);
    ~InsertImageDialog();

    Q_REQUIRED_RESULT int imageWidth() const;
    Q_REQUIRED_RESULT int imageHeight() const;

    void setImageWidth(int value);
    void setImageHeight(int value);

    Q_REQUIRED_RESULT QUrl imageUrl() const;
    void setImageUrl(const QUrl &url);

    Q_REQUIRED_RESULT bool keepOriginalSize() const;

private:
    friend class InsertImageDialogPrivate;
    InsertImageDialogPrivate *const d;
};
}

#endif // KPIMTEXTEDIT_INSERTIMAGEDIALOG_H
