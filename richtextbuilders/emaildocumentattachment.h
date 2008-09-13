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


#ifndef EMAILDOCUMENTATTACHMENT_H
#define EMAILDOCUMENTATTACHMENT_H

/**

@brief The EmailDocumentAttachment class represents an attachment in a KPIM::EmailDocument.

An EmailDocumentAttachment is a TextObject which can be moved around as part of inline text.
The attachment is represented in the document by the relevant mimetype icon with a border around it.

@author Stephen Kelly <steveire@gmail.com>
@since 4.2

*/
class EmailDocumentAttachment : public QTextObject 
{

};

/**

@brief The EmailDocumentAttachmentFormat class sets the formatted representation of an attachment in a KPIM::EmailDocument.

The attachment is represented in the document by the relevant mimetype icon with a border around it.

This class exists to be used in the EmailDocument::createObject factory method.

@author Stephen Kelly <steveire@gmail.com>
@since 4.2

*/
class EmailDocumentAttachmentFormat : public QTextObjectFormat 
{

};

#endif