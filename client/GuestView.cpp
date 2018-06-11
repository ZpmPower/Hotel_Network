#include "GuestView.h"
#include "ui_GuestView.h"

GuestView::GuestView(std::shared_ptr<MessageManager> message_manager, uint32_t guest_id, QWidget *parent) :
    message_manager_(message_manager),guestId_(guest_id),QWidget(parent),
    ui(new Ui::GuestView)
{
    message_manager->setOnReadCB(std::bind(&GuestView::onRead, this, std::placeholders::_1));
    ui->setupUi(this);

    message_manager_->getRoomTypes();
    message_manager_->getHotels();

    ui->tabWidget->setCurrentIndex(0);
}

GuestView::~GuestView()
{
    delete ui;
}

void GuestView::onRead(const network::ResponseContext &response)
{
    switch (response.message_type_()) {
    case network::HN_GET_VACANT_ROOMS: {
        network::RoomsMessageResponse roomsRes = response.rooms();
        getVacantRooms(roomsRes);
        break;
    }
    case network::HN_GET_ROOM_TYPES: {
        network::RoomTypesMessageResponse typesRes = response.types();
        setRoomTypes(typesRes);
        break;
    }
    case network::HN_GET_ALL_HOTELS: {
        network::HotelsMessageResponse hotelsRes = response.hotels();
        getHotels(hotelsRes);
        break;
    }
    case network::HN_GET_VACANT_ROOMS_GUEST: {
        network::RoomsMessageResponse roomsRes = response.rooms();
        getVacantRooms(roomsRes);
        break;
    }
    }
}

void GuestView::on_tabWidget_tabBarClicked(int index)
{
    message_manager_->getHotels();
    message_manager_->getRoomTypes();
}

void GuestView::getHotels(const network::HotelsMessageResponse &response)
{
    ui->hotelCb->clear();
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
        ui->hotelCb->addItem(QString::fromStdString(info.name()));

        ui->HotelsTbl->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(info.name())));
        ui->HotelsTbl->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(info.city())));
        ui->HotelsTbl->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(info.street())));
        ui->HotelsTbl->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(info.phonenumber())));
        ui->HotelsTbl->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(info.email())));
        ui->HotelsTbl->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(info.type())));
    }

    ui->HotelsTbl->resizeColumnsToContents();
    ui->HotelsTbl->resizeRowsToContents();
    ui->hotelCb->setCurrentIndex(0);
}

void GuestView::getVacantRooms(const network::RoomsMessageResponse &response)
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

void GuestView::setRoomTypes(const network::RoomTypesMessageResponse &responce)
{
    ui->orderRoomType->clear();
    for (size_t i = 0; i< responce.types_size(); i++)
    {
        std::string type = responce.types(i);
        ui->orderRoomType->addItem(QString::fromStdString(type));
    }
    ui->orderRoomType->setCurrentIndex(0);
}

void GuestView::on_vacantRoms_clicked()
{
    std::string datebegin = getStringDate(ui->dateBegin->date());
    std::string dateend = getStringDate(ui->dateEnd->date());
    uint32_t capacity = ui->orderplacesCb->currentText().toInt(new bool,10);
    uint32_t startPrice = ui->orderPriceStartCb->currentText().toInt(new bool,10);
    uint32_t endPrice = ui->orderPriceEndCb->currentText().toInt(new bool,10);
    uint32_t startRating = ui->orderRatingStartCb->currentText().toInt(new bool,10);
    uint32_t endRating = ui->orderRatingEndCb->currentText().toInt(new bool,10);
    std::string room_type = ui->orderRoomType->currentText().toStdString();
    std::string hotel_type = ui->hotelCb->currentText().toStdString();
    message_manager_->getVacantRoomsGuest(datebegin,dateend,capacity,startPrice,endPrice,startRating,endRating,room_type,hotel_type);
}

std::string GuestView::statusToString(uint32_t status)
{
    if (status == 0) return "Not cleaned";
    else return "Cleaned";
}

std::string GuestView::getStringDate(QDate date)
{
    std::string res = std::to_string(date.year())+'-'+std::to_string(date.month())+'-'+std::to_string(date.day());
    return res;
}

void GuestView::on_makeOrderBtn_clicked()
{
    message_manager_->makeOrder(getStringDate(ui->dateBegin->date()),getStringDate(ui->dateEnd->date()),currRoom.id(),0,guestId_);
}

void GuestView::on_vacantRoomsTbl_itemClicked(QTableWidgetItem *item)
{
    int row  = item->row();
    TableItemRooms *item1 = static_cast<TableItemRooms *>(ui->vacantRoomsTbl->item(row, 0));
    currRoom = item1->info;
    LOG_INFO(currRoom.id());
}
