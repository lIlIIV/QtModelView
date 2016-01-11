#ifndef DOCUMENT_VIEWER_HH
#define DOCUMENT_VIEWER_HH

#include <QLabel>
#include <QWidget>
#include <QScrollArea>

class DocumentViewer: public QWidget
{
    Q_OBJECT
public:
    DocumentViewer(QWidget * parent = nullptr);
    bool loadDocument(const QString & file_name);
    void displayNoData();
    void normalSize();

    void keyPressEvent(QKeyEvent * event) override;

private slots:
    void zoomIn();
    void zoomOut();

private:
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

    QLabel * m_document;
    QScrollArea * m_scroll_area;
    double m_scale_factor;

};

#endif // DOCUMENT_VIEWER_HH
