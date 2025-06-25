/*
    SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "testgeneratehtmlbuildergui.h"
using namespace Qt::Literals::StringLiterals;

#include <KPIMTextEdit/MarkupDirector>
#include <KPIMTextEdit/TextHTMLBuilder>
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

TestGenerateHTMLBuilderGui::TestGenerateHTMLBuilderGui(QWidget *parent)
    : QWidget(parent)
{
    auto mainLayout = new QVBoxLayout(this);
    auto lab = new QLabel(u"html text"_s, this);
    mainLayout->addWidget(lab);

    auto htmlEdit = new QTextEdit(this);
    htmlEdit->setAcceptRichText(false);
    mainLayout->addWidget(htmlEdit);

    auto generateHtmlFromQTextEditButton = new QPushButton(u"Generate HTML"_s, this);
    mainLayout->addWidget(generateHtmlFromQTextEditButton);

    lab = new QLabel(u"Generate Html"_s, this);
    mainLayout->addWidget(lab);

    auto generatedHtml = new QTextEdit(this);
    generatedHtml->setAcceptRichText(false);
    generatedHtml->setReadOnly(true);
    mainLayout->addWidget(generatedHtml);

    lab = new QLabel(u"HTML from QTextEdit"_s, this);
    mainLayout->addWidget(lab);

    auto generatedHtmlFromTextEdit = new QTextEdit(this);
    generatedHtmlFromTextEdit->setAcceptRichText(false);
    generatedHtmlFromTextEdit->setReadOnly(true);
    mainLayout->addWidget(generatedHtmlFromTextEdit);

    lab = new QLabel(u"HTML from Grantlee"_s, this);
    mainLayout->addWidget(lab);

    auto generatedHtmlFromGrantlee = new QTextEdit(this);
    generatedHtmlFromGrantlee->setAcceptRichText(false);
    generatedHtmlFromGrantlee->setReadOnly(true);
    mainLayout->addWidget(generatedHtmlFromGrantlee);

    lab = new QLabel(u"HTML from Grantlee to TextEdit"_s, this);
    mainLayout->addWidget(lab);

    auto generatedHtmlFromGrantleeToTextEdit = new QTextEdit(this);
    generatedHtmlFromGrantleeToTextEdit->setAcceptRichText(false);
    generatedHtmlFromGrantleeToTextEdit->setReadOnly(true);
    mainLayout->addWidget(generatedHtmlFromGrantleeToTextEdit);

    connect(generateHtmlFromQTextEditButton,
            &QPushButton::clicked,
            this,
            [generatedHtml, htmlEdit, generatedHtmlFromTextEdit, generatedHtmlFromGrantlee, generatedHtmlFromGrantleeToTextEdit] {
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

TestGenerateHTMLBuilderGui::~TestGenerateHTMLBuilderGui() = default;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TestGenerateHTMLBuilderGui window;

    window.show();
    return app.exec();
}

#include "moc_testgeneratehtmlbuildergui.cpp"
