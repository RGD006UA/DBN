#include "window_export.hpp"
#include <mutex>
#include <algorithm>
#include <chrono>
#include <fstream>

// pair_it = <int, list<var>::iterator>. Для того, чтобы можно было в одном цикле проходить сразу 2 обьекта


std::mutex clear_var;

bool WindowExport::is_stoped = false;

bool interval(const int start, const int end, const int number)
{
	return start <= number && number < end;
}

// отнимание с формулы переменной

void make_operation_with_function(std::list<var>& vars, const std::vector<QLabel*>& lbls)
{
	for (auto pair_it = std::make_pair(0, begin(vars)); 
		 pair_it.first < lbls.size(); 
		 pair_it.first++, ++pair_it.second)
	{
		if (pair_it.second->equal_from_function > 0 && !lbls[pair_it.first]->text().isEmpty())
		{
			pair_it.second->x += pair_it.second->equal_from_function;
			lbls[pair_it.first]->setText(pair_it.second->operator()('a'));
		}
	}
}

// удаление перемен, у которых х < 0

void delete_var(std::list<var>& vars, const std::vector<QLabel*>& lbls)
{
	for (auto pair_it = std::make_pair(0, begin(vars));
		 pair_it.first < lbls.size();
		 pair_it.first++, ++pair_it.second)
	{
		if (pair_it.second->x <= 0)
		{
			vars.erase(pair_it.second);
			lbls[pair_it.first]->clear();
			lbls[pair_it.first]->setStyleSheet("QLabel {background-color: none;}");
		}
	}
}

void color_vars(const std::list<var>& vars, const std::vector<QLabel*>& lbls)
{
	for (auto pair_it = std::make_pair(0, begin(vars));
		 pair_it.first < lbls.size();
		 pair_it.first++, ++pair_it.second)
	{
		if (interval(600'000, 1'000'000, pair_it.second->x))
			lbls[pair_it.first]->setStyleSheet("QLabel {background-color: green; }");
		else if (interval(400'000, 600'000, pair_it.second->x))
			lbls[pair_it.first]->setStyleSheet("QLabel {background-color: yellow; }");
		else if (interval(0, 400'000, pair_it.second->x))
			lbls[pair_it.first]->setStyleSheet("QLabel {background-color: red; }");
		else if (pair_it.second->x <= 0)
			lbls[pair_it.first]->setStyleSheet("QLabel {background-color: none;}");
	}
}

void WindowExport::function_action_async(std::list<var>& vars, const std::vector<QLabel*>& lbls, const int t)
{
	is_stoped = false;

	while (!is_stoped)
	{
		make_operation_with_function(vars, lbls);
		color_vars(vars, lbls);
		delete_var(vars, lbls);
		std::this_thread::sleep_for(std::chrono::milliseconds(t));
	}
}

WindowExport::WindowExport(const std::list<var>& vars_)
	: vars(vars_)
{
	init_elements();

	add_function_to_var();

	auto* vbox_main = new QVBoxLayout;

	vbox_main->addWidget(lbl_parameters);

	for (const var& variable : vars)
	{
		lbl_vars.push_back(new QLabel(variable('a')));
		vbox_main->addWidget(lbl_vars.back());
	}

	auto* hbox_btn = new QHBoxLayout;
	hbox_btn->addWidget(btn_start);
	hbox_btn->addWidget(btn_stop);
	hbox_btn->addWidget(btn_save);

	vbox_main->addLayout(hbox_btn);
	vbox_main->addWidget(txt_t);
	vbox_main->addWidget(txt_q);
	vbox_main->addWidget(btn_save_parameters);
	//vbox_main->addWidget(btn_debug);

	connect_element();

	setLayout(vbox_main);
}

void WindowExport::init_elements()
{
	btn_start = new QPushButton("start");
	btn_save = new QPushButton("save");
	btn_stop = new QPushButton("stop");
	btn_save_parameters = new QPushButton("save parameters");
	txt_t = new QLineEdit;
	txt_t->setPlaceholderText("t: milliseconds");
	txt_q = new QLineEdit;
	txt_q->setPlaceholderText("q: milliseconds");
	lbl_parameters = new QLabel("parameters q:" + QString::number(q) + " t: " + QString::number(t));
	//btn_debug = new QPushButton("debug");
}

void WindowExport::connect_element()
{
	connect(btn_start, &QPushButton::clicked, this, [this]() { start(is_btn_start_pressed); });
	connect(btn_stop, &QPushButton::clicked, this, [this]() { stop(is_btn_start_pressed); });
	connect(btn_save, &QPushButton::clicked, this, [this]() { save(); });
	connect(btn_save_parameters, &QPushButton::clicked, this, [this]() { save_parameters(); });
	/*connect(btn_debug, &QPushButton::clicked, this, [this]()
		{
			QString res;
			for (const var el : vars)
			{
				res += el('a') + "\n";
			}
			QMessageBox::information(this, "Debug", res);
		});*/
}

void WindowExport::save_parameters()
{
	bool success;

	QString str_t = txt_t->text();
	QString str_q = txt_q->text();

	if (str_t.isEmpty())
		str_t = "0";

	if (str_q.isEmpty())
		str_q = "0";

	t = str_t.toInt(&success);
	q = str_q.toInt(&success);

	if (!success)
	{
		QMessageBox::critical(this, "Error", "Something went wrong with t and q");
		t = q = -1;
		return;
	}

	lbl_parameters->setText("parameters q: " + QString::number(q) + " t: " + QString::number(t));
}


void WindowExport::start(bool& flag)
{
	if (flag) return;

	if (t > 0)
	{
		operations = std::move(std::thread(function_action_async, std::ref(vars), std::ref(lbl_vars), t));
		operations.detach();
	}

	if (q > 0)
	{
		delete_all = std::move(std::thread(delete_all_async, std::ref(vars), std::ref(lbl_vars), q));
		delete_all.detach();

	}

	flag = true;
}

void WindowExport::delete_all_async(std::list<var>& vars, const std::vector<QLabel*>& lbls, const int q)
{
	if (q == 0)
		return;

	std::this_thread::sleep_for(std::chrono::milliseconds(q));
	std::lock_guard<std::mutex> delete_all(clear_var);

	vars.clear();
	for (auto* lbl : lbls)
	{
		lbl->clear();
		lbl->setStyleSheet("QLabel {background-color: none;}");
	}

	is_stoped = true;
}


void WindowExport::stop(bool& flag)
{
	is_stoped = true;
	flag = false;
}

void WindowExport::save()
{
	const auto now = std::chrono::system_clock::now();
	const std::string timestamp = std::to_string(std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count());

	std::ofstream file_to_save("DB/" + timestamp + ".txt");

	for (const auto& var : vars)
		file_to_save << var('a').toStdString() << std::endl;
}

void WindowExport::add_function_to_var()
{
	for (auto& var : vars)
	{
		if (var.address_to_another_var && var.function_var == "NULL")
		{
			var.function_var = var.address_to_another_var->function_var;
			var.equal_from_function = convert_formula_to_expression(var.function_var.toStdString());
		}
	}
}


WindowExport::~WindowExport()
{
	delete btn_start;
	delete btn_save;
	delete btn_stop;
	delete btn_save_parameters;
	delete txt_t;
	delete txt_q;
	for (const auto* lbl : lbl_vars)
		delete lbl;
}
