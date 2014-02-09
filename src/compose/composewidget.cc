#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include "composewidget.hh"
#include "../qt5helper/enchanthighlighter.hh"
#include "../qt5helper/singletextedit.hh"

ComposeWidget::ComposeWidget(QWidget *parent) : QWidget(parent) {
	auto vb = new QVBoxLayout;
	auto gl = new QGridLayout;
	vb->addLayout(gl);
	gl->addWidget(new QLabel("Subject"), 0, 0);
	auto su = new SingleTextEdit;
	auto shl = new EnchantHighlighter(su->document());
	shl->setLanguage("en_US");
	gl->addWidget(su, 0, 1);

	gl->addWidget(new QLabel("To"), 1, 0);
	gl->addWidget(new QLabel("From"), 2, 0);
	auto fc = new QComboBox;
	fc->setEditable(true);
	gl->addWidget(fc, 2, 1);
	
	auto te = new QTextEdit;
	auto hl = new EnchantHighlighter(te->document());
	hl->setLanguage("en_US");
	vb->addWidget(te);
	vb->setStretchFactor(te,100);
	setLayout(vb);
}
