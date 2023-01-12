/*
   SPDX-FileCopyrightText: 2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/
#pragma once

#include "kpimtextedittexttospeech_export.h"
#include <QWidget>
namespace KPIMTextEditTextToSpeech
{
class TextToSpeechContainerWidgetPrivate;
/**
 * @brief The TextToSpeechContainerWidget class
 * @author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDITTEXTTOSPEECH_EXPORT TextToSpeechContainerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TextToSpeechContainerWidget(QWidget *parent = nullptr);
    ~TextToSpeechContainerWidget() override;

    void say(const QString &text);

private:
    std::unique_ptr<TextToSpeechContainerWidgetPrivate> const d;
};
}
