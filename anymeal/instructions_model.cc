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
#include "instructions_model.hh"


using namespace std;

InstructionsModel::InstructionsModel(QObject *parent, vector<string> &instructions, vector<pair<int, string>> &sections):
  QAbstractListModel(parent)
{
  for (int i=0; i<=(signed)sections.size(); i++) {
    if (i > 0) {
      m_sections.push_back(sections[i - 1].second);
    };
    int a = i == 0 ? 0 : sections[i - 1].first;
    int b = i < (signed)sections.size() ? sections[i].first : instructions.size();
    ostringstream s;
    for (int j=a; j<b; j++) {
      s << instructions[j];
      if (j < b - 1)
        s << endl;
    };
    m_instructions.push_back(s.str());
  };
}

int InstructionsModel::rowCount(const QModelIndex &) const {
  return m_sections.size() + 1;
}

QVariant InstructionsModel::data(const QModelIndex &index, int role) const {
  QVariant result;
  if (role == Qt::DisplayRole || role == Qt::EditRole) {
    int row = index.row();
    if (row > 0)
      result = QString(m_sections[row - 1].c_str());
    else
      result = tr("<Main>");
  };
  return result;
}

std::string InstructionsModel::get_section(const QModelIndex &index) const {
  int row = index.row();
  if (row > 0)
    return m_sections[row - 1];
  else
    return tr("<Main>").toUtf8().constData();
}

void InstructionsModel::set_section(const QModelIndex &index, const char *text) {
  int row = index.row();
  if (row > 0) {
    m_sections[row - 1] = text;
    emit dataChanged(index, index);
  };
}

QModelIndex InstructionsModel::add_section(const QModelIndex &idx) {
  if (!idx.isValid())
    return idx;
  int row = idx.row();
  beginInsertRows(QModelIndex(), row + 1, row + 1);
  m_sections.insert(m_sections.begin() + row, tr("section").toUtf8().constData());
  m_instructions.insert(m_instructions.begin() + row + 1, "");
  endInsertRows();
  return index(row + 1, 0, QModelIndex());
}

QModelIndex InstructionsModel::remove_section(const QModelIndex &idx) {
  if (!idx.isValid())
    return idx;
  int row = idx.row();
  if (row == 0)
    return idx;
  beginRemoveRows(QModelIndex(), row, row);
  m_sections.erase(m_sections.begin() + row - 1);
  m_instructions.erase(m_instructions.begin() + row);
  endRemoveRows();
  return index(row - 1, 0, QModelIndex());
}

string InstructionsModel::get_text(const QModelIndex &index) const {
  if (!index.isValid())
    return "";
  int row = index.row();
  return m_instructions[row];
}

void InstructionsModel::set_text(const QModelIndex &index, const char *text) {
  if (!index.isValid())
    return;
  int row = index.row();
  m_instructions[row] = text;
}
