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

private:
    Ui::GuestView *ui;
    network::SessionInfo sessionInfo_;
    std::shared_ptr<MessageManager> message_manager_;
    std::string statusToString(uint32_t status);
    std::string getStringDate(QDate date);
    void getHotels(const network::HotelsMessageResponse &response);
    void getVacantRooms(const network::RoomsMessageResponse &response);
    void setRoomTypes(const network::RoomTypesMessageResponse &responce);

    network::RoomInfo currRoom;
    uint32_t guestId_;
};

#endif // GUESTVIEW_H
