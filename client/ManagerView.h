#ifndef MANAGERVIEW_H
#define MANAGERVIEW_H

#include <QWidget>
#include <QTableWidgetItem>
#include "MessageManager.h"

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

class ManagerView : public QWidget
{
    Q_OBJECT

public:
    explicit ManagerView(std::shared_ptr<MessageManager> message_manager, uint32_t hoteid, QWidget *parent = 0);
    ~ManagerView();
    void onRead(const network::ResponseContext &response);

private slots:
    void on_hotelEmployeesBtn_clicked();

    void on_regReceptBtn_clicked();

    void on_pushButton_clicked();

    void on_EmployeesTbl_itemClicked(QTableWidgetItem *item);

    void on_editBtn_clicked();

    void on_deleteBtn_clicked();

private:
    Ui::ManagerView *ui;
    uint32_t hotelID_;
    network::SessionInfo sessionInfo_;
    std::shared_ptr<MessageManager> message_manager_;
    void getHotelEmployees(const network::EmployeesMessageResponse& response);
    void receptionistRegister(const network::RegisterMessageResponse &response);
    uint32_t stringToRole(const std::string& role);
    void isUpdated(const network::RegisterMessageResponse &responce);

    network::EmployeeInfo currEmployee;
};

#endif // MANAGERVIEW_H
