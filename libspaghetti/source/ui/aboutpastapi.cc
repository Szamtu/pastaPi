#include "aboutpastapi.h"
#include "spaghetti/version.h"
#include "ui_aboutpastapi.h"

AboutPastaPI::AboutPastaPI(QWidget *parent)
  : QDialog(parent)
  , ui(new Ui::AboutPastaPI)
{
  ui->setupUi(this);
  connect(ui->closeButton, &QPushButton::clicked, this, [this]() { close(); });

  ui->versionLabel->setTextFormat(Qt::RichText);
  ui->versionLabel->setText(QString("Version: </b>%1</b><br>"
                                    "Build date: <b>%2, %3</b><br>"
                                    "Git branch: <b>%4</b><br>"
                                    "Git commit: <b>%5</b><br>")
                                .arg(spaghetti::version::STRING)
                                .arg(__DATE__)
                                .arg(__TIME__)
                                .arg(spaghetti::version::BRANCH)
                                .arg(spaghetti::version::COMMIT_SHORT_HASH));
}

AboutPastaPI::~AboutPastaPI()
{
  delete ui;
}
