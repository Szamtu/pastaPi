#include "recentchangesdialog.h"
#include "ui_recentchangesdialog.h"

RecentChangesDialog::RecentChangesDialog(QString const a_text, QWidget *parent)
  : QDialog(parent)
  , ui(new Ui::RecentChangesDialog)
{
  ui->setupUi(this);
  ui->changelogText->setPlainText(a_text);
  connect(ui->closeButton, &QPushButton::clicked, this, &RecentChangesDialog::accept);
}

RecentChangesDialog::~RecentChangesDialog()
{
  delete ui;
}
