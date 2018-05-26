#ifndef RECEPTIONISTVIEW_H
#define RECEPTIONISTVIEW_H

#include <QWidget>

namespace Ui {
class ReceptionistView;
}

class ReceptionistView : public QWidget
{
    Q_OBJECT

public:
    explicit ReceptionistView(QWidget *parent = 0);
    ~ReceptionistView();

private:
    Ui::ReceptionistView *ui;
};

#endif // RECEPTIONISTVIEW_H
