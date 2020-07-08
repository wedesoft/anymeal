/* AnyMeal recipe management software
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
#include <cassert>
#include <sstream>
#include "html.hh"
#include "edit_dialog.hh"


using namespace std;

EditDialog::EditDialog(QWidget *parent):
  QDialog(parent), m_ingredient_model(nullptr), m_instructions_model(nullptr), m_title_validator(nullptr)
{
  m_ui.setupUi(this);
  connect(m_ui.title_edit, &QLineEdit::textChanged, this, &EditDialog::update_ok_button);
  connect(m_ui.categories_edit, &QLineEdit::textChanged, this, &EditDialog::update_ok_button);
  connect(m_ui.servings_unit_edit, &QLineEdit::textChanged, this, &EditDialog::update_ok_button);
  connect(m_ui.amount_type_combo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &EditDialog::amount_type_changed);
  connect(m_ui.integer_spin, QOverload<int>::of(&QSpinBox::valueChanged), this, &EditDialog::amount_int_changed);
  connect(m_ui.numerator_spin, QOverload<int>::of(&QSpinBox::valueChanged), this, &EditDialog::amount_int_changed);
  connect(m_ui.denominator_spin, QOverload<int>::of(&QSpinBox::valueChanged), this, &EditDialog::amount_int_changed);
  connect(m_ui.amount_spin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &EditDialog::amount_float_changed);
  connect(m_ui.unit_combo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &EditDialog::unit_changed);
  connect(m_ui.name_edit, &QLineEdit::textChanged, this, &EditDialog::ingredient_name_changed);
  connect(m_ui.ingredient_button, &QPushButton::clicked, this, &EditDialog::add_ingredient);
  connect(m_ui.remove_ingredient_button, &QPushButton::clicked, this, &EditDialog::delete_ingredient);
  connect(m_ui.ingredient_section_edit, &QLineEdit::textChanged, this, &EditDialog::ingredient_section_changed);
  connect(m_ui.ingredient_group_button, &QPushButton::clicked, this, &EditDialog::add_ingredient_section);
  connect(m_ui.ingredient_up_button, &QPushButton::clicked, this, &EditDialog::move_ingredient_up);
  connect(m_ui.ingredient_down_button, &QPushButton::clicked, this, &EditDialog::move_ingredient_down);
  connect(m_ui.instruction_section_edit, &QLineEdit::textChanged, this, &EditDialog::section_changed);
  connect(m_ui.instructions_group_button, &QPushButton::clicked, this, &EditDialog::add_instruction_section);
  connect(m_ui.remove_instruction_button, &QPushButton::clicked, this, &EditDialog::remove_instruction_section);
  connect(m_ui.instructions_edit, &QPlainTextEdit::textChanged, this, &EditDialog::instructions_text_changed);
  m_title_validator = new QRegExpValidator(QRegExp("\\S.*"), this);
  m_ui.title_edit->setValidator(m_title_validator);
  m_categories_validator = new QRegExpValidator(QRegExp("\\S.*"), this);
  m_ui.categories_edit->setValidator(m_categories_validator);
  m_servings_unit_validator = new QRegExpValidator(QRegExp("\\S.*"), this);
  m_ui.servings_unit_edit->setValidator(m_servings_unit_validator);
}

void EditDialog::set_recipe(Recipe &recipe) {
  // Set title fields.
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
  // Create ingredient model.
  if (m_ingredient_model) {
    m_ui.ingredients_view->setModel(nullptr);
    delete m_ingredient_model;
    m_ingredient_model = nullptr;
  };
  m_ingredient_model = new IngredientModel(this, recipe.ingredients(), recipe.ingredient_sections());
  m_ui.ingredients_view->setModel(m_ingredient_model);
  m_ui.ingredients_view->expandAll();
  connect(m_ui.ingredients_view->selectionModel(), &QItemSelectionModel::currentChanged, this, &EditDialog::select_ingredient);
  // Create instructions model.
  if (m_instructions_model) {
    m_ui.instructions_view->setModel(nullptr);
    delete m_instructions_model;
    m_instructions_model = nullptr;
  };
  m_instructions_model = new InstructionsModel(this, recipe.instructions(), recipe.instruction_sections());
  m_ui.instructions_view->setModel(m_instructions_model);
  connect(m_ui.instructions_view->selectionModel(), &QItemSelectionModel::currentChanged, this, &EditDialog::select_instruction);
}

Recipe EditDialog::get_recipe(void) {
  Recipe result;
  // Get title fields.
  result.set_title(m_ui.title_edit->text().toUtf8().constData());
  string categories = m_ui.categories_edit->text().toUtf8().constData();
  size_t pos;
  while ((pos = categories.find(',')) != string::npos) {
    result.add_category(categories.substr(0, pos).c_str());
    pos++;
    while (pos < categories.length() && categories[pos] == ' ')
      pos++;
    categories = categories.substr(pos, categories.length() - pos);
  };
  result.add_category(categories.c_str());
  result.set_servings(m_ui.servings_spin->value());
  result.set_servings_unit(m_ui.servings_unit_edit->text().toUtf8().constData());
  // Get ingredients.
  assert(m_ingredient_model);
  result.set_ingredients(m_ingredient_model->get_ingredients());
  result.set_ingredient_sections(m_ingredient_model->get_sections());
  // Get instructions.
  assert(m_instructions_model);
  result.set_instructions(m_instructions_model->get_instructions());
  result.set_instruction_sections(m_instructions_model->get_sections());
  return result;
}

void EditDialog::select_ingredient(const QModelIndex &current, const QModelIndex &) {
  if (!current.isValid())
    return;
  if (m_ingredient_model->is_ingredient(current)) {
    m_ui.ingredient_stack->setCurrentIndex(0);
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
    m_ui.name_edit->setEnabled(true);
  } else {
    m_ui.ingredient_stack->setCurrentIndex(1);
    m_ui.ingredient_section_edit->setText(m_ingredient_model->get_ingredient_section(current).c_str());
  };
}

void EditDialog::ingredient_name_changed(const QString &text) {
  QModelIndex index = m_ui.ingredients_view->currentIndex();
  if (m_ingredient_model->is_ingredient(index)) {
    Ingredient ingredient = m_ingredient_model->get_ingredient(index);
    ingredient.set_text(text.toUtf8().constData());
    m_ingredient_model->set_ingredient(index, ingredient);
    update_ok_button();
  };
}

void EditDialog::unit_changed(int idx) {
  QModelIndex index = m_ui.ingredients_view->currentIndex();
  if (m_ingredient_model->is_ingredient(index)) {
    Ingredient ingredient = m_ingredient_model->get_ingredient(index);
    ingredient.set_unit(unit_from_index(idx));
    m_ingredient_model->set_ingredient(index, ingredient);
  };
}

void EditDialog::amount_type_changed(int value) {
  QModelIndex index = m_ui.ingredients_view->currentIndex();
  if (m_ingredient_model->is_ingredient(index)) {
    if (value == 0) {
      m_ui.amount_spin->setValue(0.0);
      m_ui.integer_spin->setValue(1);
      m_ui.numerator_spin->setValue(0);
      m_ui.denominator_spin->setValue(1);
    } else {
      m_ui.integer_spin->setValue(0);
      m_ui.numerator_spin->setValue(0);
      m_ui.denominator_spin->setValue(1);
      m_ui.amount_spin->setValue(1.0);
    };
  };
}

int EditDialog::fraction_str_length(void) {
  ostringstream s;
  if (m_ui.integer_spin->value() > 0)
    s << m_ui.integer_spin->value();
  if (m_ui.numerator_spin->value() > 0) {
    if (m_ui.integer_spin->value() > 0)
      s << ' ';
    s << m_ui.numerator_spin->value() << '/' << m_ui.denominator_spin->value();
  };
  return s.str().length();
}

void EditDialog::amount_int_changed(int) {
  QModelIndex index = m_ui.ingredients_view->currentIndex();
  if (m_ingredient_model->is_ingredient(index)) {
    if (fraction_str_length() <= 7) {
      Ingredient ingredient = m_ingredient_model->get_ingredient(index);
      ingredient.set_amount_float(0.0);
      ingredient.set_amount_integer(m_ui.integer_spin->value());
      ingredient.set_amount_numerator(m_ui.numerator_spin->value());
      ingredient.set_amount_denominator(m_ui.denominator_spin->value());
      m_ingredient_model->set_ingredient(index, ingredient);
    } else {
      double value = m_ui.integer_spin->value() + m_ui.numerator_spin->value() / (double)m_ui.denominator_spin->value();
      amount_float_changed(value);
    };
  };
}

void EditDialog::amount_float_changed(double value) {
  QModelIndex index = m_ui.ingredients_view->currentIndex();
  if (m_ingredient_model->is_ingredient(index)) {
    Ingredient ingredient = m_ingredient_model->get_ingredient(index);
    ingredient.set_amount_float(value);
    ingredient.set_amount_integer(0);
    ingredient.set_amount_numerator(0);
    ingredient.set_amount_denominator(1);
    m_ingredient_model->set_ingredient(index, ingredient);
  };
}

void EditDialog::add_ingredient(void) {
  QModelIndex index = m_ui.ingredients_view->currentIndex();
  Ingredient ingredient;
  ingredient.set_text(tr("ingredient").toUtf8().constData());
  QModelIndex result = m_ingredient_model->add_ingredient(index, ingredient);
  m_ui.ingredients_view->setCurrentIndex(result);
  m_ui.name_edit->setFocus(Qt::OtherFocusReason);
  update_ok_button();
}

void EditDialog::delete_ingredient(void) {
  QModelIndex index = m_ui.ingredients_view->currentIndex();
  QModelIndex result = m_ingredient_model->delete_ingredient(index);
  m_ui.ingredients_view->setCurrentIndex(result);
  update_ok_button();
}

void EditDialog::ingredient_section_changed(const QString &text) {
  QModelIndex index = m_ui.ingredients_view->currentIndex();
  m_ingredient_model->set_ingredient_section(index, text.toUtf8().constData());
  update_ok_button();
}

void EditDialog::add_ingredient_section(void) {
  QModelIndex index = m_ui.ingredients_view->currentIndex();
  QModelIndex result = m_ingredient_model->add_ingredient_section(index, tr("Title").toUtf8().constData());
  m_ui.ingredients_view->setCurrentIndex(result);
  m_ui.ingredient_section_edit->setFocus(Qt::OtherFocusReason);
  update_ok_button();
}

void EditDialog::move_ingredient_up(void) {
  QModelIndex index = m_ui.ingredients_view->currentIndex();
  QModelIndex result = m_ingredient_model->move_up(index);
  m_ui.ingredients_view->setCurrentIndex(result);
  update_ok_button();
}

void EditDialog::move_ingredient_down(void) {
  QModelIndex index = m_ui.ingredients_view->currentIndex();
  QModelIndex result = m_ingredient_model->move_down(index);
  m_ui.ingredients_view->setCurrentIndex(result);
  update_ok_button();
}

void EditDialog::select_instruction(const QModelIndex &current, const QModelIndex &) {
  if (!current.isValid())
    return;
  m_ui.instruction_section_edit->setText(m_instructions_model->get_section(current).c_str());
  m_ui.instructions_edit->setPlainText(m_instructions_model->get_text(current).c_str());
  m_ui.instructions_edit->setEnabled(true);
}

void EditDialog::section_changed(const QString &text) {
  QModelIndex index = m_ui.instructions_view->currentIndex();
  m_instructions_model->set_section(index, text.toUtf8().constData());
  update_ok_button();
}

void EditDialog::add_instruction_section(void) {
  QModelIndex index = m_ui.instructions_view->currentIndex();
  QModelIndex result = m_instructions_model->add_section(index);
  m_ui.instructions_view->setCurrentIndex(result);
}

void EditDialog::remove_instruction_section(void) {
  QModelIndex index = m_ui.instructions_view->currentIndex();
  QModelIndex result = m_instructions_model->remove_section(index);
  m_ui.instructions_view->setCurrentIndex(result);
}

void EditDialog::instructions_text_changed(void) {
  QModelIndex index = m_ui.instructions_view->currentIndex();
  m_instructions_model->set_text(index, m_ui.instructions_edit->toPlainText().toUtf8().constData());
}

void EditDialog::update_ok_button(void) {
  if (!m_ingredient_model)
    return;
  if (!m_instructions_model)
    return;
  bool enable = m_ui.title_edit->hasAcceptableInput() &&
                m_ui.categories_edit->hasAcceptableInput() &&
                m_ui.servings_unit_edit->hasAcceptableInput() &&
                m_ingredient_model->has_acceptable_input() &&
                m_instructions_model->has_acceptable_input();
  m_ui.ok_button->setEnabled(enable);
}
