/* AnyMeal recipe database software
   Copyright (C) 2020 Jan Wedekind

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>. */
#include <sstream>
#include "html.hh"
#include "edit_dialog.hh"


using namespace std;

EditDialog::EditDialog(QWidget *parent):
  QDialog(parent), m_ingredient_model(nullptr)
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
  if (m_ingredient_model) {
    m_ui.ingredients_view->setModel(nullptr);
    delete m_ingredient_model;
    m_ingredient_model = nullptr;
  };
  m_ingredient_model = new IngredientModel(this, recipe.ingredients(), recipe.ingredient_sections());
  m_ui.ingredients_view->setModel(m_ingredient_model);
  m_ui.ingredients_view->expandAll();
  connect(m_ui.ingredients_view->selectionModel(), &QItemSelectionModel::currentChanged, this, &EditDialog::select_ingredient);
}

void EditDialog::select_ingredient(const QModelIndex &current, const QModelIndex &) {
  if (m_ingredient_model->is_ingredient(current)) {
    Ingredient ingredient = m_ingredient_model->get_ingredient(current);
    if (ingredient.amount_float() > 0) {
      m_ui.amount_type_combo->setCurrentIndex(1);
      m_ui.amount_spin->setValue(ingredient.amount_float());
    } else {
      m_ui.amount_type_combo->setCurrentIndex(0);
      m_ui.integer_spin->setValue(ingredient.amount_integer());
      m_ui.numerator_spin->setValue(ingredient.amount_numerator());
      m_ui.denominator_spin->setValue(ingredient.amount_denominator());
    };
    m_ui.unit_combo->setCurrentIndex(index_of_unit(ingredient.unit()));
    m_ui.name_edit->setText(ingredient.text().c_str());
  };
}
