#ifndef ADMINVIEW_H
#define ADMINVIEW_H

#include <QWidget>
#include <QTableWidgetItem>
#include "MessageManager.h"

namespace Ui {
class AdminView;
}

class TableItemHotels: public QTableWidgetItem
{
public:
    TableItemHotels();
    TableItemHotels(QString str)
        :QTableWidgetItem(str){

    }
    network::HotelInfo info;
};

class AdminView : public QWidget
{
    Q_OBJECT

public:
    explicit AdminView(std::shared_ptr<MessageManager> message_manager, QWidget *parent = 0);
    ~AdminView();
    void onRead(const network::ResponseContext &response);

private slots:
    void on_okBtn_clicked();
    void on_tabWidget_currentChanged(int index);


    void on_allGuestsBtn_clicked();

    void on_tabWidget_tabBarClicked(int index);

    void on_regGuestBnt_clicked();

    void on_regReceptBtn_clicked();
    void on_regManagerBtn_clicked();

    void on_allEmployeeBtn_clicked();

    void on_allHotelsBtn_clicked();

    void on_allRoomsBtn_clicked();

    void on_HotelsTbl_itemClicked(QTableWidgetItem *item);

private:
    Ui::AdminView *ui;
    std::shared_ptr<MessageManager> message_manager_;
    void getGuests(const network::GuestsMessageResponse& response);
    void getEmployees(const network::EmployeesMessageResponse& response);
    void getHotels(const network::HotelsMessageResponse& response);
    void getRooms(const network::RoomsMessageResponse& response);
    void guestRegister(const network::RegisterMessageResponse &response);

    network::HotelInfo currHotel;

};

#endif // ADMINVIEW_H
