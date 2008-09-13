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


#ifndef EMAILMARKUPDIRECTOR_H
#define EMAILMARKUPDIRECTOR_H

/**
@brief The EmailMarkupdirector class controls and instructs an email builder object to create marked up emails.

The EmailMarkupdirector is used with a subclass of AbstractEmailMarkupBuilder to create a marked up email.

This class is similar in operation to MarkupDirector, and adds functionality to process quotes in emails. TODO: and attachments etc.

@author Stephen Kelly <steveire@gmail.com>
@since 4.2
*/

class EmailMarkupDirector : public MarkupDirector 
{
public:
    // Needed?
    EmailMarkupDirector(AbstractEmailMarkupBuilder *b) { d->builder = b; }

    virtual ~EmailMarkupDirector();
    
    void processAttachment()
    {
        // Handle this like an image? Or set up a EmailDocumentAttachment
    }
    
    //processSignature();

    /**
    Treats a series of quote paragraphs as a single block.
    */
    virtual void processBlock(const QTextBlock &block)
    {
        // Overriden to process quote blocks.
        
        EmailDocumentQuoteData *quoteData = (EmailDocumentQuoteData*)block.userData();
        
        if (quoteData)
        {
            // TODO: Make a EmailDocumentQuote subclass of QTextBlockGroup and handle this the same way as lists?
            if (quoteData.isPreBlock) // The 'Joe wrote:' before a quote.
            {
                processBlockContents(block);
            } else {
                beginQuote();
                
                QTextFrame::iterator start = it;

                QTextBlock nextBlock = block.next();
                while(nextBlock.isValid())
                {
                    if (nextBlock.userData() == quoteData)
                    {
                        ++it;
                    } else {
                        //Went one too far.
                        --it;
                        break;
                    }
                    nextBlock = block.next();
                }
                
                QTextFrame::iterator end = it;

                // Use this because the quote could contain tables, or other frames.
                processDocumentContents(start, end);
                endQuote();
            }
        } else {
            MarkupDirector::processBlock(block);
        }
        
    }
       
};

#endif
