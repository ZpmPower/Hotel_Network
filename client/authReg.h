#ifndef AUTHREG_H
#define AUTHREG_H

#include <QWidget>
#include "MessageManager.h"

namespace Ui {
class authReg;
}

class authReg : public QWidget
{
    Q_OBJECT

public:
    explicit authReg(std::shared_ptr<MessageManager> message_manager, QWidget *parent = 0);
    ~authReg();
    void onError(ClientError error);
    void onRead(const network::ResponseContext &response);

private slots:
    void on_testBtn_clicked();
    void updateTime();
    void on_testBtn_2_clicked();

private:
    Ui::authReg *ui;
    std::shared_ptr<MessageManager> message_manager_;

    void cannotConnectError();
    void disconnectError();

    void userRegister(const network::RegisterMessageResponse& response);
    void userAuth(const network::AuthMessageResponse& responce);
    QTimer* timer_;
};

#endif // AUTHREG_H
