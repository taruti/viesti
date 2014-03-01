#ifndef COMPOSEDIALOG_HH
#define COMPOSEDIALOG_HH

#include <QComboBox>
#include <QDialog>
#include <vector>

class EnchantHighlighter;

class ComposeDialog : public QDialog {
	Q_OBJECT
	// Highlighters for spell checking stored for setting the language
	std::vector<EnchantHighlighter*> ehs_;
	QComboBox *from_;
public:
	ComposeDialog(QWidget *parent = nullptr);
protected:
	void showEvent(QShowEvent *event) override;
};

#endif /* COMPOSEDIALOG_HH */
