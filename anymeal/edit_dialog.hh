#pragma once
#include <QtWidgets/QDialog>
#include "recipe.hh"
#include "ingredient_model.hh"
#include "ui_edit_dialog.hh"


class EditDialog: public QDialog
{
  Q_OBJECT
public:
  EditDialog(QWidget *parent=nullptr);
  void set_recipe(Recipe &recipe);
protected:
  Ui::EditDialog m_ui;
  IngredientModel *m_ingredient_model;
};
