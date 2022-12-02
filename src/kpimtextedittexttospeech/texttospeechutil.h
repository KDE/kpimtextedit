/*
   SPDX-FileCopyrightText: 2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "kpimtextedittexttospeech_private_export.h"
#include <QObject>
namespace KPIMTextEditTextToSpeech
{
namespace TextToSpeechUtil
{
KPIMTEXTEDITTEXTTOSPEECH_TESTS_EXPORT Q_REQUIRED_RESULT QString textToSpeechConfigFileName();
KPIMTEXTEDITTEXTTOSPEECH_TESTS_EXPORT Q_REQUIRED_RESULT QString textToSpeechConfigGroupName();
};
}
