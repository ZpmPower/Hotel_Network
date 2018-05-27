#ifndef ADMINVIEW_H
#define ADMINVIEW_H

#include <QWidget>
#include "MessageManager.h"

namespace Ui {
class AdminView;
}

class AdminView : public QWidget
{
    Q_OBJECT

public:
    explicit AdminView(std::shared_ptr<MessageManager> message_manager, QWidget *parent = 0);
    ~AdminView();
    void onRead(const network::ResponseContext &response);

private slots:
    void on_okBtn_clicked();
    void on_tabWidget_currentChanged(int index);

private:
    Ui::AdminView *ui;
    std::shared_ptr<MessageManager> message_manager_;
};

#endif // ADMINVIEW_H
