/*
   SPDX-FileCopyrightText: 2014-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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

class Q_DECL_HIDDEN KPIMTextEdit::TextToSpeechWidgetPrivate
{
public:
    TextToSpeechWidgetPrivate()
    {
    }

    QPointer<KPIMTextEdit::TextToSpeechConfigDialog> mConfigDialog;
    QToolButton *mStopButton = nullptr;
    QToolButton *mPlayPauseButton = nullptr;
    QToolButton *mConfigureButton = nullptr;
    AbstractTextToSpeechInterface *mTextToSpeechInterface = nullptr;
    TextToSpeechActions *mTextToSpeechActions = nullptr;
    QSlider *mVolume = nullptr;
    bool mNeedToHide = false;
};

TextToSpeechWidget::TextToSpeechWidget(QWidget *parent)
    : QWidget(parent)
    , d(new KPIMTextEdit::TextToSpeechWidgetPrivate)
{
    auto *hbox = new QHBoxLayout(this);

    d->mTextToSpeechActions = new TextToSpeechActions(this);
    connect(d->mTextToSpeechActions, &TextToSpeechActions::stateChanged, this, &TextToSpeechWidget::stateChanged);

    auto *close = new QToolButton(this);
    close->setObjectName(QStringLiteral("close-button"));
    close->setIcon(QIcon::fromTheme(QStringLiteral("dialog-close")));
    close->setToolTip(i18n("Close"));
    connect(close, &QToolButton::clicked, this, &TextToSpeechWidget::slotCloseTextToSpeechWidget);
    hbox->addWidget(close);
    hbox->addStretch(0);

    QLabel *volume = new QLabel(i18n("Volume:"), this);
    hbox->addWidget(volume);
    d->mVolume = new QSlider;
    d->mVolume->setMinimumWidth(100);
    d->mVolume->setOrientation(Qt::Horizontal);
    d->mVolume->setObjectName(QStringLiteral("volumeslider"));
    d->mVolume->setRange(0, 100);
    d->mVolume->setTickPosition(QSlider::TicksBelow);
    connect(d->mVolume, &QSlider::valueChanged, this, &TextToSpeechWidget::slotVolumeChanged);
    hbox->addWidget(d->mVolume);

    d->mStopButton = new QToolButton(this);
    d->mStopButton->setObjectName(QStringLiteral("stopbutton"));
    d->mStopButton->setDefaultAction(d->mTextToSpeechActions->stopAction());
    hbox->addWidget(d->mStopButton);

    d->mPlayPauseButton = new QToolButton(this);
    d->mPlayPauseButton->setObjectName(QStringLiteral("playpausebutton"));
    d->mPlayPauseButton->setDefaultAction(d->mTextToSpeechActions->playPauseAction());
    hbox->addWidget(d->mPlayPauseButton);

    d->mConfigureButton = new QToolButton(this);
    d->mConfigureButton->setIcon(QIcon::fromTheme(QStringLiteral("configure")));
    d->mConfigureButton->setToolTip(i18n("Configure..."));
    d->mConfigureButton->setObjectName(QStringLiteral("configurebutton"));
    connect(d->mConfigureButton, &QToolButton::clicked, this, &TextToSpeechWidget::slotConfigure);
    hbox->addWidget(d->mConfigureButton);

    setTextToSpeechInterface(new TextToSpeechInterface(this, this));
    setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
    hide();
}

TextToSpeechWidget::~TextToSpeechWidget()
{
    delete d;
}

void TextToSpeechWidget::slotCloseTextToSpeechWidget()
{
    d->mTextToSpeechActions->slotStop();
    hide();
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
