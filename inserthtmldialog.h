/*
  Copyright (c) 2012 Montel Laurent <montel@kde.org>
  
  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.
  
  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.
  
  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef INSERTHTMLDIALOG_H
#define INSERTHTMLDIALOG_H

#include <KDialog>
namespace KPIMTextEdit {
class InsertHtmlDialogPrivate;

class InsertHtmlDialog : public KDialog
{
  Q_OBJECT
public:
  explicit InsertHtmlDialog(QWidget *parent = 0);
  ~InsertHtmlDialog();
  QString html() const;

private:
  friend class InsertHtmlDialogPrivate;
  InsertHtmlDialogPrivate *d;
  Q_PRIVATE_SLOT( d, void _k_slotTextChanged() )

};

}

#endif // INSERTHTMLDIALOG_H
