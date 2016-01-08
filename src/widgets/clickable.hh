#ifndef CLICKABLE_HH
#define CLICKABLE_HH

#include <QLabel>
#include <QPixmap>

class Clickable : public QLabel
{
Q_OBJECT
public:
    explicit Clickable(const QPixmap & image, QWidget* parent=0 );
    ~Clickable();

    bool isSaved()
    {
        return saved_;
    }
    void Saved(bool isSaved)
    {
        saved_ = isSaved;
    }

signals:
    void clicked();
protected:
    void mousePressEvent(QMouseEvent *);
private:
    bool saved_;
};

#endif // CLICKABLE_HH
