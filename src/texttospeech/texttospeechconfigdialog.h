/*
   SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedit_private_export.h"
#include <QDialog>
namespace KPIMTextEdit
{
class TextToSpeechConfigWidget;
class KPIMTEXTEDIT_TESTS_EXPORT TextToSpeechConfigDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TextToSpeechConfigDialog(QWidget *parent = nullptr);
    ~TextToSpeechConfigDialog() override;

private:
    void slotAccepted();
    void slotRestoreDefaults();
    void readConfig();
    void writeConfig();
    KPIMTextEdit::TextToSpeechConfigWidget *mTextToSpeechConfigWidget = nullptr;
};
}

