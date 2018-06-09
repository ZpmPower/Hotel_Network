#include "ReceptionistView.h"
#include "ui_ReceptionistView.h"
#include "QMessageBox"

ReceptionistView::ReceptionistView(std::shared_ptr<MessageManager> message_manager, uint32_t hotelid,uint32_t employee_id, QWidget *parent) :
    message_manager_(message_manager), hotelID_(hotelid),receptionist_id(employee_id),QWidget(parent),
    ui(new Ui::ReceptionistView)
{
    message_manager->setOnReadCB(std::bind(&ReceptionistView::onRead, this, std::placeholders::_1));  
    ui->setupUi(this);
    message_manager->getRoomTypes();
    QDate date = QDate::currentDate();
    ui->dateBegin->setDate(date);
    ui->dateBegin->setMinimumDate(date);
    ui->dateEnd->setMinimumDate(date);
    message_manager_->getHotelEmployees(hotelID_);
}

ReceptionistView::~ReceptionistView()
{
    delete ui;
}

void ReceptionistView::onRead(const network::ResponseContext &response)
{
    switch (response.message_type_()) {
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

void ReceptionistView::on_ReceptionistView_destroyed()
{
    emit enableGb();
}

void ReceptionistView::on_hotelRoomsBtn_clicked()
{
    message_manager_->getHotelRooms(hotelID_);
}

void ReceptionistView::on_vacantRoms_clicked()
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

void ReceptionistView::on_makeOrderBtn_clicked()
{
    message_manager_->makeOrder(getStringDate(ui->dateBegin->date()),getStringDate(ui->dateEnd->date()),currRoom.id(),receptionist_id,currGuest.id());
}

void ReceptionistView::on_tabWidget_tabBarClicked(int index)
{
    message_manager_->getHotels();
    message_manager_->getRoomTypes();
    message_manager_->getHotelTypes();
    message_manager_->getHotelOrders(hotelID_);
    message_manager_->getGuests();
    message_manager_->countHotelRooms(hotelID_);
    message_manager_->getHotelRooms(hotelID_);
    message_manager_->countHotelOrders(hotelID_);
    message_manager_->avgResidenceTime(hotelID_);
    message_manager_->avgRoomRating(hotelID_);
}

std::string ReceptionistView::getStringDate(QDate date)
{
    std::string res = std::to_string(date.year())+'-'+std::to_string(date.month())+'-'+std::to_string(date.day());
    return res;
}

void ReceptionistView::getHotelRooms(const network::RoomsMessageResponse &response)
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

void ReceptionistView::getVacantRooms(const network::RoomsMessageResponse &response)
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

void ReceptionistView::getGuests(const network::GuestsMessageResponse &response)
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

void ReceptionistView::getHotels(const network::HotelsMessageResponse &response)
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

void ReceptionistView::getHotelTypes(const network::HotelTypesMessageResponse &response)
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

void ReceptionistView::getHotelOrders(const network::OrdersMessageResponse &response)
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

void ReceptionistView::getGuestOrders(const network::GuestOrdersMessageResponse &response)
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

void ReceptionistView::setRoomTypes(const network::RoomTypesMessageResponse &responce)
{
    ui->orderRoomType->clear();
    for (size_t i =0; i< responce.types_size(); i++)
    {
        std::string type = responce.types(i);
        ui->orderRoomType->addItem(QString::fromStdString(type));
    }
    ui->orderRoomType->setCurrentIndex(0);
}

std::string ReceptionistView::statusToString(uint32_t status)
{
    if (status == 0) return "Not cleaned";
    else return "Cleaned";
}

void ReceptionistView::setCountRooms(const network::RegisterMessageResponse &responce)
{
    ui->roomCountLbl->setText(QString::fromStdString(responce.messagetext()));
}

void ReceptionistView::setCountEmployees(const network::RegisterMessageResponse &responce)
{
    ui->employeeCountLbl->setText(QString::fromStdString(responce.messagetext()));
}

void ReceptionistView::setCountOrders(const network::RegisterMessageResponse &responce)
{
    ui->orderCountLbl->setText(QString::fromStdString(responce.messagetext()));
}

void ReceptionistView::setAvgResTime(const network::RegisterMessageResponse &responce)
{
    ui->avgResidenceLbl->setText(QString::fromStdString(responce.messagetext()));
}

void ReceptionistView::setAvgRoomRating(const network::RegisterMessageResponse &responce)
{
     ui->avgRoomRatingLbl->setText(QString::fromStdString(responce.messagetext()));
}

void ReceptionistView::setRoomTypesGen(const network::RoomTypesMessageResponse &responce)
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

void ReceptionistView::on_deleteRoomBtn_clicked()
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
