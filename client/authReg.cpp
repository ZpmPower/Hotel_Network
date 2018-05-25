#include "authReg.h"
#include "ui_authReg.h"
#include "GlobalParams.h"
#include "QMessageBox"
#include "Helper.h"
#include <QTimer>

authReg::authReg(std::shared_ptr<MessageManager> message_manager, QWidget *parent) :
    message_manager_(message_manager),
    QWidget(parent),
    ui(new Ui::authReg)
{
    ui->setupUi(this);
    if(message_manager)
    {
        message_manager_->setOnErrorCB(std::bind(&authReg::onError, this, std::placeholders::_1));
        message_manager->setOnReadCB(std::bind(&authReg::onRead, this, std::placeholders::_1));
    }

    timer_ = new QTimer();
    timer_->setInterval(1000);
    connect(timer_, SIGNAL(timeout()), this, SLOT(updateTime()));
    timer_->start();
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
    switch (response.message_type_()) {
    case network::HN_REGISTER: {
        network::RegisterMessageResponse regRes = response.register_response();
        userRegister(regRes);
        break;
    }
    case network::HN_AUTH: {
        network::AuthMessageResponse authRes = response.auth_response();
        userAuth(authRes);
        break;
    }
    }
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

void authReg::userRegister(const network::RegisterMessageResponse &response)
{
    ui->registerLabel->setText(QString::fromStdString(response.messagetext()));
    LOG_INFO(response.messagetext());
}

void authReg::userAuth(const network::AuthMessageResponse &responce)
{
    ui->authL->setText(QString::fromStdString(responce.messagetext()));
    LOG_INFO(responce.messagetext());
}


void authReg::updateTime()
{/*
    if(!message_manager_)
    {
        cannotConnectError();
    }*/
}

void authReg::on_testBtn_2_clicked()
{
    if(!ui->loginUPTE->toPlainText().isEmpty() && !ui->passwordUPTE->toPlainText().isEmpty())
    {
        std::string login = ui->loginUPTE->toPlainText().toStdString();
        std::string password = ui->passwordUPTE->toPlainText().toStdString();
        std::string fname = ui->firstN->toPlainText().toStdString();
        std::string sname = ui->secondN->toPlainText().toStdString();
        std::string lname = ui->lastN->toPlainText().toStdString();
        std::string phone = ui->phoneTE->toPlainText().toStdString();
        std::string passport = ui->passportTE->toPlainText().toStdString();
        message_manager_->createUser(login, password, fname,sname,lname,phone,passport);
    }
}

void authReg::on_testBtn_clicked()
{
    if(!ui->loginTE->toPlainText().isEmpty() && !ui->passwordTE->toPlainText().isEmpty())
    {
        std::string login = ui->loginTE->toPlainText().toStdString();
        std::string password = ui->passwordTE->toPlainText().toStdString();
        message_manager_->userAuth(login, password);
    }
}
