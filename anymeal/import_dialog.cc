#include <QtCore/QStandardPaths>
#include <QtWidgets/QFileDialog>
#include "import_dialog.hh"


using namespace std;

ImportDialog::ImportDialog(QWidget *parent):
  QDialog(parent)
{
  m_ui.setupUi(this);
  connect(m_ui.browse_button, &QPushButton::clicked, this, &ImportDialog::select_error_file);
  auto path = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
  QDir dir(path);
  m_ui.error_file_edit->setText(dir.filePath(tr("errors.mmf")));
}

string ImportDialog::encoding(void) {
  return m_ui.encoding_combo->currentText().toUtf8().constData();
}

string ImportDialog::error_file(void) {
  return m_ui.error_file_edit->text().toUtf8().constData();
}

void ImportDialog::select_error_file(void) {
  QString result = QFileDialog::getSaveFileName(this, tr("Select Error File"), "", tr("MealMaster (*.mm *.MM *.mmf *.MMF);;"
                                                "Text (*.txt *.TXT);;All files (*)"));
  if (!result.isEmpty()) {
    m_ui.error_file_edit->setText(result);
  };
}
