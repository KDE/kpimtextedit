/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "texttospeechsliderwidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>

using namespace KPIMTextEditTextToSpeech;
TextToSpeechSliderWidget::TextToSpeechSliderWidget(QWidget *parent)
    : QWidget{parent}
    , mLabel(new QLabel(this))
    , mSlider(new QSlider(this))
{
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins({});
    mainLayout->setObjectName(QStringLiteral("mainLayout"));
    mSlider->setObjectName(QStringLiteral("mSlider"));
    mLabel->setObjectName(QStringLiteral("mLabel"));
    mainLayout->addWidget(mSlider);
    mainLayout->addWidget(mLabel);

    connect(mSlider, &QSlider::valueChanged, this, &TextToSpeechSliderWidget::slotValueChanged);
}

TextToSpeechSliderWidget::~TextToSpeechSliderWidget() = default;

void TextToSpeechSliderWidget::setValue(int value)
{
    mSlider->setValue(value);
    // TODO
}

void TextToSpeechSliderWidget::slotValueChanged(int value)
{
}