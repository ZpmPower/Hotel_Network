#include "ManagerView.h"
#include "ui_ManagerView.h"

ManagerView::ManagerView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ManagerView)
{
    ui->setupUi(this);
}

ManagerView::~ManagerView()
{
    delete ui;
}
