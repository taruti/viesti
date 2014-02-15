#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSettings>
#include <QTextEdit>
#include <QVBoxLayout>
#include <cld2/public/compact_lang_det.h>
#include <string>
#include "composewidget.hh"
#include "../qt5helper/enchanthighlighter.hh"
#include "../qt5helper/singletextedit.hh"


ComposeWidget::ComposeWidget(QWidget *parent) : QWidget(parent) {
	QSettings s;
	s.beginGroup("mail");
	
	auto vb = new QVBoxLayout;
	auto gl = new QGridLayout;
	vb->addLayout(gl);
	gl->addWidget(new QLabel("Subject"), 0, 0);
	auto su = new SingleTextEdit;
	auto shl = new EnchantHighlighter(su->document());
	ehs_.push_back(shl);
	gl->addWidget(su, 0, 1);

	gl->addWidget(new QLabel("To"), 1, 0);
	gl->addWidget(new QLabel("From"), 2, 0);
	auto fc = new QComboBox;
	fc->addItems(s.value("from").toStringList());
	fc->setEditable(true);
	gl->addWidget(fc, 2, 1);
	
	auto te = new QTextEdit;
	auto hl = new EnchantHighlighter(te->document());
	ehs_.push_back(hl);
	connect(te, &QTextEdit::textChanged, [this,te](){
			auto u8 = te->toPlainText().toUtf8();
			bool sure;
			auto lang = CLD2::DetectLanguage(u8.data(), u8.size(), false, &sure);
			auto lc = std::string{CLD2::LanguageCode(lang)};
			if(lc == "en")
				lc = "en_US";
			for(auto &&e : ehs_)
				e->setLanguage(lc);
		});
	vb->addWidget(te);
	vb->setStretchFactor(te,100);
	setLayout(vb);
}