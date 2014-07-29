/*
    Copyright (c) 2009 Thomas McGuire <mcguire@kde.org>

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

#ifndef TEXTEDITTEST_H
#define TEXTEDITTEST_H

#include <qobject.h>

class TextEditTester : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testFormattingUsed();
    void testQuoting();
    void testCleanText();
    void testEnter();
    void testEnter_data();
    void testImages();
    void testImageHtmlCode();
    void testDeleteLine();
    void testDeleteLine_data();
    void testLoadImage();
    void testWrappedPlainText();
};

#endif
