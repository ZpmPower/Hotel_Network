#include "GuestView.h"
#include "ui_GuestView.h"

GuestView::GuestView(std::shared_ptr<MessageManager> message_manager, QWidget *parent) :
    message_manager_(message_manager), QWidget(parent),
    ui(new Ui::GuestView)
{
    ui->setupUi(this);
}

GuestView::~GuestView()
{
    delete ui;
}
