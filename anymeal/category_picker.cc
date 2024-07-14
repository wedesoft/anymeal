/* AnyMeal recipe management software
   Copyright (C) 2020, 2024 Jan Wedekind

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
#include <algorithm>
#include <QtWidgets/QMessageBox>
#include "category_picker.hh"
#include "rename_dialog.hh"

using namespace std;

CategoryPicker::CategoryPicker(QWidget *parent):
  QDialog(parent), m_sort_filter_proxy_model(this), m_model(NULL)
{
  m_ui.setupUi(this);
  connect(m_ui.delete_button, &QPushButton::clicked, this, &CategoryPicker::delete_categories);
  connect(m_ui.rename_button, &QPushButton::clicked, this, &CategoryPicker::rename_category);
}

void CategoryPicker::set_model(CategoryTableModel *model) {
  m_model = model;
  m_sort_filter_proxy_model.setSourceModel(model);
  m_ui.category_table->setModel(&m_sort_filter_proxy_model);
}

void CategoryPicker::delete_categories(void) {
  if (QMessageBox::question(this, tr("Delete Categories"), tr("Do you want to delete the selected categories?")) == QMessageBox::Yes) {
    QItemSelectionModel *selection_model = m_ui.category_table->selectionModel();
    QModelIndexList index_list = selection_model->selectedRows();
    QModelIndexList mapped_list = QModelIndexList();
    for (QModelIndexList::iterator i=index_list.begin(); i!=index_list.end(); i++) {
      QModelIndex source_index = m_sort_filter_proxy_model.mapToSource(*i);
      mapped_list.push_back(source_index);
    };
    // Sort mapped indices.
    sort(mapped_list.begin(), mapped_list.end(), less<QModelIndex>());
    // Delete bottom rows first so that remaining indices remain correct.
    for (QModelIndexList::reverse_iterator i=mapped_list.rbegin(); i!=mapped_list.rend(); i++) {
      m_model->delete_category((*i).row());
    };
  };
}

void CategoryPicker::rename_category(void) {
  RenameDialog rename_dialog(this);
  rename_dialog.set_model(m_model);
  QModelIndex index = m_ui.category_table->currentIndex();
  QModelIndex source_index = m_sort_filter_proxy_model.mapToSource(index);
  rename_dialog.set_name(m_model->category(source_index.row()));
  if (rename_dialog.exec() == QDialog::Accepted) {
    m_model->rename_category(source_index.row(), rename_dialog.name());
  };
}
