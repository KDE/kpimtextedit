/*
    This file is part of KDE.

    SPDX-FileCopyrightText: 2019-2020 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>

#include "textutils.h"
#include "emoticon/emoticonunicodetab.h"

class Window : public QWidget
{
    Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        KPIMTextEdit::EmoticonUnicodeTab *w = new KPIMTextEdit::EmoticonUnicodeTab(this);
        mainLayout->addWidget(w);
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Window window;

    window.show();
    return app.exec();
}

#include "showemoticonunicodetab_gui.moc"
