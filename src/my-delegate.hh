#ifndef MY_DELEGATE
#define MY_DELEGATE

#include <QSize>
#include <QStyledItemDelegate>

class MyDelegate: public QStyledItemDelegate
{
	Q_OBJECT
public:
    MyDelegate(QObject * parent = 0);

    void setPageNbPosition(double x, double y, double margin_x = 10, double margin_y = 10);

	void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const override;

	QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const override;

    QWidget * createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const override;

    void setEditorData(QWidget * editor, const QModelIndex & index) const override;

    void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const override;

    void updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const override;

private slots:
    void edit();

private:
    double m_page_nb_position_x;
    double m_page_nb_position_y;
    double m_page_nb_margin_x;
    double m_page_nb_margin_y;
};

#endif // MY_DELEGATE
