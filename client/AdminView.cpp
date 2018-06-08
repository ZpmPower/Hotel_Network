#include "AdminView.h"
#include "ui_AdminView.h"
#include <QMessageBox>

AdminView::AdminView(std::shared_ptr<MessageManager> message_manager, QWidget *parent) :
    message_manager_(message_manager),QWidget(parent),
    ui(new Ui::AdminView)
{
    ui->setupUi(this);
    if(message_manager)
    {
        //message_manager_->setOnErrorCB(std::bind(&authReg::onError, this, std::placeholders::_1));
        message_manager_->setOnReadCB(std::bind(&AdminView::onRead, this, std::placeholders::_1));
    }
    connect(ui->okBtn, SIGNAL(clicked()), this, SLOT(onClick()));

}

AdminView::~AdminView()
{
    delete ui;
}

void AdminView::onRead(const network::ResponseContext &response)
{
    switch (response.message_type_()) {
//        case network::HN_GET_ALL_GUESTS: {
//            network::GuestsMessageResponse guestsRes = response.guests();
//            getGuests(guestsRes);
//            break;
//        }
    case network::HN_REGISTER: {
        network::RegisterMessageResponse regRes = response.register_response();
        guestRegister(regRes);
        break;
        }
    case network::HN_REGISTER_EMPLOYEE: {
        network::RegisterMessageResponse regRes = response.register_response();
        guestRegister(regRes);
        break;
        }
//    case network::HN_GET_ALL_EMPLOYEES: {
//        network::EmployeesMessageResponse employeesRes = response.employees();
//        getEmployees(employeesRes);
//        break;
//        }
//    case network::HN_GET_ALL_HOTELS: {
//        network::HotelsMessageResponse hotelRes = response.hotels();
//        getHotels(hotelRes);
//        break;
//        }
//    case network::HN_GET_ALL_ROOMS: {
//        network::RoomsMessageResponse roomRes = response.rooms();
//        getRooms(roomRes);
//        break;
//        }
    }
}


void AdminView::on_okBtn_clicked()
{
    if(ui->comboBox->currentText().toStdString()=="Guest")
    {
        ui->groupBox->setVisible(true);
        ui->groupBox_2->setVisible(false);
        ui->groupBox_3->setVisible(false);
    }
    if(ui->comboBox->currentText().toStdString()=="Receptionist")
    {
        ui->groupBox_2->setVisible(true);
        ui->groupBox->setVisible(false);
        ui->groupBox_3->setVisible(false);
    }
    if(ui->comboBox->currentText().toStdString()=="Manager")
    {
        ui->groupBox_3->setVisible(true);
        ui->groupBox_2->setVisible(false);
        ui->groupBox->setVisible(false);
    }

}

void AdminView::on_tabWidget_currentChanged(int index)
{

}


void AdminView::on_allGuestsBtn_clicked()
{
    message_manager_->getGuests();
}

//void AdminView::getGuests(const network::GuestsMessageResponse &response)
//{
//    ui->GuestTbl->clear();
//    ui->GuestTbl->setRowCount(response.guests_size());
//    ui->GuestTbl->setColumnCount(5);
//    for(size_t i =0; i< response.guests_size(); i++)
//    {
//        network::GuestInfo info = response.guests(i);
//        ui->EmployeesTbl->setHorizontalHeaderItem(0, new QTableWidgetItem("FirstName"));
//        ui->GuestTbl->setHorizontalHeaderItem(1, new QTableWidgetItem("Secondname"));
//        ui->GuestTbl->setHorizontalHeaderItem(2, new QTableWidgetItem("LastName"));
//        ui->GuestTbl->setHorizontalHeaderItem(3, new QTableWidgetItem("PhoneNumber"));
//        ui->GuestTbl->setHorizontalHeaderItem(4, new QTableWidgetItem("Passport"));

//        ui->GuestTbl->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(info.firstname())));
//        ui->GuestTbl->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(info.secondname())));
//        ui->GuestTbl->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(info.lastname())));
//        ui->GuestTbl->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(info.phonenumber())));
//        ui->GuestTbl->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(info.passport())));
//    }

//    ui->GuestTbl->resizeColumnsToContents();
//    ui->GuestTbl->resizeRowsToContents();
//}

//void AdminView::getEmployees(const network::EmployeesMessageResponse &response)
//{
//    ui->EmployeesTbl->clear();
//    ui->EmployeesTbl->setRowCount(response.employees_size());
//    ui->EmployeesTbl->setColumnCount(7);
//    for(size_t i =0; i< response.employees_size(); i++)
//    {
//        network::EmployeeInfo info = response.employees(i);
//        ui->EmployeesTbl->setHorizontalHeaderItem(0, new QTableWidgetItem("FirstName"));
//        ui->EmployeesTbl->setHorizontalHeaderItem(1, new QTableWidgetItem("SecondName"));
//        ui->EmployeesTbl->setHorizontalHeaderItem(2, new QTableWidgetItem("LastName"));
//        ui->EmployeesTbl->setHorizontalHeaderItem(3, new QTableWidgetItem("PhoneNumber"));
//        ui->EmployeesTbl->setHorizontalHeaderItem(4, new QTableWidgetItem("Salary"));
//        ui->EmployeesTbl->setHorizontalHeaderItem(5, new QTableWidgetItem("Position"));
//        ui->EmployeesTbl->setHorizontalHeaderItem(6, new QTableWidgetItem("Hotel_id"));

//        ui->EmployeesTbl->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(info.firstname())));
//        ui->EmployeesTbl->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(info.secondname())));
//        ui->EmployeesTbl->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(info.lastname())));
//        ui->EmployeesTbl->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(info.phonenumber())));
//        ui->EmployeesTbl->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(std::to_string(info.salary()))));
//        ui->EmployeesTbl->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(info.position())));
//        ui->EmployeesTbl->setItem(i, 6, new QTableWidgetItem(QString::fromStdString(std::to_string(info.hotelid()))));
//    }

//    ui->EmployeesTbl->resizeColumnsToContents();
//    ui->EmployeesTbl->resizeRowsToContents();
//}

//void AdminView::getHotels(const network::HotelsMessageResponse &response)
//{

//    ui->HotelsTbl->clear();
//    ui->HotelsTbl->setRowCount(response.hotels_size());
//    ui->HotelsTbl->setColumnCount(7);
//    for(size_t i =0; i< response.hotels_size(); i++)
//    {
//        network::HotelInfo info = response.hotels(i);
//        ui->HotelsTbl->setHorizontalHeaderItem(0, new QTableWidgetItem("Name"));
//        ui->HotelsTbl->setHorizontalHeaderItem(1, new QTableWidgetItem("City"));
//        ui->HotelsTbl->setHorizontalHeaderItem(2, new QTableWidgetItem("Street"));
//        ui->HotelsTbl->setHorizontalHeaderItem(3, new QTableWidgetItem("PhoneNumber"));
//        ui->HotelsTbl->setHorizontalHeaderItem(4, new QTableWidgetItem("E-Mail"));
//        ui->HotelsTbl->setHorizontalHeaderItem(5, new QTableWidgetItem("Type"));
//        ui->HotelsTbl->setHorizontalHeaderItem(6, new QTableWidgetItem("Stars"));

//        TableItemHotels* item1 = new TableItemHotels(QString::fromStdString(info.name()));
//        item1->info = info;
//        ui->HotelsTbl->setItem(i, 0, static_cast<QTableWidgetItem*>(item1));
//        ui->HotelsTbl->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(info.city())));
//        ui->HotelsTbl->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(info.street())));
//        ui->HotelsTbl->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(info.phonenumber())));
//        ui->HotelsTbl->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(info.email())));
//        ui->HotelsTbl->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(info.type())));
//        ui->HotelsTbl->setItem(i, 6, new QTableWidgetItem(QString::fromStdString(std::to_string(info.stars()))));


//    }

//    ui->HotelsTbl->resizeColumnsToContents();
//    ui->HotelsTbl->resizeRowsToContents();
//}

//void AdminView::getRooms(const network::RoomsMessageResponse &response)
//{
//    ui->RoomsTbl->clear();
//    ui->RoomsTbl->setRowCount(response.rooms_size());
//    ui->RoomsTbl->setColumnCount(7);
//    for(size_t i =0; i< response.rooms_size(); i++)
//    {
//        network::RoomInfo info = response.rooms(i);
//        ui->RoomsTbl->setHorizontalHeaderItem(0, new QTableWidgetItem("Places"));
//        ui->RoomsTbl->setHorizontalHeaderItem(1, new QTableWidgetItem("Price"));
//        ui->RoomsTbl->setHorizontalHeaderItem(2, new QTableWidgetItem("Rating"));
//        ui->RoomsTbl->setHorizontalHeaderItem(3, new QTableWidgetItem("Status"));
//        ui->RoomsTbl->setHorizontalHeaderItem(4, new QTableWidgetItem("Floor"));
//        ui->RoomsTbl->setHorizontalHeaderItem(5, new QTableWidgetItem("Type"));
//        ui->RoomsTbl->setHorizontalHeaderItem(6, new QTableWidgetItem("Hotel_id"));

//        ui->RoomsTbl->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(std::to_string(info.places()))));
//        ui->RoomsTbl->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(std::to_string(info.price()))));
//        ui->RoomsTbl->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(std::to_string(info.rating()))));
//        ui->RoomsTbl->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(std::to_string(info.status()))));
//        ui->RoomsTbl->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(std::to_string(info.floor()))));
//        ui->RoomsTbl->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(info.type())));
//        ui->RoomsTbl->setItem(i, 6, new QTableWidgetItem(QString::fromStdString(std::to_string(info.hotelid()))));
//    }

//    ui->RoomsTbl->resizeColumnsToContents();
//    ui->RoomsTbl->resizeRowsToContents();
//}

void AdminView::guestRegister(const network::RegisterMessageResponse &response)
{
    ui->regLabel->setText(QString::fromStdString(response.messagetext()));
    LOG_INFO(response.messagetext());
}

void AdminView::on_tabWidget_tabBarClicked(int index)
{
    ui->groupBox->setVisible(false);
    ui->groupBox_2->setVisible(false);
    ui->groupBox_3->setVisible(false);
}

void AdminView::on_regGuestBnt_clicked()
{
    if(!ui->loginGPT->toPlainText().isEmpty() && !ui->passwordGPT->toPlainText().isEmpty())
    {
        std::string login = ui->loginGPT->toPlainText().toStdString();
        std::string password = ui->passwordGPT->toPlainText().toStdString();
        std::string fname = ui->firstNGPT->toPlainText().toStdString();
        std::string sname = ui->secondNGPT->toPlainText().toStdString();
        std::string lname = ui->lastNGPT->toPlainText().toStdString();
        std::string phone = ui->phoneGPT->toPlainText().toStdString();
        std::string passport = ui->passportGPT->toPlainText().toStdString();
        message_manager_->createGuest(login, password, fname,sname,lname,phone,passport,static_cast<uint32_t>(Roles::role_guest));
    }
}

void AdminView::on_regReceptBtn_clicked()
{
    if(!ui->loginRPT->toPlainText().isEmpty() && !ui->passwordRPT->toPlainText().isEmpty())
    {
        std::string login = ui->loginRPT->toPlainText().toStdString();
        std::string password = ui->passwordRPT->toPlainText().toStdString();
        std::string fname = ui->firstNRPT->toPlainText().toStdString();
        std::string sname = ui->secondNRPT->toPlainText().toStdString();
        std::string lname = ui->lastNRPT->toPlainText().toStdString();
        std::string phone = ui->phoneRPT->toPlainText().toStdString();
        uint32_t position = static_cast<uint32_t>(Roles::role_manager);
        uint64_t salary = ui->salaryRPT->toPlainText().toInt(new bool,10);
        uint32_t hotelID = ui->hotelIDRPT->toPlainText().toInt(new bool,10);
        message_manager_->createEmployee(login, password, fname,sname,lname,phone,salary,position,hotelID,static_cast<uint32_t>(Roles::role_receptionist));
    }
}

void AdminView::on_regManagerBtn_clicked()
{
    if(!ui->loginMPT->toPlainText().isEmpty() && !ui->passwordMPT->toPlainText().isEmpty())
    {
        std::string login = ui->loginMPT->toPlainText().toStdString();
        std::string password = ui->passwordMPT->toPlainText().toStdString();
        std::string fname = ui->firstNMPT->toPlainText().toStdString();
        std::string sname = ui->secondNMPT->toPlainText().toStdString();
        std::string lname = ui->lastNMPT->toPlainText().toStdString();
        std::string phone = ui->phoneMPT->toPlainText().toStdString();
        uint32_t position = static_cast<uint32_t>(Roles::role_receptionist);
        uint64_t salary = ui->salaryMPT->toPlainText().toInt(new bool,10);
        uint32_t hotelID = ui->hotelIDMPT->toPlainText().toInt(new bool,10);
        message_manager_->createEmployee(login, password, fname,sname,lname,phone,salary,position,hotelID,static_cast<uint32_t>(Roles::role_manager));
    }
}

void AdminView::on_allEmployeeBtn_clicked()
{
    message_manager_->getEmployees();
}

void AdminView::on_allHotelsBtn_clicked()
{
    message_manager_->getHotels();
}

void AdminView::on_allRoomsBtn_clicked()
{
    message_manager_->getRooms();
}

//void AdminView::on_HotelsTbl_itemClicked(QTableWidgetItem *item)
//{
//    int row  = item->row();
//    TableItemHotels *item1 = static_cast<TableItemHotels *>(ui->HotelsTbl->item(row, 0));
//    LOG_INFO(item1->info.name());
//    currHotel = item1->info;
//    ui->HotelsTbl->item(row, 1);
//    ui->HotelsTbl->item(row, 2);
//    ui->HotelsTbl->item(row, 3);
//    ui->HotelsTbl->item(row, 4);
//    ui->HotelsTbl->item(row, 5);
//    ui->HotelsTbl->item(row, 6);
//}
