#ifndef REGISTERVIEW_H
#define REGISTERVIEW_H

#include <QWidget>
#include <QCloseEvent>
#include "MessageManager.h"

namespace Ui {
class RegisterView;
}

class RegisterView : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterView(std::shared_ptr<MessageManager> message_manager, QWidget *parent = 0);
    ~RegisterView();

private slots:
    void on_testBtn_2_clicked();
signals:
    void enableGb();
private:
    Ui::RegisterView *ui;
    std::shared_ptr<MessageManager> message_manager_;
    void userRegister(const network::RegisterMessageResponse& response);

    void onRead(const network::ResponseContext & response);
    void closeEvent(QCloseEvent *bar);
};

#endif // REGISTERVIEW_H
