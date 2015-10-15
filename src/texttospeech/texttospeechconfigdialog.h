/*
  Copyright (c) 2014-2015 Montel Laurent <montel@kde.org>

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

#ifndef TEXTTOSPEECHCONFIGDIALOG_H
#define TEXTTOSPEECHCONFIGDIALOG_H

#include <QDialog>
#include "kpimtextedit_export.h"
namespace KPIMTextEdit
{
class TextToSpeechConfigWidget;
class KPIMTEXTEDIT_EXPORT TextToSpeechConfigDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TextToSpeechConfigDialog(QWidget *parent = Q_NULLPTR);
    ~TextToSpeechConfigDialog();

private Q_SLOTS:
    void slotAccepted();

private:
    void readConfig();
    void writeConfig();
    KPIMTextEdit::TextToSpeechConfigWidget *mTextToSpeechConfigWidget;
};
}

#endif // TEXTTOSPEECHCONFIGDIALOG_H

