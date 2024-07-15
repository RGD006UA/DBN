#include "window_edit.hpp"
#include <fstream>
#include <exprtk.hpp>
#include <cstdio>

WindowEdit::WindowEdit(var& variable, QListWidgetItem* item)
	: QWidget(nullptr), var_to_change(variable)
{
	init_elements();

	item_to_save = item;

	resize(400, 400);

	auto* vbox_main = new QVBoxLayout;

	vbox_main->addWidget(lbl_name);
	vbox_main->addWidget(txt_line_name);
	vbox_main->addWidget(lbl_address);
	vbox_main->addWidget(txt_line_address);
	vbox_main->addWidget(btn_find_dir);
	vbox_main->addWidget(lbl_function);
	vbox_main->addWidget(txt_line_function);
	vbox_main->addWidget(lbl_x);
	vbox_main->addWidget(txt_line_x);

	auto* hbox_btn = new QHBoxLayout;
	hbox_btn->addWidget(btn_save);
	hbox_btn->addWidget(btn_close);

	vbox_main->addLayout(hbox_btn);

	connect_element();

	setLayout(vbox_main);
}

void WindowEdit::init_elements()
{
	txt_line_name = new QLineEdit;
	txt_line_address = new QLineEdit;
	txt_line_function = new QLineEdit;
	txt_line_x = new QLineEdit;
	lbl_name = new QLabel("name");
	lbl_address = new QLabel("address");
	lbl_function = new QLabel("function");
	lbl_x = new QLabel("x");
	btn_find_dir = new QPushButton("find file");
	btn_save = new QPushButton("save");
	btn_close = new QPushButton("close");
}

void WindowEdit::connect_element()
{
	connect(btn_find_dir, &QPushButton::clicked, this, [this]() { find_dir(); });
	connect(btn_save, &QPushButton::clicked, this, [this]() { save_var(); });
	connect(btn_close, &QPushButton::clicked, this, &QWidget::close); 
}

void WindowEdit::find_dir()
{
	file_dir_find = QFileDialog::getOpenFileName(this, "open file", QDir::homePath());
	if (check_var(file_dir_find, var_to_change.address_to_another_var) == nullptr)
	{
		QMessageBox::critical(this, "Error", "This is not file from database");
		return;
	}
	txt_line_address->setText(file_dir_find);
}

void WindowEdit::save_var()
{
	const QString name_to_save = txt_line_name->text();
	const QString address_to_save = txt_line_address->text();
	const QString function_to_save = txt_line_function->text();
	const QString x_to_save_text = txt_line_x->text();

	//QMessageBox::information(this, "..", name_to_save + "\n" + address_to_save + "\n" + function_to_save + "\n" + x_to_save_text);

	if (!x_to_save_text.isEmpty())
	{
		//QMessageBox::information(this, "..", "1");
		bool success;
		const int x_to_save = x_to_save_text.toInt(&success);
		if (!success)
		{
			QMessageBox::critical(this, "Error", "Something went wrong with x");
			return;
		}

		var_to_change.x = x_to_save;
	}

	if (!name_to_save.isEmpty())
	{
		//QMessageBox::information(this, "..", "2");
		var_to_change.name = name_to_save;
	}

	
	if (!address_to_save.isEmpty())
	{
		/*QMessageBox::information(this, "..", "3");*/
		try
		{
			var_to_change.address_to_another_var = check_var(address_to_save, var_to_change.address_to_another_var);
		}
		catch(const std::exception& e)
		{
				QMessageBox::critical(this, "Error", e.what());
		}
	}

	if (!function_to_save.isEmpty())
	{
		//QMessageBox::information(this, "..", "4");
		var_to_change.function_var = function_to_save;
		try
		{
			var_to_change.equal_from_function = convert_formula_to_expression(var_to_change.function_var.toStdString());
		}
		catch (const std::exception& e)
		{
			QMessageBox::critical(this, "Error", e.what());
			return;
		}
	}

	item_to_save->setData(Qt::DisplayRole, QVariant(var_to_change('a')));
}

var* WindowEdit::check_var(const QString& path, var* previous_var)
{
	if (path.isEmpty())
		return nullptr;

	try
	{
		auto* tmp = new var(path);
		return tmp;
	}
	catch(const std::exception& e)
	{
		QMessageBox::critical(this, "error", e.what());
		return previous_var;
	}
}

WindowEdit::~WindowEdit()
{
	delete txt_line_name;
	delete txt_line_address;
	delete txt_line_function;
	delete txt_line_x;
	delete lbl_name;
	delete lbl_address;
	delete lbl_function;
	delete lbl_x;
	delete btn_save;
	delete btn_close;
	delete btn_find_dir;
	delete item_to_save;
}
