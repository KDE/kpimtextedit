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

#include "insertimagedialog.h"
#include <KLocale>

#include <QVBoxLayout>

namespace KPIMTextEdit {

class InsertImageDialogPrivate
{
public:
  InsertImageDialogPrivate(InsertImageDialog *qq)
    :q(qq)
  {
    q->setCaption( i18n("Insert Image") );
    q->setButtons( KDialog::Ok|KDialog::Cancel );
    q->setButtonText(KDialog::Ok,i18n("Insert"));
    QWidget *page = new QWidget(q );
    q->setMainWidget( page );
    QVBoxLayout *lay = new QVBoxLayout(page);
  }
  InsertImageDialog *q;
};

InsertImageDialog::InsertImageDialog(QWidget *parent)
  :KDialog(parent),d(new InsertImageDialogPrivate(this))
{
}


InsertImageDialog::~InsertImageDialog()
{
  delete d;
}

}
