#ifndef COMPOSEDIALOG_HH
#define COMPOSEDIALOG_HH

#include <QDialog>
#include <vector>

class EnchantHighlighter;

class ComposeDialog : public QDialog {
	Q_OBJECT
	std::vector<EnchantHighlighter*> ehs_;
public:
	ComposeDialog(QWidget *parent = nullptr);
};

#endif /* COMPOSEDIALOG_HH */
