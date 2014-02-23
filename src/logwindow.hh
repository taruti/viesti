#ifndef LOGWINDOW_HH
#define LOGWINDOW_HH

#include <string>
#include <QDialog>
#include <QSettings>
#include <QPlainTextEdit>

// Logging is thread-safe by calling this global function.
void log(const std::string &val);

class LogWindow : public QDialog {
	Q_OBJECT
	QPlainTextEdit *te_;
	void emitLog(const QString&);
public:
	LogWindow(QWidget *parent = nullptr);
	static LogWindow *instance();
	friend void log(const std::string &val);
signals:
	void log(const QString&);
};


#endif /* LOGWINDOW_HH */
