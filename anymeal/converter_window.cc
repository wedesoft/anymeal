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
#include "converter_window.hh"
#include "html.hh"


using namespace std;

ConverterWindow::ConverterWindow(QWidget *parent):
  QWidget(parent)
{
  m_ui.setupUi(this);
  connect(m_ui.source_amount_spin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &ConverterWindow::update_value);
  connect(m_ui.source_unit_combo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ConverterWindow::update_value);
  connect(m_ui.dest_unit_combo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ConverterWindow::update_value);
  connect(m_ui.preset_combo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ConverterWindow::change_ingredient);
  for (int i=0; i<UNITS; i++) {
    string unit = html_unit(unit_from_index(i), &translate);
    m_ui.source_unit_combo->addItem(unit.c_str());
    m_ui.dest_unit_combo->addItem(unit.c_str());
  };
  for (int i=0; i<INGREDIENTS; i++) {
    m_ui.preset_combo->addItem(ingredient(i));
  };
  m_ui.preset_combo->setCurrentIndex(INGREDIENTS - 1);
}

string ConverterWindow::translate(const char *context, const char *text) {
  return QCoreApplication::translate(context, text).toUtf8().constData();
}

double ConverterWindow::conversion_factor(int unit_index) {
  switch (unit_index) {
    case  6: return (1 / 16.0) * 4.928921595e-3; // pinch
    case  7: return (1 / 64.0) * 4.928921595e-3; // drop
    case  8: return (1 /  8.0) * 4.928921595e-3; // dash
    case 13: return  4.928921595e-3; // teaspoon
    case 14: return 14.7867647825e-3; // tablespoon
    case 15: return 28.4130625e-3; // fluid ounce
    case 16: return 236.5882365e-3; // cup
    case 17: return 473.176473e-3; // pint
    case 18: return 0.946352946; // quart
    case 19: return 3.785411784; // gallon
    case 20: return 28.34952312e-3; // ounce
    case 21: return 453.59237e-3; // pound
    case 22: return 0.001; // milliliter
    case 23: return 0.001; // cubic centimeter
    case 24: return 0.01; // centiliter
    case 25: return 0.1; // deciliter
    case 26: return 1.0; // liter
    case 27: return 1e-6; // milligram
    case 28: return 1e-5; // centigram
    case 29: return 1e-4; // decigram
    case 30: return 0.001; // gram
    case 31: return 1.0; // kilogram
    default: return -1;
  };
}

QString ConverterWindow::ingredient(int index) {
  switch (index) {
    case  0: return tr("rolled oats");
    case  1: return tr("bread crumbs");
    case  2: return tr("desiccated coconut");
    case  3: return tr("cornflour");
    case  4: return tr("cocoa");
    case  5: return tr("ground almonds");
    case  6: return tr("shredded suet");
    case  7: return tr("sugar");
    case  8: return tr("rice");
    case  9: return tr("haricots");
    case 10: return tr("lentils");
    case 11: return tr("barley");
    case 12: return tr("peas");
    case 13: return tr("sultana");
    case 14: return tr("tapioca");
    case 15: return tr("ground rice");
    case 16: return tr("currants");
    case 17: return tr("semolina");
    case 18: return tr("raisins");
    case 19: return tr("custard powder");
    case 20: return tr("flour");
    case 21: return tr("milk");
    case 22: return tr("icing sugar");
    default: return tr("Custom");
  };
}

double ConverterWindow::density(int index) {
  switch (index) {
    case  0: return 0.35; // rolled oats
    case  1: return 0.51; // bread crumbs
    case  2: return 0.352; // desiccated coconut
    case  3: return 0.49; // cornflour
    case  4: return 0.641; // cocoa
    case  5: return 0.59; // ground almonds
    case  6: return 0.59; // shredded suet
    case  7: return 0.849; // sugar
    case  8: return 0.78; // rice
    case  9: return 0.81; // haricots
    case 10: return 0.81; // lentils
    case 11: return 0.81; // barley
    case 12: return 0.81; // peas
    case 13: return 0.613; // sultana
    case 14: return 0.613; // tapioca
    case 15: return 0.568; // ground rice
    case 16: return 0.47; // currants
    case 17: return 0.706; // semolina
    case 18: return 0.61; // raisins
    case 19: return 0.81; // custard powder
    case 20: return 0.593; // flour
    case 21: return 1.03; // milk
    case 22: return 0.801; // icing sugar
    default: return 1.0; // Custom
  };
}

void ConverterWindow::update_value(void) {
  double value = m_ui.source_amount_spin->value();
  double factor = conversion_factor(m_ui.source_unit_combo->currentIndex());
  double divisor = conversion_factor(m_ui.dest_unit_combo->currentIndex());
  QString text;
  if (factor > 0 && divisor > 0) {
    value *= factor / divisor;
    text = QString("%1").arg(value);
  } else
    text = "?";
  m_ui.dest_amount_edit->setText(text);
}

void ConverterWindow::change_ingredient(int index) {
  m_ui.density_spin->setEnabled(index == CUSTOM);
  m_ui.density_spin->setValue(density(index));
}
