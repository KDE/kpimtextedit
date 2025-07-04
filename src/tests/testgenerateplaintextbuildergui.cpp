/*
    SPDX-FileCopyrightText: 2020-2025 Laurent Montel <montel@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "testgenerateplaintextbuildergui.h"
using namespace Qt::Literals::StringLiterals;

#include <KPIMTextEdit/MarkupDirector>
#include <KPIMTextEdit/PlainTextMarkupBuilder>
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

TestGeneratePlainTextBuilderGui::TestGeneratePlainTextBuilderGui(QWidget *parent)
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

    lab = new QLabel(u"Plaintext from Grantlee to TextEdit"_s, this);
    mainLayout->addWidget(lab);

    auto generatedPlainTextFromGrantleeToTextEdit = new QTextEdit(this);
    generatedPlainTextFromGrantleeToTextEdit->setAcceptRichText(false);
    generatedPlainTextFromGrantleeToTextEdit->setReadOnly(true);
    mainLayout->addWidget(generatedPlainTextFromGrantleeToTextEdit);

    connect(generateHtmlFromQTextEditButton, &QPushButton::clicked, this, [generatedHtml, htmlEdit, generatedPlainTextFromGrantleeToTextEdit] {
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

TestGeneratePlainTextBuilderGui::~TestGeneratePlainTextBuilderGui() = default;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TestGeneratePlainTextBuilderGui window;

    window.show();
    return app.exec();
}

#include "moc_testgenerateplaintextbuildergui.cpp"
