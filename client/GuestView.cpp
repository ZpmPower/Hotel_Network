#include "GuestView.h"
#include "ui_GuestView.h"

GuestView::GuestView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GuestView)
{
    ui->setupUi(this);
}

GuestView::~GuestView()
{
    delete ui;
}
