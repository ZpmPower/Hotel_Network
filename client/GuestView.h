#ifndef GUESTVIEW_H
#define GUESTVIEW_H

#include <QWidget>

namespace Ui {
class GuestView;
}

class GuestView : public QWidget
{
    Q_OBJECT

public:
    explicit GuestView(QWidget *parent = 0);
    ~GuestView();

private:
    Ui::GuestView *ui;
};

#endif // GUESTVIEW_H
