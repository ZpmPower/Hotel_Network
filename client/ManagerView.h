#ifndef MANAGERVIEW_H
#define MANAGERVIEW_H

#include <QWidget>

namespace Ui {
class ManagerView;
}

class ManagerView : public QWidget
{
    Q_OBJECT

public:
    explicit ManagerView(QWidget *parent = 0);
    ~ManagerView();

private:
    Ui::ManagerView *ui;
};

#endif // MANAGERVIEW_H
