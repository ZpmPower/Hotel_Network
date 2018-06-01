#include "RegisterView.h"
#include "ui_RegisterView.h"
#include "GlobalParams.h"
#include "QMessageBox"
#include "Helper.h"
#include "authReg.h"

RegisterView::RegisterView(std::shared_ptr<MessageManager> message_manager, QWidget *parent):
    message_manager_(message_manager),QWidget(parent), ui(new Ui::RegisterView)
{
    ui->setupUi(this);

    if(message_manager)
    {
        //message_manager_->setOnErrorCB(std::bind(&RegisterView::onError, this, std::placeholders::_1));
        message_manager->setOnReadCB(std::bind(&RegisterView::onRead, this, std::placeholders::_1));
    }

}

RegisterView::~RegisterView()
{
    delete ui;
}

void RegisterView::userRegister(const network::RegisterMessageResponse &response)
{
    ui->registerLabel->setText(QString::fromStdString(response.messagetext()));
    LOG_INFO(response.messagetext());
}

void RegisterView::onRead(const network::ResponseContext &response)
{
    switch (response.message_type_()) {
    case network::HN_REGISTER: {
        network::RegisterMessageResponse regRes = response.register_response();
        userRegister(regRes);
        break;
    }
    }
}

void RegisterView::closeEvent(QCloseEvent *bar)
{
    emit enableGb();

}

void RegisterView::on_testBtn_2_clicked()
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
        message_manager_->createGuest(login, password, fname,sname,lname,phone,passport,static_cast<uint32_t>(Roles::role_guest));
    }
}
