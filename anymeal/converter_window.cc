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
  for (int i=0; i<UNITS; i++) {
    string unit = html_unit(unit_from_index(i), &translate);
    m_ui.source_unit_combo->addItem(unit.c_str());
    m_ui.dest_unit_combo->addItem(unit.c_str());
  };
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
