#ifndef TEXTAREA_H
#define TEXTAREA_H

#include <QObject>
#include <QWidget>
#include <QPlainTextEdit>

class TextArea:public QPlainTextEdit{
	Q_OBJECT
	public:
		TextArea();
		void OpenSpecificFile(QString fileurl);

	private slots:
		void OpenFile();
		void SaveFile();
	private:
		bool checkExtension(QFile *f);

		QFile *currentFile = nullptr ;
		const QString fileextension[12] = {"txt","html","css","java","cpp","c","hpp","h","py","php","js","xml"};
};

#endif
