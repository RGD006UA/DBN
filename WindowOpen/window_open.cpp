#include <window_open.hpp>

WindowOpen::WindowOpen(const std::string& path_)
	: QWidget(nullptr), path(path_)
{
	init_elements();
	open_file();
	setWindowTitle(QString::fromStdString(path + (is_saved ? "" : "*")));

	auto* vbox_main = new QVBoxLayout;
	vbox_main->addWidget(lbl_name_of_file);
	vbox_main->addWidget(text_box_edit);

	auto* hbox_btn = new QHBoxLayout;
	hbox_btn->addWidget(btn_save);
	hbox_btn->addWidget(btn_clear);
	hbox_btn->addWidget(btn_close);

	vbox_main->addLayout(hbox_btn);

	connect_items();

	setLayout(vbox_main);
}

void WindowOpen::init_elements()
{
	text_box_edit = new QTextEdit;
	btn_save = new QPushButton("save");
	btn_clear = new QPushButton("clear");
	btn_close = new QPushButton("close");
	lbl_name_of_file = new QLabel("name: " + QString::fromStdString(path));
}

void WindowOpen::connect_items()
{
	connect(btn_save, &QPushButton::clicked, this, [this]() { save_file(); });
	connect(btn_clear, &QPushButton::clicked, this, [this]() { erase_file(); });
	connect(btn_close, &QPushButton::clicked, this, &QWidget::close);
	connect(text_box_edit, &QTextEdit::textChanged, this, [this]() { file_changed_event(); });
}

void WindowOpen::save_file()
{
	file_manager.open(path, std::ios::out);

	const std::string res(text_box_edit->toPlainText().toStdString());

	if (file_manager.is_open())
	{
		file_manager << res << std::endl;

		is_saved = true;
		setWindowTitle(QString::fromStdString(path + (is_saved ? "" : "*")));
	}

	file_manager.close();
}

void WindowOpen::erase_file() const
{
	text_box_edit->setText("");
}

void WindowOpen::file_changed_event()
{
	is_saved = false;
	setWindowTitle(QString::fromStdString(path + (is_saved ? "" : "*")));
}

void WindowOpen::open_file()
{
	file_manager.open(path, std::ios::in);

	std::string temp; 
	QString str_to_text_edit;

	while (std::getline(file_manager, temp))
		str_to_text_edit += QString::fromStdString(temp) + "\n";

	text_box_edit->setText(str_to_text_edit);

	file_manager.close();
}

WindowOpen::~WindowOpen()
{
	delete text_box_edit;
	delete btn_save;
	delete btn_clear;
	delete btn_close;
	delete lbl_name_of_file;
}
