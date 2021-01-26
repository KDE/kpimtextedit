/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "richtextcomposerimages.h"
#include "richtextcomposer.h"

#include <KCodecs>
#include <KLocalizedString>
#include <KMessageBox>
#include <QBuffer>
#include <QRandomGenerator>
#include <QTextBlock>
#include <QTextDocument>

using namespace KPIMTextEdit;

class Q_DECL_HIDDEN RichTextComposerImages::RichTextComposerImagesPrivate
{
public:
    RichTextComposerImagesPrivate(RichTextComposer *editor)
        : composer(editor)
    {
    }

    /**
     * The names of embedded images.
     * Used to easily obtain the names of the images.
     * New images are compared to the list and not added as resource if already present.
     */
    QStringList mImageNames;

    RichTextComposer *const composer;
};

RichTextComposerImages::RichTextComposerImages(RichTextComposer *composer, QObject *parent)
    : QObject(parent)
    , d(new RichTextComposerImages::RichTextComposerImagesPrivate(composer))
{
}

RichTextComposerImages::~RichTextComposerImages()
{
    delete d;
}

void RichTextComposerImages::addImage(const QUrl &url, int width, int height)
{
    addImageHelper(url, width, height);
}

void RichTextComposerImages::addImageHelper(const QUrl &url, int width, int height)
{
    QImage image;
    if (!image.load(url.path())) {
        KMessageBox::error(d->composer, xi18nc("@info", "Unable to load image <filename>%1</filename>.", url.path()));
        return;
    }
    const QFileInfo fi(url.path());
    const QString imageName = fi.baseName().isEmpty() ? QStringLiteral("image.png") : QString(fi.baseName() + QLatin1String(".png"));
    if (width != -1 && height != -1 && (image.width() > width && image.height() > height)) {
        image = image.scaled(width, height);
    }
    addImageHelper(imageName, image, width, height);
}

void RichTextComposerImages::loadImage(const QImage &image, const QString &matchName, const QString &resourceName)
{
    QSet<int> cursorPositionsToSkip;
    QTextBlock currentBlock = d->composer->document()->begin();
    QTextBlock::iterator it;
    while (currentBlock.isValid()) {
        for (it = currentBlock.begin(); !it.atEnd(); ++it) {
            QTextFragment fragment = it.fragment();
            if (fragment.isValid()) {
                QTextImageFormat imageFormat = fragment.charFormat().toImageFormat();
                if (imageFormat.isValid() && imageFormat.name() == matchName) {
                    int pos = fragment.position();
                    if (!cursorPositionsToSkip.contains(pos)) {
                        QTextCursor cursor(d->composer->document());
                        cursor.setPosition(pos);
                        cursor.setPosition(pos + 1, QTextCursor::KeepAnchor);
                        cursor.removeSelectedText();
                        d->composer->document()->addResource(QTextDocument::ImageResource, QUrl(resourceName), QVariant(image));
                        QTextImageFormat format;
                        format.setName(resourceName);
                        if ((imageFormat.width() != 0.0) && (imageFormat.height() != 0.0)) {
                            format.setWidth(imageFormat.width());
                            format.setHeight(imageFormat.height());
                        }
                        cursor.insertImage(format);

                        // The textfragment iterator is now invalid, restart from the beginning
                        // Take care not to replace the same fragment again, or we would be in
                        // an infinite loop.
                        cursorPositionsToSkip.insert(pos);
                        it = currentBlock.begin();
                    }
                }
            }
        }
        currentBlock = currentBlock.next();
    }
}

void RichTextComposerImages::addImageHelper(const QString &imageName, const QImage &image, int width, int height)
{
    QString imageNameToAdd = imageName;
    QTextDocument *document = d->composer->document();

    // determine the imageNameToAdd
    int imageNumber = 1;
    while (d->mImageNames.contains(imageNameToAdd)) {
        QVariant qv = document->resource(QTextDocument::ImageResource, QUrl(imageNameToAdd));
        if (qv == image) {
            // use the same name
            break;
        }
        const int firstDot = imageName.indexOf(QLatin1Char('.'));
        if (firstDot == -1) {
            imageNameToAdd = imageName + QString::number(imageNumber++);
        } else {
            imageNameToAdd = imageName.left(firstDot) + QString::number(imageNumber++) + imageName.mid(firstDot);
        }
    }

    if (!d->mImageNames.contains(imageNameToAdd)) {
        document->addResource(QTextDocument::ImageResource, QUrl(imageNameToAdd), image);
        d->mImageNames << imageNameToAdd;
    }
    if (width != -1 && height != -1) {
        QTextImageFormat format;
        format.setName(imageNameToAdd);
        format.setWidth(width);
        format.setHeight(height);
        d->composer->textCursor().insertImage(format);
    } else {
        d->composer->textCursor().insertImage(imageNameToAdd);
    }
    d->composer->activateRichText();
}

ImageWithNameList RichTextComposerImages::imagesWithName() const
{
    ImageWithNameList retImages;
    QStringList seenImageNames;
    const QVector<QTextImageFormat> imageFormats = embeddedImageFormats();
    for (const QTextImageFormat &imageFormat : imageFormats) {
        const QString name = imageFormat.name();
        if (!seenImageNames.contains(name)) {
            QVariant resourceData = d->composer->document()->resource(QTextDocument::ImageResource, QUrl(name));
            QImage image = qvariant_cast<QImage>(resourceData);

            ImageWithNamePtr newImage(new ImageWithName);
            newImage->image = image;
            newImage->name = name;
            retImages.append(newImage);
            seenImageNames.append(name);
        }
    }
    return retImages;
}

QVector<QSharedPointer<EmbeddedImage>> RichTextComposerImages::embeddedImages() const
{
    const ImageWithNameList normalImages = imagesWithName();
    QVector<QSharedPointer<EmbeddedImage>> retImages;
    retImages.reserve(normalImages.count());
    for (const ImageWithNamePtr &normalImage : normalImages) {
        retImages.append(createEmbeddedImage(normalImage->image, normalImage->name));
    }
    return retImages;
}

QSharedPointer<EmbeddedImage> RichTextComposerImages::createEmbeddedImage(const QImage &img, const QString &imageName) const
{
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    img.save(&buffer, "PNG");

    QSharedPointer<EmbeddedImage> embeddedImage(new EmbeddedImage());
    embeddedImage->image = KCodecs::Codec::codecForName("base64")->encode(buffer.buffer());
    embeddedImage->imageName = imageName;
    embeddedImage->contentID = QStringLiteral("%1@KDE").arg(QRandomGenerator::global()->generate64());
    return embeddedImage;
}

QVector<QTextImageFormat> RichTextComposerImages::embeddedImageFormats() const
{
    QTextDocument *doc = d->composer->document();
    QVector<QTextImageFormat> retList;

    QTextBlock currentBlock = doc->begin();
    while (currentBlock.isValid()) {
        for (QTextBlock::iterator it = currentBlock.begin(); !it.atEnd(); ++it) {
            QTextFragment fragment = it.fragment();
            if (fragment.isValid()) {
                QTextImageFormat imageFormat = fragment.charFormat().toImageFormat();
                if (imageFormat.isValid()) {
                    // TODO: Replace with a way to see if an image is an embedded image or a remote
                    const QUrl url(imageFormat.name());
                    if (!url.isValid() || !url.scheme().startsWith(QLatin1String("http"))) {
                        retList.append(imageFormat);
                    }
                }
            }
        }
        currentBlock = currentBlock.next();
    }
    return retList;
}

void RichTextComposerImages::insertImage(const QImage &image, const QFileInfo &fileInfo)
{
    const QString imageName = fileInfo.baseName().isEmpty() ? i18nc("Start of the filename for an image", "image") : fileInfo.baseName();
    addImageHelper(imageName, image);
}

QByteArray RichTextComposerImages::imageNamesToContentIds(const QByteArray &htmlBody, const KPIMTextEdit::ImageList &imageList)
{
    QByteArray result = htmlBody;
    for (const QSharedPointer<EmbeddedImage> &image : imageList) {
        const QString newImageName = QLatin1String("cid:") + image->contentID;
        QByteArray quote("\"");
        result.replace(QByteArray(quote + image->imageName.toLocal8Bit() + quote), QByteArray(quote + newImageName.toLocal8Bit() + quote));
    }
    return result;
}
