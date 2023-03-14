#include "progressdialog.h"
#include "ui_progressdialog.h"

progressDialog::progressDialog(QWidget *parent) : QWidget(parent), ui(new Ui::progressDialog)
{
    ui->setupUi(this);
    movie = new QMovie(":/icons/icons/gifs/spinning_segments.GIF");
    ui->movieLabel->setMovie(movie);
    movie->start();
}

progressDialog::~progressDialog()
{
    delete ui;
}

void progressDialog::setText(QString text){
QString output = text.replace("\n","");

    ui->progressText->setText(output);
}
