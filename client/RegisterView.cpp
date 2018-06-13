#include "RegisterView.h"
#include "ui_RegisterView.h"
#include "GlobalParams.h"
#include "QMessageBox"
#include "Helper.h"
#include "authReg.h"
#include <regex>

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
    if(!ui->loginUPTE->toPlainText().isEmpty() && !ui->passwordUPTE->toPlainText().isEmpty() &&
            !ui->firstN->toPlainText().isEmpty() && !ui->secondN->toPlainText().isEmpty() && !ui->passportTE->toPlainText().isEmpty())
    {
        std::string login = ui->loginUPTE->toPlainText().toStdString();
        std::string password = ui->passwordUPTE->toPlainText().toStdString();
        std::string fname = ui->firstN->toPlainText().toStdString();
        std::string sname = ui->secondN->toPlainText().toStdString();
        std::string lname = ui->lastN->toPlainText().toStdString();
        std::string phone = ui->phoneTE->toPlainText().toStdString();
        std::string passport = ui->passportTE->toPlainText().toStdString();
        std::regex regexLogin("^[a-zA-Z][a-zA-Z0-9-_\\.]{1,20}$");
        if(std::regex_match(login,regexLogin))
        {
            if(std::regex_match(passport,std::regex("^[A-Z]{2}\\d[0-9]{5}$")))
            {
                if(std::regex_match(fname, std::regex("^[a-zA-Z]+$")) && std::regex_match(sname, std::regex("^[a-zA-Z]+$")))
                {
                    if(!lname.empty() || !phone.empty())
                    {
                        std::regex regexPhone("^(\\s*)?(\\+)?([- _():=+]?\\d[- _():=+]?){10,14}(\\s*)?$");
                        std::regex regexLastN("^[a-zA-Z]+$");
                        if(!phone.empty() && lname.empty())
                        {
                            if(std::regex_match(phone,regexPhone))
                            {
                                message_manager_->createGuest(login, password, fname,sname,lname,phone,passport,static_cast<uint32_t>(Roles::role_manager));
                            }
                            else
                            {
                                ui->registerLabel->setText(QString::fromStdString("Enter corrent Phone_Number"));
                            }
                        }
                        if(phone.empty() && !lname.empty())
                        {
                            if(std::regex_match(lname,regexLastN))
                            {

                                message_manager_->createGuest(login, password, fname,sname,lname,phone,passport,static_cast<uint32_t>(Roles::role_guest));
                            }
                            else
                            {
                                ui->registerLabel->setText(QString::fromStdString("Enter corrent Last_Name"));
                            }
                        }
                        if(!phone.empty() && !lname.empty())
                        {
                            if(std::regex_match(phone,regexPhone) && std::regex_match(lname,regexLastN))
                            {
                                message_manager_->createGuest(login, password, fname,sname,lname,phone,passport,static_cast<uint32_t>(Roles::role_guest));
                            }
                            else
                            {
                                 ui->registerLabel->setText(QString::fromStdString("Enter corrent Last_Name\nand phone"));
                            }
                        }
                    }
                    else
                    {
                        message_manager_->createGuest(login, password, fname,sname,lname,phone,passport,static_cast<uint32_t>(Roles::role_guest));
                    }
                }
                else
                {
                    ui->registerLabel->setText(QString::fromStdString("Check First_Name and Second_Name"));
                }
            }
            else
            {
                ui->registerLabel->setText(QString::fromStdString("Passport must have 2 letter\nand 6 numbers"));
            }
        }
        else
        {
            ui->registerLabel->setText(QString::fromStdString("Login must be 2-20 chars\nStart from letter"));
        }
    }
    else
    {
        ui->registerLabel->setText(QString::fromStdString("Enter all required fields"));
    }
}
