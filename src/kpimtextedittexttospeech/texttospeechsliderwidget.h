/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "kpimtextedittexttospeech_private_export.h"
#include <QWidget>
class QLabel;
class QSlider;
namespace KPIMTextEditTextToSpeech
{
class KPIMTEXTEDITTEXTTOSPEECH_TESTS_EXPORT TextToSpeechSliderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TextToSpeechSliderWidget(QWidget *parent = nullptr);
    ~TextToSpeechSliderWidget() override;

    void setValue(int value);

private:
    void slotValueChanged(int value);
    QLabel *const mLabel;
    QSlider *const mSlider;
};
}
