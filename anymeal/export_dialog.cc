#include "export_dialog.hh"


using namespace std;

ExportDialog::ExportDialog(QWidget *parent):
  QDialog(parent)
{
  m_ui.setupUi(this);
}

string ExportDialog::encoding(void) {
  return m_ui.encoding_combo->currentText().toUtf8().constData();
}
