#include <QPushButton>
#include <QVBoxLayout>
#include "logwindow.hh"

LogWindow::LogWindow(QWidget *parent) :
	QDialog(parent) {
	auto vb = new QVBoxLayout;
	te_ = new QPlainTextEdit;
	te_->setReadOnly(true);
	vb->addWidget(te_);
	auto clear = new QPushButton("Clear");
	connect(clear, &QPushButton::pressed, [&]() { te_->document()->clear(); });
	vb->addWidget(clear);
	setLayout(vb);
	connect(this, SIGNAL(log(const QString&)), te_, SLOT(appendPlainText(const QString&)));
}

LogWindow* LogWindow::instance() {
	static LogWindow lw;
	return &lw;
}

void LogWindow::emitLog(const QString& msg) {
	emit(log(msg));
}


void log(const std::string &val) {
	LogWindow::instance()->emitLog(QString::fromStdString(val));
}
