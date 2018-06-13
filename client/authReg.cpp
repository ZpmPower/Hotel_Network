#include "authReg.h"
#include "ui_authReg.h"
#include "GlobalParams.h"
#include "QMessageBox"
#include "Helper.h"

authReg::authReg(std::shared_ptr<MessageManager> message_manager, QWidget *parent) :
    message_manager_(message_manager),
    QWidget(parent), ui(new Ui::authReg)
    {
    qRegisterMetaType<std::string>("std::string");
    ui->setupUi(this);
    if(message_manager)
    {
        message_manager_->setOnErrorCB(std::bind(&authReg::onError, this, std::placeholders::_1));
        message_manager->setOnReadCB(std::bind(&authReg::onRead, this, std::placeholders::_1));
    }
    connect(this, SIGNAL(readData(std::string)), this, SLOT(onReadData(std::string)));
}

authReg::~authReg()
{
    delete ui;
}

void authReg::onError(ClientError error)
{
    switch (error) {
    case ClientError::status_cannot_connect:
        cannotConnectError();
        break;
    case ClientError::status_disconnect:
        disconnectError();
        break;
    default:
        break;
    }
}

void authReg::onRead(const network::ResponseContext & response)
{
    emit readData(response.SerializeAsString());
}


void authReg::cannotConnectError()
{
    QMessageBox::critical(this, "Error", "Failure trying connect to network!");
    this->close();
}

void authReg::disconnectError()
{
    QMessageBox::critical(this, "Error", "You was disconnected! Restart application for connecting");
}

void authReg::userAuth(const network::AuthMessageResponse &responce, const network::SessionInfo &sessionInfo)
{
    ui->authL->setText(QString::fromStdString(responce.messagetext()));
    Roles role = static_cast<Roles>(responce.role());
    message_manager_->setSession(sessionInfo);
    switch (role) {
    case Roles::role_admin:{
        adminView_ = std::make_shared<AdminView>(message_manager_);
        connect(adminView_.get(), SIGNAL(onClose()), this, SLOT(logout()));
        adminView_->setAttribute(Qt::WA_DeleteOnClose, true);
        adminView_->show();
        this->hide();
        break;
    }
    case Roles::role_guest:{
        guestView_ = std::make_shared<GuestView>(message_manager_,responce.id_user());
        connect(guestView_.get(), SIGNAL(onClose()), this, SLOT(logout()));
        guestView_->setAttribute(Qt::WA_DeleteOnClose, true);
        guestView_->show();
        this->hide();
        break;
    }
    case Roles::role_manager:{
        managerView_ = std::make_shared<ManagerView>(message_manager_,responce.id_hotel(),responce.id_user());
        connect(managerView_.get(), SIGNAL(onClose()), this, SLOT(logout()));
        managerView_->setAttribute(Qt::WA_DeleteOnClose, true);
        managerView_->show();
        this->hide();
        break;
    }
    case Roles::role_receptionist:{
        receptionistView_ = std::make_shared<ReceptionistView>(message_manager_,responce.id_hotel(),responce.id_user());
        connect(receptionistView_.get(), SIGNAL(onClose()), this, SLOT(logout()));
        receptionistView_->setAttribute(Qt::WA_DeleteOnClose, true);
        receptionistView_->show();
        this->hide();
        break;
    }
    default:
        break;
    }
    LOG_INFO(responce.messagetext());
}


void authReg::on_testBtn_clicked()
{
    if(!ui->loginTE->toPlainText().isEmpty() && !ui->passLE->text().isEmpty())
    {
        std::string login = ui->loginTE->toPlainText().toStdString();
        std::string password = ui->passLE->text().toStdString();
        LOG_INFO(password);
        message_manager_->userAuth(login, password);
    }
}

void authReg::on_regBtn_clicked()
{
    registerView_ = std::make_shared<RegisterView>(message_manager_);
    connect(registerView_.get(), SIGNAL(onClose()), this, SLOT(logout()));
    connect(registerView_.get(), SIGNAL(enableGb()), this, SLOT(enableGb()));
    registerView_->show();
    ui->groupBox->setEnabled(false);
}

void authReg::enableGb()
{
    ui->groupBox->setEnabled(true);

    message_manager_->setOnReadCB(std::bind(&authReg::onRead, this, std::placeholders::_1));
}

void authReg::onReadData(std::string data)
{
    network::ResponseContext response;
    response.ParseFromString(data);
    switch (response.message_type_()) {
    case network::HN_AUTH: {
        network::AuthMessageResponse authRes = response.auth_response();
        userAuth(authRes,response.session_info());
        break;
    }
    }
}
