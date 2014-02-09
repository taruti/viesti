#include <QTextEdit>
#include <QVBoxLayout>
#include "composewidget.hh"
#include "../qt5helper/enchanthighlighter.hh"

ComposeWidget::ComposeWidget(QWidget *parent) : QWidget(parent) {
	auto vb = new QVBoxLayout;
	auto te = new QTextEdit;
	auto hl = new EnchantHighlighter(te->document());
	hl->setLanguage("en_US");
	vb->addWidget(te);
	setLayout(vb);
}
