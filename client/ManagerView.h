#ifndef MANAGERVIEW_H
#define MANAGERVIEW_H

#include <QWidget>
#include <QTableWidgetItem>
#include "MessageManager.h"
#include "QDateTimeEdit"


namespace Ui {
class ManagerView;
}


class TableItemEmployees: public QTableWidgetItem
{
public:
    TableItemEmployees();
    TableItemEmployees(QString str)
        :QTableWidgetItem(str){

    }
    network::EmployeeInfo info;
};

class TableItemRooms: public QTableWidgetItem
{
public:
    TableItemRooms();
    TableItemRooms(QString str)
        :QTableWidgetItem(str){

    }
    network::RoomInfo info;
};

class TableItemOrders: public QTableWidgetItem
{
public:
    TableItemOrders();
    TableItemOrders(QString str)
        :QTableWidgetItem(str){

    }
    network::OrderInfo info;
};
class TableItemGuest: public QTableWidgetItem
{
public:
    TableItemGuest();
    TableItemGuest(QString str)
        :QTableWidgetItem(str){

    }
    network::GuestInfo info;
};

class ManagerView : public QWidget
{
    Q_OBJECT

public:
    explicit ManagerView(std::shared_ptr<MessageManager> message_manager, uint32_t hoteid, uint32_t employee_id, QWidget *parent = 0);
    ~ManagerView();
    void onRead(const network::ResponseContext &response);

private slots:
    void on_hotelEmployeesBtn_clicked();

    void on_regReceptBtn_clicked();

    void on_pushButton_clicked();

    void on_EmployeesTbl_itemClicked(QTableWidgetItem *item);

    void on_editBtn_clicked();

    void on_deleteBtn_clicked();

    void on_hotelRoomsBtn_clicked();

    void on_RoomsTbl_itemClicked(QTableWidgetItem *item);

    void on_deleteRoomBtn_clicked();

    void on_addRoomBtn_clicked();

    void on_addRoomBtn_2_clicked();

    void on_chooseBtn_clicked();

    void on_guestBtn_clicked();

    void on_tabWidget_tabBarClicked(int index);

    void on_vacantRoms_clicked();

    void on_orderRatingStartCb_currentIndexChanged(const QString &arg1);
    void on_vacantRoomsTbl_itemClicked(QTableWidgetItem *item);

    void on_GuestsTbl_itemClicked(QTableWidgetItem *item);

    void on_regGuestBnt_2_clicked();

    void on_makeOrderBtn_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_currentGuests_clicked();

private:
    Ui::ManagerView *ui;
    uint32_t hotelID_;
    uint32_t manager_id;
    network::SessionInfo sessionInfo_;
    std::shared_ptr<MessageManager> message_manager_;
    void getHotelEmployees(const network::EmployeesMessageResponse& response);
    void getHotelRooms(const network::RoomsMessageResponse &response);
    void getVacantRooms(const network::RoomsMessageResponse &response);
    void getGuests(const network::GuestsMessageResponse &response);
    void getHotels(const network::HotelsMessageResponse &response);
    void getHotelTypes(const network::HotelTypesMessageResponse &response);
    void getHotelOrders(const network::OrdersMessageResponse &response);
    void getGuestOrders(const network::GuestOrdersMessageResponse &response);
    void receptionistRegister(const network::RegisterMessageResponse &response);
    uint32_t stringToRole(const std::string& role);
    void isUpdated(const network::RegisterMessageResponse &responce);
    void isDeleteEmployee(const network::RegisterMessageResponse &responce);
    void isDeleteRoom(const network::RegisterMessageResponse &responce);
    std::string statusToString(uint32_t status);
    void setRoomTypes(const network::RoomTypesMessageResponse &responce);
    void setCountRooms(const network::RegisterMessageResponse &responce);
    void setCountEmployees(const network::RegisterMessageResponse &responce);
    void setCountOrders(const network::RegisterMessageResponse &responce);
    void setAvgResTime(const network::RegisterMessageResponse &responce);
    void setAvgRoomRating(const network::RegisterMessageResponse &responce);
    void setRoomTypesGen(const network::RoomTypesMessageResponse &responce);
    //void setRoomTypesOrders(const network::RoomTypesMessageResponse &responce);
    std::string getStringDate(QDate date);

    network::EmployeeInfo currEmployee;
    network::RoomInfo currRoom;
    network::GuestInfo currGuest;
};

#endif // MANAGERVIEW_H
