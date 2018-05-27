#include "ReceptionistView.h"
#include "ui_ReceptionistView.h"

ReceptionistView::ReceptionistView(std::shared_ptr<MessageManager> message_manager, QWidget *parent) :
    message_manager_(message_manager), QWidget(parent),
    ui(new Ui::ReceptionistView)
{
    ui->setupUi(this);
}

ReceptionistView::~ReceptionistView()
{
    delete ui;
}

void ReceptionistView::on_ReceptionistView_destroyed()
{
    emit enableGb();
}
