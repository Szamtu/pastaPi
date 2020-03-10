#ifndef RECENTCHANGESDIALOG_H
#define RECENTCHANGESDIALOG_H

#include <QDialog>

namespace Ui {
class RecentChangesDialog;
}

class RecentChangesDialog : public QDialog {
  Q_OBJECT

 public:
  explicit RecentChangesDialog(QString const a_text, QWidget *parent = nullptr);
  ~RecentChangesDialog();

 private:
  Ui::RecentChangesDialog *ui;
};

#endif // RECENTCHANGESDIALOG_H
