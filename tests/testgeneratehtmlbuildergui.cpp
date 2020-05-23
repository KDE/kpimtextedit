/*
    Copyright (C) 2020 Laurent Montel <montel@kde.org>

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

#include "testgeneratehtmlbuildergui.h"
#include <QVBoxLayout>
#include <QApplication>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <KPIMTextEdit/TextHTMLBuilder>
#include <KPIMTextEdit/MarkupDirector>

TestGenerateHTMLBuilderGui::TestGenerateHTMLBuilderGui(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QLabel *lab = new QLabel(QStringLiteral("html text"), this);
    mainLayout->addWidget(lab);

    QTextEdit *htmlEdit = new QTextEdit(this);
    htmlEdit->setAcceptRichText(false);
    mainLayout->addWidget(htmlEdit);

    QPushButton *generateHtmlFromQTextEditButton = new QPushButton(QStringLiteral("Generate HTML"), this);
    mainLayout->addWidget(generateHtmlFromQTextEditButton);

    lab = new QLabel(QStringLiteral("Generate Html"), this);
    mainLayout->addWidget(lab);

    QTextEdit *generatedHtml = new QTextEdit(this);
    generatedHtml->setAcceptRichText(false);
    generatedHtml->setReadOnly(true);
    mainLayout->addWidget(generatedHtml);

    lab = new QLabel(QStringLiteral("HTML from QTextEdit"), this);
    mainLayout->addWidget(lab);

    QTextEdit *generatedHtmlFromTextEdit = new QTextEdit(this);
    generatedHtmlFromTextEdit->setAcceptRichText(false);
    generatedHtmlFromTextEdit->setReadOnly(true);
    mainLayout->addWidget(generatedHtmlFromTextEdit);


    lab = new QLabel(QStringLiteral("HTML from Grantlee"), this);
    mainLayout->addWidget(lab);

    QTextEdit *generatedHtmlFromGrantlee = new QTextEdit(this);
    generatedHtmlFromGrantlee->setAcceptRichText(false);
    generatedHtmlFromGrantlee->setReadOnly(true);
    mainLayout->addWidget(generatedHtmlFromGrantlee);

    lab = new QLabel(QStringLiteral("HTML from Grantlee to TextEdit"), this);
    mainLayout->addWidget(lab);

    QTextEdit *generatedHtmlFromGrantleeToTextEdit = new QTextEdit(this);
    generatedHtmlFromGrantleeToTextEdit->setAcceptRichText(false);
    generatedHtmlFromGrantleeToTextEdit->setReadOnly(true);
    mainLayout->addWidget(generatedHtmlFromGrantleeToTextEdit);


    connect(generateHtmlFromQTextEditButton, &QPushButton::clicked, [=]{
        generatedHtml->setHtml(htmlEdit->toPlainText());
        generatedHtmlFromTextEdit->setPlainText(generatedHtml->document()->toHtml());

        auto hb = new KPIMTextEdit::TextHTMLBuilder();
        auto md = new KPIMTextEdit::MarkupDirector(hb);
        md->processDocument(generatedHtml->document());
        auto result = hb->getResult();
        delete md;
        delete hb;
        generatedHtmlFromGrantlee->setPlainText(result);
        generatedHtmlFromGrantleeToTextEdit->setHtml(result);
    });
}

TestGenerateHTMLBuilderGui::~TestGenerateHTMLBuilderGui()
{

}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TestGenerateHTMLBuilderGui window;

    window.show();
    return app.exec();
}
