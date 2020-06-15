#include <sstream>
#include "edit_dialog.hh"


using namespace std;

EditDialog::EditDialog(QWidget *parent):
  QDialog(parent)
{
  m_ui.setupUi(this);
}

void EditDialog::set_recipe(Recipe &recipe) {
  m_ui.title_edit->setText(recipe.title().c_str());
  ostringstream categories;
  for (unsigned int i=0; i<recipe.categories().size(); i++) {
    categories << recipe.categories()[i];
    if (i < recipe.categories().size() - 1)
      categories << ", ";
  };
  m_ui.categories_edit->setText(categories.str().c_str());
  m_ui.servings_spin->setValue(recipe.servings());
  m_ui.servings_unit_edit->setText(recipe.servings_unit().c_str());
}
