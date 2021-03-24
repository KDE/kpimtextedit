/*
   SPDX-FileCopyrightText: 2014-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "kpimtextedit_export.h"
#include <QObject>
namespace KPIMTextEdit
{
/**
 * @brief The AbstractTextToSpeechInterface class
 * @author Laurent Montel <montel@kde.org>
 */
class KPIMTEXTEDIT_EXPORT AbstractTextToSpeechInterface : public QObject
{
    Q_OBJECT
public:
    explicit AbstractTextToSpeechInterface(QObject *parent = nullptr);
    ~AbstractTextToSpeechInterface() override;

    virtual Q_REQUIRED_RESULT bool isReady() const;
    virtual void say(const QString &text);
    virtual Q_REQUIRED_RESULT int volume() const;
    virtual void setVolume(int value);
    virtual void reloadSettings();
};
}

