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
#include "instructions_model.hh"


using namespace std;
InstructionsModel::InstructionsModel(QObject *parent, vector<string> &instructions, vector<pair<int, string>> &sections):
  QAbstractListModel(parent), m_instructions(instructions), m_sections(sections)
{
}

int InstructionsModel::rowCount(const QModelIndex &) const {
  return m_sections.size() + 1;
}

QVariant InstructionsModel::data(const QModelIndex &index, int role) const {
  QVariant result;
  if (role == Qt::DisplayRole || role == Qt::EditRole) {
    int row = index.row();
    if (row > 0)
      result = QString(m_sections[row - 1].second.c_str());
    else
      result = tr("<Main>");
  };
  return result;
}

std::string InstructionsModel::get_section(const QModelIndex &index) const {
  int row = index.row();
  if (row > 0)
    return m_sections[row - 1].second;
  else
    return tr("<Main>").toUtf8().constData();
}

void InstructionsModel::set_section(const QModelIndex &index, const char *text) {
  int row = index.row();
  if (row > 0) {
    m_sections[row - 1].second = text;
    emit dataChanged(index, index);
  };
}

QModelIndex InstructionsModel::add_section(const QModelIndex &idx) {
  if (!idx.isValid())
    return idx;
  int row = idx.row();
  int offset;
  if (row < (signed)m_sections.size()) {
    offset = m_sections[row].first;
  } else {
    offset = m_instructions.size();
  };
  beginInsertRows(QModelIndex(), row + 1, row + 1);
  m_sections.insert(m_sections.begin() + row, pair<int, string>(offset, tr("section").toUtf8().constData()));
  endInsertRows();
  return index(row + 1, 0, QModelIndex());
}

QModelIndex InstructionsModel::remove_section(const QModelIndex &idx) {
  if (!idx.isValid())
    return idx;
  int row = idx.row();
  if (row == 0)
    return idx;
  int a = m_sections[row - 1].first;
  int b = row < (signed)m_sections.size() ? m_sections[row].first : m_instructions.size();
  beginRemoveRows(QModelIndex(), row, row);
  m_sections.erase(m_sections.begin() + row - 1);
  for (int i=row - 1; i<(signed)m_sections.size(); i++) {
    m_sections[i].first -= b - a;
  };
  m_instructions.erase(m_instructions.begin() + a, m_instructions.begin() + b);
  endRemoveRows();
  return index(row - 1, 0, QModelIndex());
}
