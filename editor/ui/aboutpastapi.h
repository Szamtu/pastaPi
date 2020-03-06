#ifndef ABOUTPASTAPI_H
#define ABOUTPASTAPI_H

#include <QDialog>

namespace Ui {
  class AboutPastaPI;
}

class AboutPastaPI : public QDialog
{
  Q_OBJECT

public:
  explicit AboutPastaPI(QWidget *parent = nullptr);
  ~AboutPastaPI();

private:
  Ui::AboutPastaPI *ui;
};

#endif // ABOUTPASTAPI_H
