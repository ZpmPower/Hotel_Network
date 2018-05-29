#ifndef AUTHREG_H
#define AUTHREG_H

#include <QWidget>
#include "MessageManager.h"
#include "GuestView.h"
#include "AdminView.h"
#include "ManagerView.h"
#include "ReceptionistView.h"
#include "RegisterView.h"

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
    void on_regBtn_clicked();
    void enableGb();
    void onReadData(std::string data);

signals:
    void readData(std::string);

private:
    Ui::authReg *ui;
    std::shared_ptr<MessageManager> message_manager_;
    std::shared_ptr<GuestView> guestView_;
    std::shared_ptr<AdminView> adminView_;
    std::shared_ptr<ManagerView> managerView_;
    std::shared_ptr<ReceptionistView> receptionistView_;
    std::shared_ptr<RegisterView> registerView_;

    void cannotConnectError();
    void disconnectError();

    void userAuth(const network::AuthMessageResponse& responce);
};

#endif // AUTHREG_H
