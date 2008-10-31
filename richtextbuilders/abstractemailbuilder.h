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


#ifndef ABSTRACTEMAILBUILDER_H
#define ABSTRACTEMAILBUILDER_H


#include "abstractmarkupbuilder.h"

/**
@brief Abstract class for building marked up email

This class works similarly to the AbstractMarkupBuilder class, and adds methods for handling of quote blocks and attachments.

A subclass of this can conveniently combined with a subclass of AbstractMarkupBuilder to supply most functionality. See RichEmailbuilder and PlainTextEmailBuilder.

@author Stephen Kelly <steveire@gmail.com>
@since 4.2

*/
class AbstractEmailBuilder : public virtual AbstractMarkupBuilder // Does this enforce that a subclass of this must be a subclass of AbstractMarkupBuilder?
{
    // Or name it EmailBuilderMixin?
// Instead of this, I'll push, pop a QStringList. Put the QStringList here? Or getCurrentIntroLine? Actually I don't need to do that at all. The document being processed is a one-time. I can just create the divs I want. I'll let the EmailMarkupDirector take care of anything. Though I don't think there's anything to take care of.
    // class Metadata {
    // QString line;
    // QString person;
    // QDateTime;
    // };

    // This class won't have anything to do with signature, will it? That might make sense to do in the EMailMarkupDirector actually.
    // virtual void insertSignature() {}

    //virtual void addQuotedFragment(data, metadata) {}


    /**
    Begin a quote block. This may create a styled div, or in the case of plain text, indicate that lines shoudl be prepended with a '>' character.

    Quote blocks may be nested.
    */
    virtual void beginQuote() { }

    /** End quote block */
    virtual void endQuote() { }

    // This works, but it isn't nice. I really should use processQuoteBlock.
    //virtual void processBlock()=0; // I want this to be reimplemented in each email builder subclass, so make it virtual again. Maybe a new pure virtual otherwise with processQuoteBlock.

    virtual void processQuoteBlock ( const QTextBlock &block ) = 0;
    /**
    Might be an (external?) image, another document as part of the multipart/related, or an attachment.
    */
    virtual void addEmbeddedDocument ( data ) {}


};

#endif

