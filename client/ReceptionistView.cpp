#include "ReceptionistView.h"
#include "ui_ReceptionistView.h"

ReceptionistView::ReceptionistView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReceptionistView)
{
    ui->setupUi(this);
}

ReceptionistView::~ReceptionistView()
{
    delete ui;
}
