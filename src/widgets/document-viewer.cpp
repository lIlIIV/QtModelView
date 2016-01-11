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
#include <QWheelEvent>

#include "widgets/clickable.hh"

DocumentViewer::DocumentViewer(QWidget *parent)
    : QWidget(parent), m_document(new QLabel), m_scroll_area(new QScrollArea),
        m_scale_factor(1.0)
{
    m_document->setText( tr("<i>No data.</i>") );
    m_document->setBackgroundRole(QPalette::Base);

    m_document->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    m_document->setScaledContents(true);

    //m_document->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    m_document->setAlignment( Qt::AlignCenter );

    m_scroll_area->setBackgroundRole(QPalette::Dark);
    m_scroll_area->setWidget(m_document);
    m_scroll_area->setAlignment(Qt::AlignCenter );

    QPixmap pixmap(":images/plus.png");
    Clickable * zoom_in = new Clickable(pixmap.scaled(15, 15, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    connect(zoom_in, &Clickable::clicked, this, &DocumentViewer::zoomIn);

    pixmap.load(":images/minus.png");
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
    // reader.setAutoTransform(true);
    // reader.setScaledSize(QSize());

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
    QPixmap pixmap = QPixmap::fromImage(image).scaledToWidth(std::min(640, std::max(300, image.width())));
    m_document->setPixmap(pixmap);
    m_scale_factor = 1.0;

    normalSize();

    //m_scroll_area->setWidgetResizable(true);

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
    if(m_document->pixmap() == nullptr)
    {
        return;
    }

    m_scale_factor *= factor;
    QSize current_size = m_document->pixmap()->size();
    m_document->resize(m_scale_factor * current_size);

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

void DocumentViewer::keyPressEvent(QKeyEvent * event)
{
    if( event->key() == Qt::Key_0 )
    {
        normalSize();
        event->accept();
        return;
    }

    if( event->key() == Qt::Key_Plus )
    {
        zoomIn();
        event->accept();
        return;
    }

    if( event->key() == Qt::Key_Minus )
    {
        zoomOut();
        event->accept();
        return;
    }

    event->ignore();



//    QPoint numPixels = event->pixelDelta();
//    QPoint numDegrees = event->angleDelta() / 8;



//    event->accept();
}
