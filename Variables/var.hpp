#pragma once

#include <QString>
#include <fstream>
#include <string>
#include <array>
#include <QMessageBox>


struct var
{
	QString name;
	QString path_to_file;
	QString data;
	var* address_to_another_var = nullptr;
	unsigned int x = 0;
	unsigned int equal_from_function;
	QString function_var;

	friend bool operator!(const var &c);
	QString operator()(char parameter) const;
	explicit var(const QString& path);
	~var();
};


var* add_parent_var(const QString& path);
int convert_formula_to_expression(const std::string& formula);