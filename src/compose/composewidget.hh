#ifndef COMPOSEWIDGET_HH
#define COMPOSEWIDGET_HH

#include <QWidget>
#include <vector>

class EnchantHighlighter;

class ComposeWidget : public QWidget {
	Q_OBJECT
	std::vector<EnchantHighlighter*> ehs_;
public:
	ComposeWidget(QWidget *parent = nullptr);
};

#endif /* COMPOSEWIDGET_HH */
