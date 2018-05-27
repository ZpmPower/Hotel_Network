#include "AdminView.h"
#include "ui_AdminView.h"
#include<QMessageBox>

AdminView::AdminView(std::shared_ptr<MessageManager> message_manager, QWidget *parent) :
    message_manager_(message_manager),QWidget(parent),
    ui(new Ui::AdminView)
{
    ui->setupUi(this);
    connect(ui->okBtn, SIGNAL(clicked()), this, SLOT(onClick()));
}

AdminView::~AdminView()
{
    delete ui;
}

void AdminView::on_okBtn_clicked()
{
    QMessageBox::warning(this, "Внимание","Это очень важный текст\n www.itmathrepetitor.ru");

}

void AdminView::on_tabWidget_currentChanged(int index)
{
}
