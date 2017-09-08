/*
   Copyright (C) 2014-2017 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "texttospeechwidget.h"
#include "texttospeechactions.h"
#include "texttospeechinterface.h"
#include "texttospeechconfigdialog.h"
#include <KLocalizedString>
#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>
#include <QSlider>
#include <QPointer>

using namespace KPIMTextEdit;

class KPIMTextEdit::TextToSpeechWidgetPrivate
{
public:
    TextToSpeechWidgetPrivate()
    {
    }

    bool mNeedToHide = false;
    QPointer<KPIMTextEdit::TextToSpeechConfigDialog> mConfigDialog;
    QToolButton *mStopButton = nullptr;
    QToolButton *mPlayPauseButton = nullptr;
    QToolButton *mConfigureButton = nullptr;
    AbstractTextToSpeechInterface *mTextToSpeechInterface = nullptr;
    TextToSpeechActions *mTextToSpeechActions = nullptr;
    QSlider *mVolume = nullptr;
};

TextToSpeechWidget::TextToSpeechWidget(QWidget *parent)
    : QWidget(parent)
    , d(new KPIMTextEdit::TextToSpeechWidgetPrivate)
{
    QHBoxLayout *hbox = new QHBoxLayout(this);

    d->mTextToSpeechActions = new TextToSpeechActions(this);
    connect(d->mTextToSpeechActions, &TextToSpeechActions::stateChanged, this, &TextToSpeechWidget::stateChanged);

    QToolButton *close = new QToolButton(this);
    close->setObjectName(QStringLiteral("close-button"));
    close->setIcon(QIcon::fromTheme(QStringLiteral("dialog-close")));
    close->setToolTip(i18n("Close"));
    connect(close, &QToolButton::clicked, this, &TextToSpeechWidget::hide);
    hbox->addWidget(close);
    hbox->addStretch(0);

    QLabel *volume = new QLabel(i18n("Volume:"));
    hbox->addWidget(volume);
    d->mVolume = new QSlider;
    d->mVolume->setMinimumWidth(100);
    d->mVolume->setOrientation(Qt::Horizontal);
    d->mVolume->setObjectName(QStringLiteral("volumeslider"));
    d->mVolume->setRange(0, 100);
    d->mVolume->setTickPosition(QSlider::TicksBelow);
    connect(d->mVolume, &QSlider::valueChanged, this, &TextToSpeechWidget::slotVolumeChanged);
    hbox->addWidget(d->mVolume);

    d->mStopButton = new QToolButton;
    d->mStopButton->setObjectName(QStringLiteral("stopbutton"));
    d->mStopButton->setDefaultAction(d->mTextToSpeechActions->stopAction());
    hbox->addWidget(d->mStopButton);

    d->mPlayPauseButton = new QToolButton;
    d->mPlayPauseButton->setObjectName(QStringLiteral("playpausebutton"));
    d->mPlayPauseButton->setDefaultAction(d->mTextToSpeechActions->playPauseAction());
    hbox->addWidget(d->mPlayPauseButton);

    d->mConfigureButton = new QToolButton;
    d->mConfigureButton->setIcon(QIcon::fromTheme(QStringLiteral("configure")));
    d->mConfigureButton->setToolTip(i18n("Configure..."));
    d->mConfigureButton->setObjectName(QStringLiteral("configurebutton"));
    connect(d->mConfigureButton, &QToolButton::clicked, this, &TextToSpeechWidget::slotConfigure);
    hbox->addWidget(d->mConfigureButton);

    setTextToSpeechInterface(new TextToSpeechInterface(this, this));
    hide();
}

TextToSpeechWidget::~TextToSpeechWidget()
{
    delete d;
}

void TextToSpeechWidget::slotConfigure()
{
    if (!d->mConfigDialog.data()) {
        d->mNeedToHide = false;
        d->mConfigDialog = new TextToSpeechConfigDialog(this);
        if (d->mConfigDialog->exec()) {
            d->mTextToSpeechInterface->reloadSettings();
        }
        delete d->mConfigDialog;
        if (d->mNeedToHide) {
            hide();
            d->mNeedToHide = false;
        }
    }
}

void TextToSpeechWidget::slotVolumeChanged(int value)
{
    d->mTextToSpeechInterface->setVolume(value);
}

bool TextToSpeechWidget::isReady() const
{
    return d->mTextToSpeechInterface->isReady();
}

void TextToSpeechWidget::say(const QString &text)
{
    if (d->mTextToSpeechInterface->isReady()) {
        d->mTextToSpeechInterface->say(text);
    }
}

TextToSpeechWidget::State TextToSpeechWidget::state() const
{
    return d->mTextToSpeechActions->state();
}

void TextToSpeechWidget::slotStateChanged(KPIMTextEdit::TextToSpeech::State state)
{
    switch (state) {
    case KPIMTextEdit::TextToSpeech::Ready:
        if (state == KPIMTextEdit::TextToSpeech::Ready) {
            d->mTextToSpeechActions->setState(TextToSpeechWidget::Stop);
            if (d->mConfigDialog) {
                d->mNeedToHide = true;
            } else {
                hide();
            }
        }
        break;
    default:
        //TODO
        break;
    }
}

void TextToSpeechWidget::setState(TextToSpeechWidget::State state)
{
    d->mTextToSpeechActions->setState(state);
}

void TextToSpeechWidget::setTextToSpeechInterface(AbstractTextToSpeechInterface *interface)
{
    delete d->mTextToSpeechInterface;
    d->mTextToSpeechInterface = interface;
    //Update volume value
    if (d->mTextToSpeechInterface) {
        d->mTextToSpeechInterface->reloadSettings();
        d->mVolume->setValue(interface->volume());
    }
}
