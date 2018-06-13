#ifndef GUESTVIEW_H
#define GUESTVIEW_H

#include <QWidget>
#include "MessageManager.h"
#include "ManagerView.h"

namespace Ui {
class GuestView;
}

class GuestView : public QWidget
{
    Q_OBJECT

public:
    explicit GuestView(std::shared_ptr<MessageManager> message_manager,uint32_t guest_id, QWidget *parent = 0);
    ~GuestView();    
    void onRead(const network::ResponseContext &response);

private slots:
    void on_tabWidget_tabBarClicked(int index);

    void on_vacantRoms_clicked();

    void on_makeOrderBtn_clicked();

    void on_vacantRoomsTbl_itemClicked(QTableWidgetItem *item);

    void on_orderPriceStartCb_currentIndexChanged(int index);

    void onReadData(std::string data);

    void on_editBtn_clicked();

    void on_orderRatingStartCb_currentTextChanged(const QString &arg1);

    void on_dateBegin_dateChanged(const QDate &date);

signals:
    void readData(std::string);

private:
    Ui::GuestView *ui;
    network::SessionInfo sessionInfo_;
    std::shared_ptr<MessageManager> message_manager_;
    std::string statusToString(uint32_t status);
    std::string getStringDate(QDate date);
    void getHotels(const network::HotelsMessageResponse &response);
    void getGuestOrders(const network::OrdersMessageResponse &response);
    void getVacantRooms(const network::RoomsMessageResponse &response);
    void setRoomTypes(const network::RoomTypesMessageResponse &responce);

    network::RoomInfo currRoom;
    network::GuestInfo thisGuest;
    uint32_t guestId_;
};

#endif // GUESTVIEW_H
