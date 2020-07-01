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

#include "testgenerateplaintextbuildergui.h"
#include <QVBoxLayout>
#include <QApplication>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <KPIMTextEdit/PlainTextMarkupBuilder>
#include <KPIMTextEdit/MarkupDirector>

TestGeneratePlainTextBuilderGui::TestGeneratePlainTextBuilderGui(QWidget *parent)
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

    lab = new QLabel(QStringLiteral("Plaintext from Grantlee to TextEdit"), this);
    mainLayout->addWidget(lab);

    QTextEdit *generatedPlainTextFromGrantleeToTextEdit = new QTextEdit(this);
    generatedPlainTextFromGrantleeToTextEdit->setAcceptRichText(false);
    generatedPlainTextFromGrantleeToTextEdit->setReadOnly(true);
    mainLayout->addWidget(generatedPlainTextFromGrantleeToTextEdit);


    connect(generateHtmlFromQTextEditButton, &QPushButton::clicked, this, [=]{
        generatedHtml->setHtml(htmlEdit->toPlainText());

        auto hb = new KPIMTextEdit::PlainTextMarkupBuilder();
        auto md = new KPIMTextEdit::MarkupDirector(hb);
        md->processDocument(generatedHtml->document());
        auto result = hb->getResult();
        delete md;
        delete hb;
        generatedPlainTextFromGrantleeToTextEdit->setPlainText(result);
    });
}

TestGeneratePlainTextBuilderGui::~TestGeneratePlainTextBuilderGui()
{

}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TestGeneratePlainTextBuilderGui window;

    window.show();
    return app.exec();
}
