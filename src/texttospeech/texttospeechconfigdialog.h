/*
   SPDX-FileCopyrightText: 2014-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef TEXTTOSPEECHCONFIGDIALOG_H
#define TEXTTOSPEECHCONFIGDIALOG_H

#include <QDialog>
#include "kpimtextedit_private_export.h"
namespace KPIMTextEdit {
class TextToSpeechConfigWidget;
class KPIMTEXTEDIT_TESTS_EXPORT TextToSpeechConfigDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TextToSpeechConfigDialog(QWidget *parent = nullptr);
    ~TextToSpeechConfigDialog();

private:
    void slotAccepted();
    void slotRestoreDefaults();
    void readConfig();
    void writeConfig();
    KPIMTextEdit::TextToSpeechConfigWidget *mTextToSpeechConfigWidget = nullptr;
};
}

#endif // TEXTTOSPEECHCONFIGDIALOG_H
