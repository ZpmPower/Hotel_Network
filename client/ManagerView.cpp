#include "ManagerView.h"
#include "ui_ManagerView.h"

ManagerView::ManagerView(std::shared_ptr<MessageManager> message_manager, QWidget *parent) :
    message_manager_(message_manager), QWidget(parent),
    ui(new Ui::ManagerView)
{
    ui->setupUi(this);
}

ManagerView::~ManagerView()
{
    delete ui;
}
