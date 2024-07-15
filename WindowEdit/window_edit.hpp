#pragma once

#include <QtWidgets>
#include <var.hpp>

class WindowEdit : public QWidget
{
private:
	QLineEdit* txt_line_name;
	QLineEdit* txt_line_address;
	QLineEdit* txt_line_function;
	QLineEdit* txt_line_x;
	QLabel* lbl_name;
	QLabel* lbl_address;
	QLabel* lbl_function;
	QLabel* lbl_x;
	QPushButton* btn_save;
	QPushButton* btn_close;
	QPushButton* btn_find_dir;
	QString file_dir_find;

	var& var_to_change;
	QListWidgetItem* item_to_save;
public:
	WindowEdit(var& variable, QListWidgetItem* item);
	~WindowEdit();
	void init_elements();
	void connect_element();
	void find_dir();
	void save_var();
	var* check_var(const QString& path, var* previous_var);
};