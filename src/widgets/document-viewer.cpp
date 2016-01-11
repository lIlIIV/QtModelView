#include "document-viewer.hh"

#include <QApplication>
#include <QDir>
#include <QHBoxLayout>
#include <QImageReader>
#include <QMessageBox>
#include <QPixmap>
#include <QRect>
#include <QScrollBar>
#include <QVBoxLayout>

#include "widgets/clickable.hh"

DocumentViewer::DocumentViewer(QWidget *parent)
    : QWidget(parent), m_document(new QLabel), m_scroll_area(new QScrollArea),
        m_scale_factor(1.0)
{
    m_document->setText( tr("<i>No data.</i>") );
    m_document->setBackgroundRole(QPalette::Base);
    //m_document->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    //m_document->setScaledContents(true);
    m_document->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    m_document->setAlignment( Qt::AlignCenter );

    m_scroll_area->setBackgroundRole(QPalette::Dark);
    m_scroll_area->setWidget(m_document);
    m_scroll_area->setAlignment(Qt::AlignCenter );

    QPixmap pixmap(":images/uncompleted.png");
    Clickable * zoom_in = new Clickable(pixmap.scaled(15, 15, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    connect(zoom_in, &Clickable::clicked, this, &DocumentViewer::zoomIn);

    pixmap.load(":images/completed.png");
    Clickable * zoom_out = new Clickable(pixmap.scaled(15, 15, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    connect(zoom_out, &Clickable::clicked, this, &DocumentViewer::zoomOut);

    QHBoxLayout * h_layout = new QHBoxLayout();

    h_layout->addWidget(zoom_in);
    h_layout->addWidget(zoom_out);
    h_layout->addStretch();

    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->addLayout(h_layout);
    layout->addWidget(m_scroll_area);
}

bool DocumentViewer::loadDocument(const QString & file_name)
{
    QImageReader reader(file_name);
    reader.setAutoTransform(true);
    const QImage image = reader.read();
    if (image.isNull())
    {
        QMessageBox::information(this, QApplication::applicationDisplayName(),
                                 tr("Cannot load %1.").arg(QDir::toNativeSeparators(file_name)));
        //setWindowFilePath(QString());
        m_document->setPixmap(QPixmap());
        m_document->adjustSize();
        return false;
    }
    m_document->setPixmap(QPixmap::fromImage(image).scaledToHeight(800, Qt::SmoothTransformation));
    m_scale_factor = 1.0;

    normalSize();

//    fitToWindowAct->setEnabled(true);
//    updateActions();

//    if (!fitToWindowAct->isChecked())
//        imageLabel->adjustSize();

//    setWindowFilePath(fileName);
    return true;
}


void DocumentViewer::displayNoData()
{
    m_document->setText( tr("<i>No data.</i>") );
    m_document->adjustSize();
}

void DocumentViewer::normalSize()
{
    m_document->adjustSize();
    m_scale_factor = 1.0;
}

void DocumentViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}

void DocumentViewer::scaleImage(double factor)
{
    Q_ASSERT(m_document->pixmap());
    m_scale_factor *= factor;
    m_document->resize(m_scale_factor * m_document->pixmap()->size());

    adjustScrollBar(m_scroll_area->horizontalScrollBar(), factor);
    adjustScrollBar(m_scroll_area->verticalScrollBar(), factor);


}

void DocumentViewer::zoomIn()
{
    scaleImage(1.25);
}

void DocumentViewer::zoomOut()
{
    scaleImage(0.8);
}
