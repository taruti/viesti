#include <QAction>
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSettings>
#include <QTextEdit>
#include <QToolBar>
#include <QVBoxLayout>
#include <cld2/public/compact_lang_det.h>
#include <string>
#include "composedialog.hh"
#include "../qt5helper/enchanthighlighter.hh"
#include "../qt5helper/singletextedit.hh"


template<typename T>
static QAction* new_act(QWidget *wdg, const QString &text, T functor) {
	auto act = new QAction(text, nullptr);
	act->connect(act, &QAction::triggered, functor);
	wdg->addAction(act);
	return act;
}

ComposeDialog::ComposeDialog(QWidget *parent) : QDialog(parent) {
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
	from_ = new QComboBox;
	from_->setEditable(true);
	gl->addWidget(from_, 2, 1);
	auto tb = new QToolBar;
	new_act(tb, "Send", []() {});
	//	new_act(tb, "Attach", [](){
	//			QFileDialog::getOpenFileNames();
	//		});
	new_act(tb, "Close", [this]() {
		this->close();
	});
	vb->addWidget(tb);
	auto te = new QTextEdit;
	auto hl = new EnchantHighlighter(te->document());
	ehs_.push_back(hl);
	connect(te, &QTextEdit::textChanged, [this, te]() {
		auto u8 = te->toPlainText().toUtf8();
		bool sure;
		auto lang = CLD2::DetectLanguage(u8.data(), u8.size(), false, &sure);
		auto lc = std::string {CLD2::LanguageCode(lang)};
		if(lc == "en")
			lc = "en_US";
		for(auto && e : ehs_)
			e->setLanguage(lc);
	});
	vb->addWidget(te);
	vb->setStretchFactor(te, 100);
	setLayout(vb);
}

void ComposeDialog::showEvent(QShowEvent *event) {
	QSettings s;
	from_->clear();
	from_->addItems(s.value("mail_from").toStringList());
	QDialog::showEvent(event);
}
