/*
    This file is part of KDE.

    Copyright (c) 2008 Stephen Kelly <steveire@gmail.com>

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


#ifndef EMAILDOCUMENT_H
#define EMAILDOCUMENT_H

#include <QTextDocument>
#include <QTextFormat>

#include "emaildocumentattachment.h"

/**

@brief The EmailDocument class is a QTextDocument which can handle EmailDocumentAttachment classes.

This class makes inline attachments possible in KMEditor.

@author Stephen Kelly <steveire@gmail.com>
@since 4.2

*/
class EmailDocument : public QTextDocument
{
    public:
        EmailDocument() : QTextDocument() {  }
        EmailDocument(QTextDocument *doc) : QTextDocument(doc) {  }

protected:
    virtual QTextObject* createObject ( const QTextFormat &format ){

        
        if (format.type() == EmailDocumentAttachmentFormat::AttachmentType)
        {
            // TODO: figure out how to make this work.

            // http://lists.trolltech.com/qt-interest/2008-09/thread00216-0.html

//             http://lists.trolltech.com/qt-interest/2006-12/thread00195-0.html
//             http://lists.trolltech.com/qt4-preview-feedback/2004-12/thread00095-0.html
//             http://lists.trolltech.com/qt4-preview-feedback/2005-03/thread00034-0.html
//             http://lists.trolltech.com/qt4-preview-feedback/2006-01/thread00000-0.html
//             http://lists.trolltech.com/qt4-preview-feedback/2004-10/msg00202.html
// 
//             http://trolltech.com/developer/knowledgebase/faq.2008-03-05.5300306332/
            
            return new EmailDocumentAttachment(this); 
        }              
        
        return QTextDocument::createObject(format);
        
    }
    
    //TODO: Do I need to reimplemement loadResource as well for attachments etc? 
    //Probably need to reimplement it to handle image resources? or attachment resources?
    // http://doc.trolltech.com/4.4/qtextdocument.html#resource
};

#endif

