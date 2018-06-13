#include "GuestView.h"
#include "ui_GuestView.h"
#include <regex>
#include <qmessagebox.h>

GuestView::GuestView(std::shared_ptr<MessageManager> message_manager, uint32_t guest_id, QWidget *parent) :
    message_manager_(message_manager),guestId_(guest_id),QWidget(parent),
    ui(new Ui::GuestView)
{
    message_manager->setOnReadCB(std::bind(&GuestView::onRead, this, std::placeholders::_1));
    ui->setupUi(this);

    message_manager_->getRoomTypes();
    message_manager_->getHotels();
    QDate date = QDate::currentDate();
    QDate nextday(date.year(),date.month(),date.day()+1);
    ui->dateBegin->setDate(date);
    ui->dateBegin->setMinimumDate(date);
    ui->dateEnd->setMinimumDate(nextday);
    ui->tabWidget->setCurrentIndex(0);
    message_manager_->getGuestbyId(guest_id);
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
    case network::HN_MAKE_ORDER: {
        emit readData(response.SerializeAsString());
        break;
    }
    case network::HN_GET_GUEST_ALL_ORDERS: {
        network::OrdersMessageResponse ordersRes = response.orders();
        getGuestOrders(ordersRes);
        break;
    }
    case network::HN_GET_GUEST_BY_ID: {
        network::GuestInfo guestRes = response.guest_info();
        thisGuest.set_firstname(guestRes.firstname());
        thisGuest.set_secondname(guestRes.secondname());
        thisGuest.set_lastname(guestRes.lastname());
        thisGuest.set_phonenumber(guestRes.phonenumber());
        thisGuest.set_passport(guestRes.passport());
        ui->helloLbl->setText(QString::fromStdString("Hello, "+this->thisGuest.firstname() + " " + thisGuest.secondname()));
        break;
    }
    case network::HN_EDIT_GUEST: {
        network::RegisterMessageResponse res = response.register_response();
        ui->editLbl->setText(QString::fromStdString(res.messagetext()));
        break;
    }
    }
}

void GuestView::on_tabWidget_tabBarClicked(int index)
{
    message_manager_->getHotels();
    message_manager_->getRoomTypes();
    message_manager_->getGuestbyId(guestId_);
    message_manager_->getGuestsOrders(guestId_);
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

void GuestView::getGuestOrders(const network::OrdersMessageResponse &response)
{
    ui->guestOrders->clear();
    ui->guestOrders->setRowCount(response.orders_size());
    ui->guestOrders->setColumnCount(4);

    ui->guestOrders->setHorizontalHeaderItem(0, new QTableWidgetItem("StartDate"));
    ui->guestOrders->setHorizontalHeaderItem(1, new QTableWidgetItem("EndDate"));
    ui->guestOrders->setHorizontalHeaderItem(2, new QTableWidgetItem("Idroom"));
    ui->guestOrders->setHorizontalHeaderItem(3, new QTableWidgetItem("HotelId"));
    for(size_t i =0; i< response.orders_size(); i++)
    {
        network::OrderInfo info = response.orders(i);
        ui->guestOrders->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(info.startdate())));
        ui->guestOrders->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(info.enddate())));
        ui->guestOrders->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(std::to_string(info.idroom()))));
        ui->guestOrders->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(std::to_string(info.hotelid()))));
    }

    ui->guestOrders->resizeColumnsToContents();
    ui->guestOrders->resizeRowsToContents();
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

//void GuestView::on_testBtn_clicked()
//{

//      double a = -1; //Начало интервала, где рисуем график по оси Ox
//      double b =  1; //Конец интервала, где рисуем график по оси Ox
//      double h = 0.01; //Шаг, с которым будем пробегать по оси Ox

//      int N=(b-a)/h + 2; //Вычисляем количество точек, которые будем отрисовывать
//      QVector<double> x(N), y(N); //Массивы координат точек

//      //Вычисляем наши данные
//      int i=0;
//      for (double X=a; X<=b; X+=h)//Пробегаем по всем точкам
//      {
//          x[i] = X;
//          y[i] = X*X;//Формула нашей функции
//          i++;
//      }

//      ui->widget->clearGraphs();//Если нужно, но очищаем все графики
//      //Добавляем один график в widget
//      ui->widget->addGraph();
//      //Говорим, что отрисовать нужно график по нашим двум массивам x и y
//      ui->widget->graph(0)->setData(x, y);

//      //Подписываем оси Ox и Oy
//      ui->widget->xAxis->setLabel("x");
//      ui->widget->yAxis->setLabel("y");

//      //Установим область, которая будет показываться на графике
//      ui->widget->xAxis->setRange(a, b);//Для оси Ox

//      //Для показа границ по оси Oy сложнее, так как надо по правильному
//      //вычислить минимальное и максимальное значение в векторах
//      double minY = y[0], maxY = y[0];
//      for (int i=1; i<N; i++)
//      {
//          if (y[i]<minY) minY = y[i];
//          if (y[i]>maxY) maxY = y[i];
//      }
//      ui->widget->yAxis->setRange(minY, maxY);//Для оси Oy

//      //И перерисуем график на нашем widget
//      ui->widget->replot();
//}

//void GuestView::on_dateBegin_dateChanged(const QDate &date)
//{
//    QDate newDate(date.year(),date.month(),date.day()+1);
//    ui->dateEnd->setMinimumDate(newDate);
//}

//void GuestView::on_orderRatingStartCb_currentIndexChanged(const QString &arg1)
//{
//    ui->orderRatingEndCb->clear();
//    uint32_t value = arg1.toInt(new bool,10);
//    for (size_t i = value; i< 11; i++)
//    {
//        std::string type = std::to_string(i);
//        ui->orderRatingEndCb->addItem(QString::fromStdString(type));
//    }
//    ui->orderRatingEndCb->setCurrentIndex(0);
//}

//void GuestView::on_orderPriceStartCb_currentIndexChanged(int index)
//{
//    ui->orderPriceEndCb->clear();
//    for (size_t i=index; i<ui->orderPriceStartCb->count();i++)
//    {
//        ui->orderPriceEndCb->addItem(ui->orderPriceStartCb->itemText(i));
//    }
//}

void GuestView::onReadData(std::string data)
{
    network::ResponseContext response;
    response.ParseFromString(data);
    network::RegisterMessageResponse res = response.register_response();
    if(res.status() == true) QMessageBox::information(this, "Order status","Your order's processed succesfull");
    else QMessageBox::information(this, "Order status","Something wrong");
}



void GuestView::on_editBtn_clicked()
{
    if(!ui->firstN->toPlainText().isEmpty() && !ui->secondN->toPlainText().isEmpty() && !ui->passportTE->toPlainText().isEmpty())
    {
        std::string fname = ui->firstN->toPlainText().toStdString();
        std::string sname = ui->secondN->toPlainText().toStdString();
        std::string lname = ui->lastN->toPlainText().toStdString();
        std::string phone = ui->phoneTE->toPlainText().toStdString();
        std::string passport = ui->passportTE->toPlainText().toStdString();
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
                            message_manager_->editGuest(guestId_,fname,sname,lname,phone,passport);
                            message_manager_->getGuestbyId(guestId_);
                        }
                        else
                        {
                            ui->editLbl->setText(QString::fromStdString("Enter corrent Phone_Number"));
                        }
                    }
                    if(phone.empty() && !lname.empty())
                    {
                        if(std::regex_match(lname,regexLastN))
                        {
                            message_manager_->editGuest(guestId_,fname,sname,lname,phone,passport);
                            message_manager_->getGuestbyId(guestId_);
                        }
                        else
                        {
                            ui->editLbl->setText(QString::fromStdString("Enter corrent Last_Name"));
                        }
                    }
                    if(!phone.empty() && !lname.empty())
                    {
                        if(std::regex_match(phone,regexPhone) && std::regex_match(lname,regexLastN))
                        {
                            message_manager_->editGuest(guestId_,fname,sname,lname,phone,passport);
                            message_manager_->getGuestbyId(guestId_);
                        }
                        else
                        {
                             ui->editLbl->setText(QString::fromStdString("Enter corrent Last_Name\nand phone"));
                        }
                    }
                }
                else
                {
                    message_manager_->editGuest(guestId_,fname,sname,lname,phone,passport);
                    message_manager_->getGuestbyId(guestId_);
                }
            }
            else
            {
                ui->editLbl->setText(QString::fromStdString("Check First_Name and Second_Name"));
            }
        }
        else
        {
            ui->editLbl->setText(QString::fromStdString("Passport must have 2 letter\nand 6 numbers"));
        }

    }
    else
    {
        ui->editLbl->setText(QString::fromStdString("Enter all required fields"));
    }
}

void GuestView::on_orderPriceStartCb_currentIndexChanged(int index)
{
    ui->orderPriceEndCb->clear();
    for (size_t i=index; i<ui->orderPriceStartCb->count();i++)
    {
        ui->orderPriceEndCb->addItem(ui->orderPriceStartCb->itemText(i));
    }
}

void GuestView::on_orderRatingStartCb_currentTextChanged(const QString &arg1)
{
    ui->orderRatingEndCb->clear();
    uint32_t value = arg1.toInt(new bool,10);
    for (size_t i = value; i< 11; i++)
    {
        std::string type = std::to_string(i);
        ui->orderRatingEndCb->addItem(QString::fromStdString(type));
    }
    ui->orderRatingEndCb->setCurrentIndex(0);
}

void GuestView::on_dateBegin_dateChanged(const QDate &date)
{
    QDate newDate(date.year(),date.month(),date.day()+1);
    ui->dateEnd->setMinimumDate(newDate);
}
