#pragma once

#include <QtWidgets>
#include <fstream>
#include <string>

namespace chrono = std::chrono;

class WindowOpen : public QWidget
{
private:
	QTextEdit* text_box_edit;
	QPushButton* btn_save;
	QPushButton* btn_clear;
	QPushButton* btn_close;
	QLabel* lbl_name_of_file;
	std::fstream file_manager;
	std::string path;

	bool is_saved = false;
public:
	WindowOpen(const std::string& path_);
	~WindowOpen();
private:
	void init_elements();
	void open_file();
	void connect_items();
	void save_file();
	void erase_file() const;
	void file_changed_event();
};
