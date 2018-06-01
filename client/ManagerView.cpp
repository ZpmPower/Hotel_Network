#include "ManagerView.h"
#include "ui_ManagerView.h"
#include "QMessageBox"

ManagerView::ManagerView(std::shared_ptr<MessageManager> message_manager, uint32_t hoteid, QWidget *parent) :
    message_manager_(message_manager), hotelID_(hoteid),QWidget(parent),
    ui(new Ui::ManagerView)
{
    message_manager->setOnReadCB(std::bind(&ManagerView::onRead, this, std::placeholders::_1));
    ui->setupUi(this);
    ui->editBox->setVisible(false);
}

ManagerView::~ManagerView()
{
    delete ui;
}

void ManagerView::onRead(const network::ResponseContext &response)
{
    switch (response.message_type_()) {
    case network::HN_GET_HOTEL_EMPLOYEES: {
        network::EmployeesMessageResponse employeesRes = response.employees();
        getHotelEmployees(employeesRes);
        break;
    }
    case network::HN_REGISTER_EMPLOYEE: {
        network::RegisterMessageResponse regRes = response.register_response();
        receptionistRegister(regRes);
        break;
    }
    case network::HN_EDIT_EMPLOYEE: {
        network::RegisterMessageResponse regRes = response.register_response();
        isUpdated(regRes);
        break;
    }
    }
}

void ManagerView::on_hotelEmployeesBtn_clicked()
{
    message_manager_->getHotelEmployees(hotelID_);
}

void ManagerView::getHotelEmployees(const network::EmployeesMessageResponse &response)
{
    LOG_INFO(response.employees_size());
    ui->EmployeesTbl->clear();
    ui->EmployeesTbl->setRowCount(response.employees_size());
    ui->EmployeesTbl->setColumnCount(6);
    for(size_t i =0; i< response.employees_size(); i++)
    {
        network::EmployeeInfo info = response.employees(i);
        ui->EmployeesTbl->setHorizontalHeaderItem(0, new QTableWidgetItem("FirstName"));
        ui->EmployeesTbl->setHorizontalHeaderItem(1, new QTableWidgetItem("SecondName"));
        ui->EmployeesTbl->setHorizontalHeaderItem(2, new QTableWidgetItem("LastName"));
        ui->EmployeesTbl->setHorizontalHeaderItem(3, new QTableWidgetItem("PhoneNumber"));
        ui->EmployeesTbl->setHorizontalHeaderItem(4, new QTableWidgetItem("Salary"));
        ui->EmployeesTbl->setHorizontalHeaderItem(5, new QTableWidgetItem("Position"));

        TableItemEmployees* item1 = new TableItemEmployees(QString::fromStdString(info.firstname()));
        item1->info = info;
        ui->EmployeesTbl->setItem(i, 0, static_cast<QTableWidgetItem*>(item1));
        ui->EmployeesTbl->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(info.secondname())));
        ui->EmployeesTbl->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(info.lastname())));
        ui->EmployeesTbl->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(info.phonenumber())));
        ui->EmployeesTbl->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(std::to_string(info.salary()))));
        ui->EmployeesTbl->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(info.position())));
    }

    ui->EmployeesTbl->resizeColumnsToContents();
    ui->EmployeesTbl->resizeRowsToContents();
}

void ManagerView::receptionistRegister(const network::RegisterMessageResponse &response)
{
    ui->regLabel->setText(QString::fromStdString(response.messagetext()));
    LOG_INFO(response.messagetext());
}

uint32_t ManagerView::stringToRole(const std::string &role)
{
    if(role=="Manager") return 1;
    if(role=="Receptionist") return 2;
    if(role=="Admin") return 0;
    if(role=="Guest") return 3;
}

void ManagerView::isUpdated(const network::RegisterMessageResponse &responce)
{
    ui->label->setText(QString::fromStdString(responce.messagetext()));
    LOG_INFO(responce.messagetext());
}

void ManagerView::on_regReceptBtn_clicked()
{
    if(!ui->loginRPT->toPlainText().isEmpty() && !ui->passwordRPT->toPlainText().isEmpty())
    {
        std::string login = ui->loginRPT->toPlainText().toStdString();
        std::string password = ui->passwordRPT->toPlainText().toStdString();
        std::string fname = ui->firstNRPT->toPlainText().toStdString();
        std::string sname = ui->secondNRPT->toPlainText().toStdString();
        std::string lname = ui->lastNRPT->toPlainText().toStdString();
        std::string phone = ui->phoneRPT->toPlainText().toStdString();
        uint32_t position = 3;
        uint64_t salary = ui->salaryRPT->toPlainText().toInt(new bool,10);
        message_manager_->createEmployee(login, password, fname,sname,lname,phone,salary,position,hotelID_,static_cast<uint32_t>(Roles::role_receptionist));
    }
}

void ManagerView::on_pushButton_clicked()
{
    if(ui->comboBox->currentText().toStdString()=="Registration")
    {
        ui->regBox->setVisible(true);
        ui->editBox->setVisible(false);
    }
    else
    {
        ui->regBox->setVisible(false);
        ui->editBox->setVisible(true);
    }
}

void ManagerView::on_EmployeesTbl_itemClicked(QTableWidgetItem *item)
{
    int row  = item->row();
    TableItemEmployees *item1 = static_cast<TableItemEmployees *>(ui->EmployeesTbl->item(row, 0));
    //LOG_INFO(item1->info.name());
    currEmployee = item1->info;
    ui->label->setText(QString::fromStdString(std::to_string(currEmployee.id())));
    ui->firstNEdit->setPlainText(ui->EmployeesTbl->item(row, 0)->text());
    ui->secondNEdit->setPlainText(ui->EmployeesTbl->item(row, 1)->text());
    ui->lastNEdit->setPlainText(ui->EmployeesTbl->item(row, 2)->text());
    ui->phoneEdit->setPlainText(ui->EmployeesTbl->item(row, 3)->text());
    ui->salaryEdit->setPlainText(ui->EmployeesTbl->item(row, 4)->text());
}



void ManagerView::on_editBtn_clicked()
{
    std::string fname = ui->firstNEdit->toPlainText().toStdString();
    std::string sname = ui->secondNEdit->toPlainText().toStdString();
    std::string lname = ui->lastNEdit->toPlainText().toStdString();
    std::string phone = ui->phoneEdit->toPlainText().toStdString();
    uint64_t salary = ui->salaryEdit->toPlainText().toInt(new bool,10);
    message_manager_->editEmployee(currEmployee.id(),fname,sname,lname,phone,salary,currEmployee.position(),currEmployee.hotelid());
}


void ManagerView::on_deleteBtn_clicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("title");
    msgBox.setText("Question");
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if(msgBox.exec() == QMessageBox::Yes)
    {

    }
    else
    {
      // do something else
    }
}
