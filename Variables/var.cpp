#include <var.hpp>
#include <exprtk.hpp>

var* add_parent_var(const QString& path)
{
	if (path == "NULL")
		return nullptr;

	try
	{
		auto* new_var = new var(path);
		return new_var;
	}
	catch (const std::exception& e)
	{
		QMessageBox::information(new QWidget, "Warning", e.what());
	}
}

int convert_formula_to_expression(const std::string& formula)
{
	if (formula == "NULL")
		return 0;

	exprtk::symbol_table<double> symbol_table;
	exprtk::expression<double> expression;
	expression.register_symbol_table(symbol_table);
	exprtk::parser<double> parser;
	if (parser.compile(formula, expression))
	{
		const int number = static_cast<int>(expression.value());
		return number < 0 ? number : throw std::exception("the result from formula is above 0");
	}
	else
	{
		throw std::exception("expression is wrong");
	}
}


inline var::var(const QString& path)
{
	std::fstream input_file(path.toStdString());

	constexpr std::array<size_t, 4> PARSE_POS{ 6, 7, 10, 13 }; // места, откуда нужно парсить данные
	const std::array<std::string, 4> PARAMETER_VAR_NECESSARILY{ "name: ", "value: ", "function: ", "parent var: " };
	std::array<QString, 4> value_to_parse;

	if (input_file.is_open())
	{
		for (size_t i = 0; i < PARSE_POS.size(); ++i)
		{
			std::string temp;
			std::getline(input_file, temp);

			if (temp.substr(0, PARSE_POS[i]) != PARAMETER_VAR_NECESSARILY[i])
			{
				throw std::exception("The template is written incorrectly");
			}

			if (temp.size() > PARSE_POS[i])
			{
				value_to_parse[i] = QString::fromStdString(temp.substr(PARSE_POS[i]));
			}
			else
			{
				value_to_parse[i] = "NULL";
			}
		}

		name = value_to_parse[0];
		data = value_to_parse[1];
		function_var = value_to_parse[2];
		address_to_another_var = add_parent_var(value_to_parse[3]);
		path_to_file = path;
		x = 0;
		equal_from_function = convert_formula_to_expression(function_var.toStdString());
	}
	else
	{
		throw std::exception("File not found");
	}
}

bool operator!(const var& c)
{
	return (c.name.isEmpty() && c.path_to_file.isEmpty() && c.data.isEmpty());
}

QString var::operator()(char parameter) const
{
	const QString result_parent_var = address_to_another_var ? address_to_another_var->path_to_file : "NULL";
	switch (parameter)
	{
	case 'a': // all parameters
		return "name: " + name + " path: " + path_to_file + " data: " + data + " function: " + function_var + " parent var: " + result_parent_var + " x: " + QString::number(x) + "\n";
	case 'b': // only base (name, path_to_file, data, parent_var)
		return "name: " + name + " path: " + path_to_file + " data: " + data +  " parent_var: " + result_parent_var + "\n";
	}
}

var::~var()
{
	delete address_to_another_var;
}
