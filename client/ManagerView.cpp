#include "ManagerView.h"
#include "ui_ManagerView.h"
#include "QMessageBox"

ManagerView::ManagerView(std::shared_ptr<MessageManager> message_manager, uint32_t hoteid,uint32_t employee_id, QWidget *parent) :
    message_manager_(message_manager), hotelID_(hoteid),manager_id(employee_id),QWidget(parent),
    ui(new Ui::ManagerView)
{
    message_manager->setOnReadCB(std::bind(&ManagerView::onRead, this, std::placeholders::_1));
    ui->setupUi(this);
    ui->editBox->setVisible(false);
    ui->editRoomBox->setVisible(false);
    message_manager->getRoomTypes();
    QDate date = QDate::currentDate();
    ui->dateBegin->setDate(date);
    ui->dateBegin->setMinimumDate(date);
    ui->dateEnd->setMinimumDate(date);
    message_manager_->getHotelEmployees(hotelID_);
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
    case network::HN_DELETE_EMPLOYEE: {
        network::RegisterMessageResponse regRes = response.register_response();
        isDeleteEmployee(regRes);
        break;
    }
    case network::HN_DELETE_ROOM: {
        network::RegisterMessageResponse regRes = response.register_response();
        isDeleteRoom(regRes);
        break;
    }
    case network::HN_GET_HOTEL_ROOMS: {
        network::RoomsMessageResponse roomsRes = response.rooms();
        getHotelRooms(roomsRes);
        break;
    }
    case network::HN_GET_ROOM_TYPES: {
        network::RoomTypesMessageResponse typesRes = response.types();
        setRoomTypes(typesRes);
        setRoomTypesGen(typesRes);
        //setRoomTypesOrders(typesRes);
        break;
    }
    case network::HN_GET_HOTEL_ORDERS: {
        network::OrdersMessageResponse ordersRes = response.orders();
        getHotelOrders(ordersRes);
        break;
    }
    case network::HN_GET_ALL_GUESTS: {
        network::GuestsMessageResponse guestsRes = response.guests();
        getGuests(guestsRes);
        break;
    }
    case network::HN_GET_ALL_HOTELS: {
        network::HotelsMessageResponse hotelsRes = response.hotels();
        getHotels(hotelsRes);
        break;
    }
    case network::HN_GET_HOTEL_TYPES: {
        network::HotelTypesMessageResponse typesRes = response.hotel_types();
        getHotelTypes(typesRes);
        break;
    }
    case network::HN_GET_VACANT_ROOMS: {
        network::RoomsMessageResponse roomsRes = response.rooms();
        getVacantRooms(roomsRes);
        break;
    }
    case network::HN_GET_CURRENT_GUESTS: {
        network::GuestOrdersMessageResponse guestOrderRes = response.guest_orders();
        getGuestOrders(guestOrderRes);
        break;
    }
    case network::HN_COUNT_HOTEL_ROOMS: {
        network::RegisterMessageResponse res = response.register_response();
        setCountRooms(res);
        break;
    }
    case network::HN_COUNT_HOTEL_EMPLOYEES: {
        network::RegisterMessageResponse res = response.register_response();
        setCountEmployees(res);
        break;
    }
    case network::HN_COUNT_HOTEL_ORDERS: {
        network::RegisterMessageResponse res = response.register_response();
        setCountOrders(res);
        break;
    }
    case network::HN_AVG_RESIDENCE_TIME: {
        network::RegisterMessageResponse res = response.register_response();
        setAvgResTime(res);
        break;
    }
    case network::HN_AVG_ROOM_RATING: {
        network::RegisterMessageResponse res = response.register_response();
        setAvgRoomRating(res);
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
    ui->EmployeesTbl->setColumnCount(7);
    ui->EmployeesTbl->setHorizontalHeaderItem(0, new QTableWidgetItem("FirstName"));
    ui->EmployeesTbl->setHorizontalHeaderItem(1, new QTableWidgetItem("SecondName"));
    ui->EmployeesTbl->setHorizontalHeaderItem(2, new QTableWidgetItem("LastName"));
    ui->EmployeesTbl->setHorizontalHeaderItem(3, new QTableWidgetItem("PhoneNumber"));
    ui->EmployeesTbl->setHorizontalHeaderItem(4, new QTableWidgetItem("Salary"));
    ui->EmployeesTbl->setHorizontalHeaderItem(5, new QTableWidgetItem("Position"));
    ui->EmployeesTbl->setHorizontalHeaderItem(6, new QTableWidgetItem("ID"));
    for(size_t i =0; i< response.employees_size(); i++)
    {
        network::EmployeeInfo info = response.employees(i);

        TableItemEmployees* item1 = new TableItemEmployees(QString::fromStdString(info.firstname()));
        item1->info = info;
        ui->EmployeesTbl->setItem(i, 0, static_cast<QTableWidgetItem*>(item1));
        ui->EmployeesTbl->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(info.secondname())));
        ui->EmployeesTbl->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(info.lastname())));
        ui->EmployeesTbl->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(info.phonenumber())));
        ui->EmployeesTbl->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(std::to_string(info.salary()))));
        ui->EmployeesTbl->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(info.position())));
        ui->EmployeesTbl->setItem(i, 6, new QTableWidgetItem(QString::fromStdString(std::to_string(info.id()))));
    }

    ui->EmployeesTbl->resizeColumnsToContents();
    ui->EmployeesTbl->resizeRowsToContents();
}

void ManagerView::getHotelRooms(const network::RoomsMessageResponse &response)
{
    ui->RoomsTbl->clear();
    ui->RoomsTbl->setRowCount(response.rooms_size());
    ui->RoomsTbl->setColumnCount(7);
    ui->RoomsTbl->setHorizontalHeaderItem(0, new QTableWidgetItem("Places"));
    ui->RoomsTbl->setHorizontalHeaderItem(1, new QTableWidgetItem("Price"));
    ui->RoomsTbl->setHorizontalHeaderItem(2, new QTableWidgetItem("Rating"));
    ui->RoomsTbl->setHorizontalHeaderItem(3, new QTableWidgetItem("Status"));
    ui->RoomsTbl->setHorizontalHeaderItem(4, new QTableWidgetItem("Floor"));
    ui->RoomsTbl->setHorizontalHeaderItem(5, new QTableWidgetItem("Type"));
    ui->RoomsTbl->setHorizontalHeaderItem(5, new QTableWidgetItem("ID"));
    for(size_t i =0; i< response.rooms_size(); i++)
    {
        network::RoomInfo info = response.rooms(i);

        TableItemRooms* item1 = new TableItemRooms(QString::fromStdString(std::to_string(info.places())));
        item1->info = info;
        ui->RoomsTbl->setItem(i, 0, static_cast<QTableWidgetItem*>(item1));
        ui->RoomsTbl->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(std::to_string(info.price()))));
        ui->RoomsTbl->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(std::to_string(info.rating()))));
        ui->RoomsTbl->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(statusToString(info.status()))));
        ui->RoomsTbl->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(std::to_string(info.floor()))));
        ui->RoomsTbl->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(std::to_string(info.id()))));
    }

    ui->RoomsTbl->resizeColumnsToContents();
    ui->RoomsTbl->resizeRowsToContents();
}

void ManagerView::getVacantRooms(const network::RoomsMessageResponse &response)
{
    ui->vacantRoomsTbl->clear();
    ui->vacantRoomsTbl->setRowCount(response.rooms_size());
    ui->vacantRoomsTbl->setColumnCount(7);
    ui->vacantRoomsTbl->setHorizontalHeaderItem(0, new QTableWidgetItem("Places"));
    ui->vacantRoomsTbl->setHorizontalHeaderItem(1, new QTableWidgetItem("Price"));
    ui->vacantRoomsTbl->setHorizontalHeaderItem(2, new QTableWidgetItem("Rating"));
    ui->vacantRoomsTbl->setHorizontalHeaderItem(3, new QTableWidgetItem("Status"));
    ui->vacantRoomsTbl->setHorizontalHeaderItem(4, new QTableWidgetItem("Floor"));
    ui->vacantRoomsTbl->setHorizontalHeaderItem(5, new QTableWidgetItem("Type"));
    ui->vacantRoomsTbl->setHorizontalHeaderItem(5, new QTableWidgetItem("ID"));
    for(size_t i =0; i< response.rooms_size(); i++)
    {
        network::RoomInfo info = response.rooms(i);

        TableItemRooms* item1 = new TableItemRooms(QString::fromStdString(std::to_string(info.places())));
        item1->info = info;
        ui->vacantRoomsTbl->setItem(i, 0, static_cast<QTableWidgetItem*>(item1));
        ui->vacantRoomsTbl->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(std::to_string(info.price()))));
        ui->vacantRoomsTbl->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(std::to_string(info.rating()))));
        ui->vacantRoomsTbl->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(statusToString(info.status()))));
        ui->vacantRoomsTbl->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(std::to_string(info.floor()))));
        ui->vacantRoomsTbl->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(std::to_string(info.id()))));
    }

    ui->vacantRoomsTbl->resizeColumnsToContents();
    ui->vacantRoomsTbl->resizeRowsToContents();
}

void ManagerView::getGuests(const network::GuestsMessageResponse &response)
{
    ui->GuestsTbl->clear();
    ui->GuestsTbl->setRowCount(response.guests_size());
    ui->GuestsTbl->setColumnCount(5);

    ui->GuestsTbl->setHorizontalHeaderItem(0, new QTableWidgetItem("FirstName"));
    ui->GuestsTbl->setHorizontalHeaderItem(1, new QTableWidgetItem("Secondname"));
    ui->GuestsTbl->setHorizontalHeaderItem(2, new QTableWidgetItem("LastName"));
    ui->GuestsTbl->setHorizontalHeaderItem(3, new QTableWidgetItem("PhoneNumber"));
    ui->GuestsTbl->setHorizontalHeaderItem(4, new QTableWidgetItem("Passport"));
    for(size_t i =0; i< response.guests_size(); i++)
    {
        network::GuestInfo info = response.guests(i);
        TableItemGuest* item1 = new TableItemGuest(QString::fromStdString(info.firstname()));
        item1->info = info;

        ui->GuestsTbl->setItem(i, 0, static_cast<QTableWidgetItem*>(item1));
        ui->GuestsTbl->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(info.secondname())));
        ui->GuestsTbl->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(info.lastname())));
        ui->GuestsTbl->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(info.phonenumber())));
        ui->GuestsTbl->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(info.passport())));
    }

    ui->GuestsTbl->resizeColumnsToContents();
    ui->GuestsTbl->resizeRowsToContents();
}

void ManagerView::getHotels(const network::HotelsMessageResponse &response)
{
    ui->HotelsTbl->clear();
    ui->HotelsTbl->setRowCount(response.hotels_size());
    ui->HotelsTbl->setColumnCount(6);

    ui->HotelsTbl->setHorizontalHeaderItem(0, new QTableWidgetItem("Name"));
    ui->HotelsTbl->setHorizontalHeaderItem(1, new QTableWidgetItem("City"));
    ui->HotelsTbl->setHorizontalHeaderItem(2, new QTableWidgetItem("Street"));
    ui->HotelsTbl->setHorizontalHeaderItem(3, new QTableWidgetItem("PhoneNumber"));
    ui->HotelsTbl->setHorizontalHeaderItem(4, new QTableWidgetItem("Email"));
    ui->HotelsTbl->setHorizontalHeaderItem(5, new QTableWidgetItem("Type"));
    for(size_t i =0; i< response.hotels_size(); i++)
    {
        network::HotelInfo info = response.hotels(i);


        ui->HotelsTbl->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(info.name())));
        ui->HotelsTbl->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(info.city())));
        ui->HotelsTbl->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(info.street())));
        ui->HotelsTbl->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(info.phonenumber())));
        ui->HotelsTbl->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(info.email())));
        ui->HotelsTbl->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(info.type())));
    }

    ui->HotelsTbl->resizeColumnsToContents();
    ui->HotelsTbl->resizeRowsToContents();
}

void ManagerView::getHotelTypes(const network::HotelTypesMessageResponse &response)
{
    ui->HotelTypesTbl->clear();
    ui->HotelTypesTbl->setRowCount(response.types_size());
    ui->HotelTypesTbl->setColumnCount(1);
    for(size_t i =0; i< response.types_size(); i++)
    {
        network::HotelType info = response.types(i);
        ui->HotelTypesTbl->setHorizontalHeaderItem(0, new QTableWidgetItem("Type"));

        ui->HotelTypesTbl->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(info.name())));
    }

    ui->HotelTypesTbl->resizeColumnsToContents();
    ui->HotelTypesTbl->resizeRowsToContents();
}

void ManagerView::getHotelOrders(const network::OrdersMessageResponse &response)
{
    ui->OrdersTbl->clear();
    ui->OrdersTbl->setRowCount(response.orders_size());
    ui->OrdersTbl->setColumnCount(6);
    ui->OrdersTbl->setHorizontalHeaderItem(0, new QTableWidgetItem("StartDate"));
    ui->OrdersTbl->setHorizontalHeaderItem(1, new QTableWidgetItem("EndDate"));
    ui->OrdersTbl->setHorizontalHeaderItem(2, new QTableWidgetItem("IdRoom"));
    ui->OrdersTbl->setHorizontalHeaderItem(3, new QTableWidgetItem("Employee"));
    ui->OrdersTbl->setHorizontalHeaderItem(4, new QTableWidgetItem("Guest"));
    ui->OrdersTbl->setHorizontalHeaderItem(5, new QTableWidgetItem("ID"));
    for(size_t i =0; i< response.orders_size(); i++)
    {
        network::OrderInfo info = response.orders(i);


        TableItemOrders* item1 = new TableItemOrders(QString::fromStdString(info.startdate()));
        item1->info = info;
        ui->OrdersTbl->setItem(i, 0, static_cast<QTableWidgetItem*>(item1));
        ui->OrdersTbl->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(info.enddate())));
        ui->OrdersTbl->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(std::to_string(info.idroom()))));
        ui->OrdersTbl->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(info.employee_secondname())));
        ui->OrdersTbl->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(info.guest_secondname())));
        ui->OrdersTbl->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(std::to_string(info.id()))));
    }

    ui->OrdersTbl->resizeColumnsToContents();
    ui->OrdersTbl->resizeRowsToContents();
}

void ManagerView::getGuestOrders(const network::GuestOrdersMessageResponse &response)
{
    ui->currGuestsTbl->clear();
    ui->currGuestsTbl->setRowCount(response.orders_size());
    ui->currGuestsTbl->setColumnCount(6);
    ui->currGuestsTbl->setHorizontalHeaderItem(0, new QTableWidgetItem("FirstName"));
    ui->currGuestsTbl->setHorizontalHeaderItem(1, new QTableWidgetItem("SecondName"));
    ui->currGuestsTbl->setHorizontalHeaderItem(2, new QTableWidgetItem("LastName"));
    ui->currGuestsTbl->setHorizontalHeaderItem(3, new QTableWidgetItem("StartDate"));
    ui->currGuestsTbl->setHorizontalHeaderItem(4, new QTableWidgetItem("EndDate"));
    ui->currGuestsTbl->setHorizontalHeaderItem(5, new QTableWidgetItem("IDRoom"));
    for(size_t i =0; i< response.orders_size(); i++)
    {
        network::GuestOrderInfo info = response.orders(i);

        ui->currGuestsTbl->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(info.guest_firstname())));
        ui->currGuestsTbl->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(info.guest_secondname())));
        ui->currGuestsTbl->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(info.guest_lastname())));
        ui->currGuestsTbl->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(info.startdate())));
        ui->currGuestsTbl->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(info.enddate())));
        ui->currGuestsTbl->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(std::to_string(info.idroom()))));
    }

    ui->currGuestsTbl->resizeColumnsToContents();
    ui->currGuestsTbl->resizeRowsToContents();
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

void ManagerView::isDeleteEmployee(const network::RegisterMessageResponse &responce)
{
    ui->deleteLbl->setText(QString::fromStdString(responce.messagetext()));
    LOG_INFO(responce.messagetext());
}

void ManagerView::isDeleteRoom(const network::RegisterMessageResponse &responce)
{
    //ui->deleteRoomLbl->setText(QString::fromStdString(responce.messagetext()));
    LOG_INFO(responce.messagetext());
}

std::string ManagerView::statusToString(uint32_t status)
{
    if (status == 0) return "Not cleaned";
    else return "Cleaned";
}

void ManagerView::setRoomTypes(const network::RoomTypesMessageResponse &responce)
{
    ui->typeCb->clear();
    ui->typeCbE->clear();
    ui->orderRoomType->clear();
    for (size_t i =0; i< responce.types_size(); i++)
    {
        std::string type = responce.types(i);
        ui->typeCb->addItem(QString::fromStdString(type));
        ui->typeCbE->addItem(QString::fromStdString(type));
        ui->orderRoomType->addItem(QString::fromStdString(type));
    }
    ui->typeCb->setCurrentIndex(0);
    ui->typeCbE->setCurrentIndex(0);    
    ui->orderRoomType->setCurrentIndex(0);
}

void ManagerView::setCountRooms(const network::RegisterMessageResponse &responce)
{
    ui->roomCountLbl->setText(QString::fromStdString(responce.messagetext()));
}

void ManagerView::setCountEmployees(const network::RegisterMessageResponse &responce)
{
    ui->employeeCountLbl->setText(QString::fromStdString(responce.messagetext()));
}

void ManagerView::setCountOrders(const network::RegisterMessageResponse &responce)
{
    ui->orderCountLbl->setText(QString::fromStdString(responce.messagetext()));
}

void ManagerView::setAvgResTime(const network::RegisterMessageResponse &responce)
{
    ui->avgResidenceLbl->setText(QString::fromStdString(responce.messagetext()));
}

void ManagerView::setAvgRoomRating(const network::RegisterMessageResponse &responce)
{
     ui->avgRoomRatingLbl->setText(QString::fromStdString(responce.messagetext()));
}

void ManagerView::setRoomTypesGen(const network::RoomTypesMessageResponse &responce)
{
    ui->RoomTypesTbl->clear();
    ui->RoomTypesTbl->setRowCount(responce.types_size());
    ui->RoomTypesTbl->setColumnCount(1);
    for(size_t i =0; i< responce.types_size(); i++)
    {
        ui->RoomTypesTbl->setHorizontalHeaderItem(0, new QTableWidgetItem("Name"));
        ui->RoomTypesTbl->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(responce.types(i))));
    }

    ui->RoomTypesTbl->resizeColumnsToContents();
    ui->RoomTypesTbl->resizeRowsToContents();
}

//void ManagerView::setRoomTypesOrders(const network::RoomTypesMessageResponse &responce)
//{
//    ui->typeCb->clear();
//    LOG_INFO(responce.types_size());
//    for (size_t i =0; i< responce.types_size(); i++)
//    {
//        std::string type = responce.types(i);
//        ui->orderRoomType->addItem(QString::fromStdString(type));
//    }
//    ui->orderRoomType->setCurrentIndex(0);
//}

std::string ManagerView::getStringDate(QDate date)
{
    std::string res = std::to_string(date.year())+'-'+std::to_string(date.month())+'-'+std::to_string(date.day());
    return res;
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
        message_manager_->getHotelEmployees(hotelID_);
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


void ManagerView::on_RoomsTbl_itemClicked(QTableWidgetItem *item)
{
    int row  = item->row();
    TableItemRooms *item1 = static_cast<TableItemRooms *>(ui->RoomsTbl->item(row, 0));
    currRoom = item1->info;
    //ui->deleteRoomLbl->setText(QString::fromStdString(std::to_string(currRoom.id())));
    ui->placesCbE->setCurrentText(ui->RoomsTbl->item(row, 0)->text());
    ui->priceRTEE->setPlainText(ui->RoomsTbl->item(row, 1)->text());
    ui->ratingCbE->setCurrentText(ui->RoomsTbl->item(row, 2)->text());
    ui->statusCbE->setCurrentText(ui->RoomsTbl->item(row, 3)->text());
    ui->floorRTEE->setPlainText(ui->RoomsTbl->item(row, 4)->text());
    ui->typeCbE->setCurrentText(ui->RoomsTbl->item(row, 5)->text());
}


void ManagerView::on_editBtn_clicked()
{
    std::string fname = ui->firstNEdit->toPlainText().toStdString();
    std::string sname = ui->secondNEdit->toPlainText().toStdString();
    std::string lname = ui->lastNEdit->toPlainText().toStdString();
    std::string phone = ui->phoneEdit->toPlainText().toStdString();
    uint64_t salary = ui->salaryEdit->toPlainText().toInt(new bool,10);
    message_manager_->editEmployee(currEmployee.id(),fname,sname,lname,phone,salary,currEmployee.position(),currEmployee.hotelid());
    message_manager_->getHotelEmployees(hotelID_);
}


void ManagerView::on_deleteBtn_clicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Delete employee");
    msgBox.setText("Are you sure?");
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if(msgBox.exec() == QMessageBox::Yes)
    {
        message_manager_->deleteEmployee(currEmployee.id());
        message_manager_->getHotelEmployees(hotelID_);
    }
    else
    {
      // do something else
    }
}



void ManagerView::on_hotelRoomsBtn_clicked()
{
    message_manager_->getHotelRooms(hotelID_);
}


void ManagerView::on_deleteRoomBtn_clicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Delete room");
    msgBox.setText("Are you sure?");
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if(msgBox.exec() == QMessageBox::Yes)
    {
        message_manager_->deleteRoom(currRoom.id());
        message_manager_->getHotelRooms(hotelID_);
    }
    else
    {
      // do something else
    }
}

void ManagerView::on_addRoomBtn_clicked()
{
    if(!ui->priceRTE->toPlainText().isEmpty() && !ui->floorRTE->toPlainText().isEmpty())
    {
        uint32_t places = ui->placesCb->currentText().toInt();
        uint32_t price = ui->priceRTE->toPlainText().toInt();
        uint32_t rating = ui->ratingCb->currentText().toInt();
        bool status = ui->statusCb->currentIndex();
        uint32_t floor = ui->floorRTE->toPlainText().toInt(new bool,10);
        std::string type = ui->typeCb->currentText().toStdString();
        message_manager_->addRoom(places,price,rating,status,floor,type, hotelID_);
        message_manager_->getHotelRooms(hotelID_);
    }
}

void ManagerView::on_addRoomBtn_2_clicked()
{
    uint32_t places = ui->placesCbE->currentText().toInt();
    uint32_t price = ui->priceRTEE->toPlainText().toInt();
    uint32_t rating = ui->ratingCbE->currentText().toInt();
    bool status = ui->statusCbE->currentIndex();
    uint32_t floor = ui->floorRTEE->toPlainText().toInt(new bool,10);
    std::string type = ui->typeCbE->currentText().toStdString();
    message_manager_->editRoom(currRoom.id(),places,price,rating,status,floor,type, hotelID_);
    message_manager_->getHotelRooms(hotelID_);
}


void ManagerView::on_chooseBtn_clicked()
{
    if(ui->comboBox_2->currentText().toStdString()=="Registration")
    {
        ui->newRoomBox->setVisible(true);
        ui->editRoomBox->setVisible(false);
    }
    else
    {
        ui->newRoomBox->setVisible(false);
        ui->editRoomBox->setVisible(true);
    }
}


void ManagerView::on_guestBtn_clicked()
{
    message_manager_->getGuests();
}

void ManagerView::on_tabWidget_tabBarClicked(int index)
{
    message_manager_->getHotels();
    message_manager_->getRoomTypes();
    message_manager_->getHotelTypes();    
    message_manager_->getHotelOrders(hotelID_);
    message_manager_->getGuests();
    message_manager_->countHotelRooms(hotelID_);
    message_manager_->getHotelRooms(hotelID_);
    message_manager_->getHotelEmployees(hotelID_);
    message_manager_->countHotelEmployees(hotelID_);
    message_manager_->countHotelOrders(hotelID_);
    message_manager_->avgResidenceTime(hotelID_);
    message_manager_->avgRoomRating(hotelID_);

}

void ManagerView::on_vacantRoms_clicked()
{
    std::string datebegin = getStringDate(ui->dateBegin->date());
    std::string dateend = getStringDate(ui->dateEnd->date());
    uint32_t capacity = ui->orderplacesCb->currentText().toInt(new bool,10);
    uint32_t startPrice = ui->orderPriceStartCb->currentText().toInt(new bool,10);
    uint32_t endPrice = ui->orderPriceEndCb->currentText().toInt(new bool,10);
    uint32_t startRating = ui->orderRatingStartCb->currentText().toInt(new bool,10);
    uint32_t endRating = ui->orderRatingEndCb->currentText().toInt(new bool,10);
    std::string room_type = ui->orderRoomType->currentText().toStdString();
    message_manager_->getVacantRooms(datebegin,dateend,capacity,startPrice,endPrice,startRating,endRating,room_type,hotelID_);
}

void ManagerView::on_orderRatingStartCb_currentIndexChanged(const QString &arg1)
{
    ui->orderRatingEndCb->clear();
    uint32_t value = arg1.toInt(new bool,10);
    LOG_INFO(value);
    for (size_t i = value; i< 11; i++)
    {
        std::string type = std::to_string(i);
        ui->orderRatingEndCb->addItem(QString::fromStdString(type));
        //ui->typeCbE->addItem(QString::fromStdString(type));
    }
    ui->orderRatingEndCb->setCurrentIndex(0);
    //ui->typeCbE->setCurrentIndex(0);
}

void ManagerView::on_vacantRoomsTbl_itemClicked(QTableWidgetItem *item)
{
    int row  = item->row();
    TableItemRooms *item1 = static_cast<TableItemRooms *>(ui->vacantRoomsTbl->item(row, 0));
    currRoom = item1->info;
    //ui->label_16->setText(QString::fromStdString(std::to_string(currRoom.id())));
}

void ManagerView::on_GuestsTbl_itemClicked(QTableWidgetItem *item)
{
    int row  = item->row();
    TableItemGuest *item1 = static_cast<TableItemGuest *>(ui->GuestsTbl->item(row, 0));
    currGuest= item1->info;
    //ui->label_15->setText(QString::fromStdString(std::to_string(currGuest.id())));
}

void ManagerView::on_regGuestBnt_2_clicked()
{
    if(!ui->loginGPT_2->toPlainText().isEmpty() && !ui->passwordGPT_2->toPlainText().isEmpty())
    {
        std::string login = ui->loginGPT_2->toPlainText().toStdString();
        std::string password = ui->passwordGPT_2->toPlainText().toStdString();
        std::string fname = ui->firstNGPT_2->toPlainText().toStdString();
        std::string sname = ui->secondNGPT_2->toPlainText().toStdString();
        std::string lname = ui->lastNGPT_2->toPlainText().toStdString();
        std::string phone = ui->phoneGPT_2->toPlainText().toStdString();
        std::string passport = ui->passportGPT_2->toPlainText().toStdString();
        message_manager_->createGuest(login, password, fname,sname,lname,phone,passport,static_cast<uint32_t>(Roles::role_manager));
        message_manager_->getGuests();
    }
}

void ManagerView::on_makeOrderBtn_clicked()
{
    //LOG_INFO(std::to_string(currGuest.id())+ " " + std::to_string(manager_id) + std::to_string(currRoom.id()));
    message_manager_->makeOrder(getStringDate(ui->dateBegin->date()),getStringDate(ui->dateEnd->date()),currRoom.id(),manager_id,currGuest.id());
}

void ManagerView::on_tabWidget_currentChanged(int index)
{

}

void ManagerView::on_currentGuests_clicked()
{
    message_manager_->getCurrentGuests(hotelID_);
}
