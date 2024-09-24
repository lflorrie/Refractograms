#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#define QT_MESSAGE_PATTERN "[%{time dd.MM.yyyy hh:mm:ss.zzz} %{if-debug}DEBUG%{endif}%{if-info}INFO%{endif}%{if-warning}WARN%{endif}%{if-critical}CRIT%{endif}%{if-fatal}FATAL%{endif}] %{file}:%{line} - %{message}"

QtMessageHandler originalHandler = nullptr;

// TODO:Add log rotating
void logToFile(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	QString message = qFormatLogMessage(type, context, msg);
	static FILE *f = fopen("log.txt", "a");
	fprintf(f, "%s\n", qPrintable(message));
	fflush(f);

	if (originalHandler)
		originalHandler(type, context, msg);
}

int main(int argc, char *argv[])
{
	qSetMessagePattern(QT_MESSAGE_PATTERN);
	originalHandler = qInstallMessageHandler(logToFile);

	qInfo() << "\n\nApplication \"Refractograms\" started.";
	QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Refractograms_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();
    return a.exec();
}
