#include "loginview.h"
#include "ui_loginview.h"
#include "iddatabase.h"

LoginView::LoginView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginView)
{
    ui->setupUi(this);
}

LoginView::~LoginView()
{
    delete ui;
}

void LoginView::on_btSignIn_clicked()
{
    QString status = IDdatabase::getInstance().userLogin(ui->inputUserName->text(),
                                                         ui->inputPassword->text());

    if(status == "loginOK")
        emit loginSuccess();
}

