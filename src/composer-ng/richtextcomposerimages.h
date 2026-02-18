/*
   SPDX-FileCopyrightText: 2015-2026 Laurent Montel <montel@kde.org>

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
/*!
 * Holds information about an embedded HTML image that will be useful for mail clients.
 * A list with all images can be retrieved with TextEdit::embeddedImages().
 */
struct EmbeddedImage {
    QByteArray image; ///< The image, encoded as PNG with base64 encoding
    QString contentID; ///< The content id of the embedded image
    QString imageName; ///< Name of the image as it is available as a resource in the editor
};

/*!
 * Holds information about an embedded HTML image that will be generally useful.
 * A list with all images can be retrieved with TextEdit::imagesWithName().
 *
 * \since 4.4
 */
struct ImageWithName {
    QImage image; ///< The image
    QString name; ///< The name of the image as it is available as a resource in the editor
};

using ImageWithNamePtr = QSharedPointer<ImageWithName>;
using ImageWithNameList = QList<ImageWithNamePtr>;
using ImageList = QList<QSharedPointer<EmbeddedImage>>;

/*!
 * \class KPIMTextEdit::RichTextComposerImages
 * \inmodule KPIMTextedit
 * \inheaderfile KPIMTextEdit/RichTextComposerImages
 *
 * \brief The RichTextComposerImages class manages embedded images in rich text documents.
 */
class KPIMTEXTEDIT_EXPORT RichTextComposerImages : public QObject
{
    Q_OBJECT
public:
    /*!
     * Constructs a RichTextComposerImages object.
     *
     * \param composer The RichTextComposer instance
     * \param parent The parent QObject
     */
    explicit RichTextComposerImages(RichTextComposer *composer, QObject *parent = nullptr);
    /*!
     * Destroys the RichTextComposerImages object.
     */
    ~RichTextComposerImages() override;

    /*!
     * Adds an image. The image is loaded from file and then pasted to the current
     * cursor position with the given \a width and \a height.
     *
     * \a url The URL of the file which contains the image
     * \a width The width the inserted image will have.
     * \a height The height the inserted image will have.
     *
     */
    void addImage(const QUrl &url, int width = -1, int height = -1);

    /*!
     * Loads an image into the textedit. The difference to addImage() is that this
     * function expects that the image tag is already present in the HTML source.
     *
     * \a image the image to load
     * \a matchName the name of tags to match image
     * \a resourceName the resource name of image
     * So what this message does is that it scans the HTML source for the image
     * tag that matches the \a matchName, and then inserts the \a image as a
     * resource, giving that resource the name \a resourceName.
     *
     */
    void loadImage(const QImage &image, const QString &matchName, const QString &resourceName);

    /*!
     * Helper function to add an image with the given name.
     *
     * \param imageName The name of the image
     * \param image The image to add
     * \param width The width of the image, or -1 for default
     * \param height The height of the image, or -1 for default
     */
    void addImageHelper(const QString &imageName, const QImage &image, int width = -1, int height = -1);
    /*!
     * Returns a list of all images with their names.
     *
     * \return A list of ImageWithNamePtr objects
     */
    [[nodiscard]] ImageWithNameList imagesWithName() const;
    /*!
     * Returns a list of all embedded images.
     *
     * \return A list of embedded image pointers
     */
    [[nodiscard]] QList<QSharedPointer<EmbeddedImage>> embeddedImages() const;
    /*!
     * Returns a list of all embedded image formats.
     *
     * \return A list of QTextImageFormat objects
     */
    [[nodiscard]] QList<QTextImageFormat> embeddedImageFormats() const;
    /*!
     * Helper function to add an image from a URL.
     *
     * \param url The URL of the image file
     * \param width The width of the image, or -1 for default
     * \param height The height of the image, or -1 for default
     */
    void addImageHelper(const QUrl &url, int width = -1, int height = -1);
    /*!
     * Inserts an image from file information.
     *
     * \param image The image to insert
     * \param fileInfo The file information for the image
     */
    void insertImage(const QImage &image, const QFileInfo &fileInfo);

    /*!
     * For all given embedded images, this function replace the image name
     * in the \<img\> tag of the HTML body with cid:content-id, so that the
     * HTML references the image body parts, see RFC 2557.
     *
     * This is useful when building a MIME message with inline images.
     *
     * Note that this function works on encoded content already.
     *
     * \a htmlBody the HTML code in which the \<img\> tag will be modified.
     *                 The HTML code here could come from toHtml(), for example.
     *
     * \a imageList the list of images of which the \<img\> tag will be modified.
     *                  You can get such a list from the embeddedImages() function.
     *
     * Returns a modified HTML code, where the \<img\> tags got replaced
     */
    [[nodiscard]] static QByteArray imageNamesToContentIds(const QByteArray &htmlBody, const ImageList &imageList);

    /*!
     * Creates an embedded image from a QImage.
     *
     * \param img The image to embed
     * \param imageName The name of the image
     * \return A shared pointer to the created EmbeddedImage
     */
    [[nodiscard]] QSharedPointer<EmbeddedImage> createEmbeddedImage(const QImage &img, const QString &imageName) const;

private:
    class RichTextComposerImagesPrivate;
    std::unique_ptr<RichTextComposerImagesPrivate> const d;
};
}
