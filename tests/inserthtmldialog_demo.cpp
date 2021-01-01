/*
    This file is part of KDE.

    SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "../src/inserthtmldialog.h"

#include <QApplication>

int main(int argv, char **argc)
{
    QApplication app(argv, argc);

    KPIMTextEdit::InsertHtmlDialog insertHtmlDialog;
    insertHtmlDialog.show();

    return app.exec();
}
