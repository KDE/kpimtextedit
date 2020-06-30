/*
    SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
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


    connect(generateHtmlFromQTextEditButton, &QPushButton::clicked, this, [=]{
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
