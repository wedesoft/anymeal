#include "category_dialog.hh"


using namespace std;

CategoryDialog::CategoryDialog(QWidget *parent):
  QDialog(parent), m_categories_completer(NULL)
{
  m_ui.setupUi(this);
}

void CategoryDialog::set_categories_model(CategoriesModel *categories_model) {
  if (m_categories_completer) {
    delete m_categories_completer;
    m_categories_completer = NULL;
  };
  m_categories_completer = new QCompleter(categories_model, this);
  m_ui.category_edit->setCompleter(m_categories_completer);
}

string CategoryDialog::category(void) {
  return m_ui.category_edit->text().toUtf8().constData();
}
