#include <mainwindow.h>
#include <window_open.hpp>
#include <window_edit.hpp>
#include <window_export.hpp>

MainWindow::MainWindow()
{
	init_elements();

	resize(1000, 600);

	auto* widget = new QWidget;
	auto* vbox_main = new QVBoxLayout;
	auto* hbox_header = new QHBoxLayout;

	hbox_header->addWidget(btn_add);
	hbox_header->addWidget(btn_open);
	hbox_header->addWidget(btn_export);

	vbox_main->addLayout(hbox_header);

	auto* hbox_footer = new QHBoxLayout;
	hbox_footer->addWidget(var_table);

	auto* vbox_btn = new QVBoxLayout;
	vbox_btn->addWidget(btn_delete_var);
	vbox_btn->addWidget(btn_edit_var);

	hbox_footer->addLayout(vbox_btn);

	vbox_main->addLayout(hbox_footer);
	vbox_main->addWidget(lbl_index);

	widget->setLayout(vbox_main);


	connect_elements();
	setCentralWidget(widget);
}

MainWindow::~MainWindow()
{
	delete btn_add;
	delete btn_open;
	delete btn_export;
	delete btn_delete_var;
	delete btn_edit_var;
	delete var_table;
	delete lbl_index;
}

void MainWindow::connect_elements()
{
	connect(var_table, &QListWidget::clicked, this, [this]() { get_index(); });
	connect(btn_add, &QPushButton::clicked, this, [this]() { add_var(); });
	connect(btn_open, &QPushButton::clicked, this, [this]() { open_var(); });
	connect(btn_edit_var, &QPushButton::clicked, this, [this]() { edit_var(); });
	connect(var_table, &QListWidget::doubleClicked, this, [this]() { edit_var(); });
	connect(btn_delete_var, &QPushButton::clicked, this, [this]() { delete_var(); });
	connect(btn_export, &QPushButton::clicked, this, [this]() { export_var(); });
}

void MainWindow::get_index()
{
	index = var_table->currentIndex().row();
	lbl_index->setText("index: " + QString::number(index + 1));
}

void MainWindow::export_var()
{
	const std::list<var> tmp(begin(vars), end(vars));
	auto* export_window = new WindowExport(tmp);

	export_window->show();
}


void MainWindow::open_var()
{
	const std::string path = QFileDialog::getOpenFileName(this, "open file", QDir::homePath()).toStdString();

	if (path.empty())
		return;

	auto* window_open = new WindowOpen(path);

	window_open->show();
}

void MainWindow::edit_var()
{
	var& var_to_edit = vars[index];
	QListWidgetItem* item = var_table->item(index);

	auto* window_edit = new WindowEdit(var_to_edit, item);

	window_edit->show();
}

void MainWindow::delete_var()
{
	if (vars.empty())
		return;

	var_table->removeItemWidget(var_table->takeItem(index));
	std::swap(vars[index], vars.back());
	vars.pop_back();
	index = 0;
	lbl_index->setText("index: " + QString::number(index));
}

void MainWindow::add_var()
{
	const QString path = QFileDialog::getOpenFileName(this, "Open file", QDir::homePath());

	if (path.isEmpty())
		return;

	const var temp(path);

	var_table->addItem(temp('a'));
	vars.push_back(temp);
}

void MainWindow::init_elements()
{
	btn_add = new QPushButton("add variable");
	btn_open = new QPushButton("open file");
	btn_export = new QPushButton("export window");
	btn_edit_var = new QPushButton("edit variable");
	btn_delete_var = new QPushButton("delete variable");
	var_table = new QListWidget;
	lbl_index = new QLabel("index:" + QString::number(index));
}