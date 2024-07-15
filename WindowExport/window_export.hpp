#pragma once

#include <QtWidgets>
#include <var.hpp>
#include <vector>
#include <thread>

class WindowExport : public QWidget
{
private:
	QPushButton* btn_start;
	QPushButton* btn_stop;
	QPushButton* btn_save;
	QPushButton* btn_save_parameters;
	QLineEdit* txt_t;
	QLineEdit* txt_q;
	QLabel* lbl_parameters;

	std::vector<QLabel*> lbl_vars;
	std::list<var> vars;

	std::thread operations;
	std::thread delete_all;
	int t = 0;
	int q = 0;
	static bool is_stoped; // флаг для остановки и возобновления экспорта
	bool is_btn_start_pressed = false; // флаг для проверки нажатия кнопки

	// QPushButton* btn_debug; кнопка для просмротра состояния перемен в окне экспорта
public:
	WindowExport(const std::list<var>& vars_);
	~WindowExport();
private:
	void init_elements();
	void connect_element();
	void start(bool& flag);
	static void stop(bool& flag);
	void save();
	void save_parameters();
	void add_function_to_var();
	static void function_action_async(std::list<var>& vars, const std::vector<QLabel*>& lbls, const int t);
	static void delete_all_async(std::list<var>& vars, const std::vector<QLabel*>& lbls, const int q);	
};

bool interval(int start, int end, int number);
void make_operation_with_function(std::list<var>& vars, const std::vector<QLabel*>& lbls);
void color_vars(const std::list<var>& vars, const std::vector<QLabel*>& lbls);
void delete_var(std::list<var>& vars, const std::vector<QLabel*>& lbls);
