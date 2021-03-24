/*
   SPDX-FileCopyrightText: 2015-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "kpimtextedit_export.h"
#include <QFileInfo>
#include <QImage>
#include <QObject>
#include <QSharedPointer>
#include <QTextImageFormat>
namespace KPIMTextEdit
{
class RichTextComposer;
/**
 * Holds information about an embedded HTML image that will be useful for mail clients.
 * A list with all images can be retrieved with TextEdit::embeddedImages().
 */
struct EmbeddedImage {
    QByteArray image; ///< The image, encoded as PNG with base64 encoding
    QString contentID; ///< The content id of the embedded image
    QString imageName; ///< Name of the image as it is available as a resource in the editor
};

/**
 * Holds information about an embedded HTML image that will be generally useful.
 * A list with all images can be retrieved with TextEdit::imagesWithName().
 *
 * @since 4.4
 */
struct ImageWithName {
    QImage image; ///< The image
    QString name; ///< The name of the image as it is available as a resource in the editor
};

typedef QSharedPointer<ImageWithName> ImageWithNamePtr;
typedef QVector<ImageWithNamePtr> ImageWithNameList;
typedef QVector<QSharedPointer<EmbeddedImage>> ImageList;

class KPIMTEXTEDIT_EXPORT RichTextComposerImages : public QObject
{
    Q_OBJECT
public:
    explicit RichTextComposerImages(RichTextComposer *composer, QObject *parent = nullptr);
    ~RichTextComposerImages() override;

    /**
     * Adds an image. The image is loaded from file and then pasted to the current
     * cursor position with the given @p width and @p height.
     *
     * @param url The URL of the file which contains the image
     * @param width The width the inserted image will have.
     * @param height The height the inserted image will have.
     *
     */
    void addImage(const QUrl &url, int width = -1, int height = -1);

    /**
     * Loads an image into the textedit. The difference to addImage() is that this
     * function expects that the image tag is already present in the HTML source.
     *
     * @param image the image to load
     * @param matchName the name of tags to match image
     * @param resourceName the resource name of image
     * So what this message does is that it scans the HTML source for the image
     * tag that matches the @p matchName, and then inserts the @p image as a
     * resource, giving that resource the name @p resourceName.
     *
     */
    void loadImage(const QImage &image, const QString &matchName, const QString &resourceName);

    void addImageHelper(const QString &imageName, const QImage &image, int width = -1, int height = -1);
    Q_REQUIRED_RESULT ImageWithNameList imagesWithName() const;
    Q_REQUIRED_RESULT QVector<QSharedPointer<EmbeddedImage>> embeddedImages() const;
    Q_REQUIRED_RESULT QVector<QTextImageFormat> embeddedImageFormats() const;
    void addImageHelper(const QUrl &url, int width = -1, int height = -1);
    void insertImage(const QImage &image, const QFileInfo &fileInfo);

    /**
     * For all given embedded images, this function replace the image name
     * in the \<img\> tag of the HTML body with cid:content-id, so that the
     * HTML references the image body parts, see RFC 2557.
     *
     * This is useful when building a MIME message with inline images.
     *
     * Note that this function works on encoded content already.
     *
     * @param htmlBody the HTML code in which the \<img\> tag will be modified.
     *                 The HTML code here could come from toHtml(), for example.
     *
     * @param imageList the list of images of which the \<img\> tag will be modified.
     *                  You can get such a list from the embeddedImages() function.
     *
     * @return a modified HTML code, where the \<img\> tags got replaced
     */
    static Q_REQUIRED_RESULT QByteArray imageNamesToContentIds(const QByteArray &htmlBody, const ImageList &imageList);

    Q_REQUIRED_RESULT QSharedPointer<EmbeddedImage> createEmbeddedImage(const QImage &img, const QString &imageName) const;

private:
    class RichTextComposerImagesPrivate;
    RichTextComposerImagesPrivate *const d;
};
}

