#ifndef ADMINVIEW_H
#define ADMINVIEW_H

#include <QWidget>
#include "MessageManager.h"

namespace Ui {
class AdminView;
}

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

private:
    Ui::AdminView *ui;
    std::shared_ptr<MessageManager> message_manager_;
    void getGuests(const network::GuestsMessageResponse& response);
    void getEmployees(const network::EmployeesMessageResponse& response);
    void getHotels(const network::HotelsMessageResponse& response);
    void getRooms(const network::RoomsMessageResponse& response);
    void guestRegister(const network::RegisterMessageResponse &response);

};

#endif // ADMINVIEW_H
