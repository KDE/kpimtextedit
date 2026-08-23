/*
   SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "richtextcomposerimages.h"
using namespace Qt::Literals::StringLiterals;

#include "richtextcomposer.h"

#include <KCodecs>
#include <KLocalizedString>
#include <KMessageBox>
#include <QBuffer>
#include <QRandomGenerator>
#include <QStringConverter>
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

RichTextComposerImages::~RichTextComposerImages() = default;

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
    const QString imageName = fi.baseName().isEmpty() ? u"image.png"_s : QString(fi.baseName() + ".png"_L1);
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
            const QTextFragment fragment = it.fragment();
            if (fragment.isValid()) {
                const QTextImageFormat imageFormat = fragment.charFormat().toImageFormat();
                if (imageFormat.isValid() && imageFormat.name() == matchName) {
                    const int pos = fragment.position();
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
        const QVariant qv = document->resource(QTextDocument::ImageResource, QUrl(imageNameToAdd));
        if (qv == image) {
            // use the same name
            break;
        }
        const int firstDot = imageName.indexOf(u'.');
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
    const QList<QTextImageFormat> imageFormats = embeddedImageFormats();
    for (const QTextImageFormat &imageFormat : imageFormats) {
        QString name = imageFormat.name();
        if (!seenImageNames.contains(name)) {
            const QVariant resourceData = d->composer->document()->resource(QTextDocument::ImageResource, QUrl(name));
            auto image = qvariant_cast<QImage>(resourceData);

            ImageWithNamePtr newImage(new ImageWithName);
            newImage->image = std::move(image);
            newImage->name = name;
            retImages.append(std::move(newImage));
            seenImageNames.append(std::move(name));
        }
    }
    return retImages;
}

QList<QSharedPointer<EmbeddedImage>> RichTextComposerImages::embeddedImages() const
{
    const ImageWithNameList normalImages = imagesWithName();
    QList<QSharedPointer<EmbeddedImage>> retImages;
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
    embeddedImage->contentID = u"%1@KDE"_s.arg(QRandomGenerator::global()->generate64());
    return embeddedImage;
}

QList<QTextImageFormat> RichTextComposerImages::embeddedImageFormats() const
{
    const QTextDocument *doc = d->composer->document();
    QList<QTextImageFormat> retList;

    QTextBlock currentBlock = doc->begin();
    while (currentBlock.isValid()) {
        for (QTextBlock::iterator it = currentBlock.begin(); !it.atEnd(); ++it) {
            const QTextFragment fragment = it.fragment();
            if (fragment.isValid()) {
                const QTextImageFormat imageFormat = fragment.charFormat().toImageFormat();
                if (imageFormat.isValid()) {
                    // TODO: Replace with a way to see if an image is an embedded image or a remote
                    const QUrl url(imageFormat.name());
                    if (!url.isValid() || !url.scheme().startsWith("http"_L1)) {
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

namespace
{
// htmlBody was encoded by the caller with the charset of the message, so the image name has to be
// encoded the same way, otherwise a name with non-ascii characters is never found and the <img> tag
// keeps pointing to the resource name instead of the content id.
// The charset is not always known by the caller, and it can be a charset which Qt doesn't support,
// so the encodings which the html body is likely to use are tried too. A name encoded with the wrong
// charset simply doesn't match anything.
QList<QByteArray> encodedImageNames(const QString &imageName, const QByteArray &charset)
{
    QList<QByteArray> names;
    const auto addName = [&names, &imageName](QStringEncoder encoder) {
        if (!encoder.isValid()) {
            return;
        }
        QByteArray name = encoder.encode(imageName);
        // An unencodable character is replaced by a placeholder, such a name would match the wrong <img> tag.
        if (encoder.hasError() || name.isEmpty() || names.contains(name)) {
            return;
        }
        names.append(std::move(name));
    };
    if (!charset.isEmpty()) {
        addName(QStringEncoder(charset.constData()));
    }
    addName(QStringEncoder(QStringEncoder::Utf8));
    addName(QStringEncoder(QStringEncoder::System));
    addName(QStringEncoder(QStringEncoder::Latin1));
    return names;
}
}

QByteArray RichTextComposerImages::imageNamesToContentIds(const QByteArray &htmlBody, const KPIMTextEdit::ImageList &imageList)
{
    return imageNamesToContentIds(htmlBody, imageList, {});
}

QByteArray RichTextComposerImages::imageNamesToContentIds(const QByteArray &htmlBody, const KPIMTextEdit::ImageList &imageList, const QByteArray &charset)
{
    QByteArray result = htmlBody;
    const QByteArray quote("\"");
    for (const QSharedPointer<EmbeddedImage> &image : imageList) {
        // The name is quoted so that a name which is a substring of another one is not replaced.
        const QByteArray newImageName = quote + "cid:" + image->contentID.toLatin1() + quote;
        const QList<QByteArray> oldImageNames = encodedImageNames(image->imageName, charset);
        for (const QByteArray &oldImageName : oldImageNames) {
            result.replace(QByteArray(quote + oldImageName + quote), newImageName);
        }
    }
    return result;
}

#include "moc_richtextcomposerimages.cpp"
