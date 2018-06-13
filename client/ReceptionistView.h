#ifndef RECEPTIONISTVIEW_H
#define RECEPTIONISTVIEW_H

#include <QWidget>
#include "MessageManager.h"
#include "ManagerView.h"

namespace Ui {
class ReceptionistView;
}

class ReceptionistView : public QWidget
{
    Q_OBJECT

public:
    explicit ReceptionistView(std::shared_ptr<MessageManager> message_manager, uint32_t hotelid,uint32_t employee_id,  QWidget *parent = 0);
    ~ReceptionistView();
    void onRead(const network::ResponseContext &response);

private slots:
    void on_ReceptionistView_destroyed();

    void on_hotelRoomsBtn_clicked();

    void on_deleteRoomBtn_clicked();

    void on_vacantRoms_clicked();

    void on_makeOrderBtn_clicked();

    void on_tabWidget_tabBarClicked(int index);

    void on_currentGuests_clicked();

    void on_vacantRoomsTbl_itemClicked(QTableWidgetItem *item);

    void on_GuestsTbl_itemClicked(QTableWidgetItem *item);

    void on_guestBtn_clicked();

    void on_dateBegin_dateChanged(const QDate &date);
    void onReadData(std::string data);

    void on_orderRatingStartCb_currentIndexChanged(const QString &arg1);

    void on_orderPriceStartCb_currentIndexChanged(int index);

    void on_orderRatingStartCb_currentIndexChanged(int index);

signals:
    void readData(std::string);
    void enableGb();
private:
    Ui::ReceptionistView *ui;
    uint32_t hotelID_;
    uint32_t receptionist_id;
    network::SessionInfo sessionInfo_;
    std::shared_ptr<MessageManager> message_manager_;
    std::string getStringDate(QDate date);
    void getHotelRooms(const network::RoomsMessageResponse &response);
    void getVacantRooms(const network::RoomsMessageResponse &response);
    void getGuests(const network::GuestsMessageResponse &response);
    void getHotels(const network::HotelsMessageResponse &response);
    void getHotelTypes(const network::HotelTypesMessageResponse &response);
    void getHotelOrders(const network::OrdersMessageResponse &response);
    void getGuestOrders(const network::GuestOrdersMessageResponse &response);
    void setRoomTypes(const network::RoomTypesMessageResponse &responce);
    void setCountRooms(const network::RegisterMessageResponse &responce);
    void setCountEmployees(const network::RegisterMessageResponse &responce);
    void setCountOrders(const network::RegisterMessageResponse &responce);
    void setAvgResTime(const network::RegisterMessageResponse &responce);
    void setAvgRoomRating(const network::RegisterMessageResponse &responce);
    void setRoomTypesGen(const network::RoomTypesMessageResponse &responce);
    std::string statusToString(uint32_t status);

    network::RoomInfo currRoom;
    network::GuestInfo currGuest;
};

#endif // RECEPTIONISTVIEW_H
