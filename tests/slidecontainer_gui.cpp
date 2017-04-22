/*
code based on source from Gwenview
Copyright 2007 Aurélien Gâteau <agateau@kde.org>

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
// Qt
#include <QApplication>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

// Local
#include "kpimtextedit/slidecontainer.h"

using namespace KPIMTextEdit;

class Window : public QWidget
{
    Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        SlideContainer *container = new SlideContainer(this);

        QPushButton *inButton = new QPushButton(this);
        inButton->setText(QStringLiteral("Slide &In"));
        connect(inButton, &QPushButton::clicked, container, &SlideContainer::slideIn);

        QPushButton *outButton = new QPushButton(this);
        outButton->setText(QStringLiteral("Slide &Out"));
        connect(outButton, &QPushButton::clicked, container, &SlideContainer::slideOut);

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(inButton);
        layout->addWidget(outButton);
        layout->addWidget(container);

        QLineEdit *content = new QLineEdit(container);
        content->setText(QStringLiteral("Some long text. Some long text. Some long text. Some long text."));
        container->setContent(content);
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Window window;

    window.show();
    return app.exec();
}

#include "slidecontainer_gui.moc"
