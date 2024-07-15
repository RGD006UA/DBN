#pragma once

#include <QtWidgets>

#include <vector>
#include <var.hpp>

class MainWindow : public QMainWindow
{
private:
    QPushButton* btn_add;
    QPushButton* btn_open;
    QPushButton* btn_export;
    QPushButton* btn_delete_var;
    QPushButton* btn_edit_var;
	QListWidget* var_table;
    QLabel* lbl_index;

    std::vector<var> vars;
    int index = 0;
public:
    MainWindow();
    ~MainWindow() override;
private:
    void connect_elements();
    void init_elements();
    void get_index();
    void add_var();
    void open_var();
    void edit_var();
    void delete_var();
    void export_var();
};
