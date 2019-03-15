/*
    This file is part of KDE.

    Copyright (C) 2019 Laurent Montel <montel@kde.org>

    This library is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This library is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
    License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to the
    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301, USA.
*/
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QTextEdit>

#include "textutils.h"

class Window : public QWidget
{
    Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        QTextEdit *edit = new QTextEdit(this);
        edit->setReadOnly(true);
        mainLayout->addWidget(edit);
        QString str;
        const QList<uint> lstEmoji = KPIMTextEdit::TextUtils::unicodeEmoji();
        for (uint emoji : lstEmoji) {
            str += QString::fromUcs4(&emoji, 1);
        }
        edit->setPlainText(str);
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Window window;

    window.show();
    return app.exec();
}

#include "showemojiunicode_gui.moc"
