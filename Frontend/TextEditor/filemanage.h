#ifndef FILEMANAGE_H
#define FILEMANAGE_H

#include <QObject>
#include <QWidget>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QTabWidget>

#include "textarea.h"

class Tab : public QTabWidget{
	Q_OBJECT
	public:
		Tab();
		TextArea* myTextArea;
	private slots:
		void createNewTab();
};

class FileManage:public QWidget{
	Q_OBJECT
	public:
		FileManage();
		QString getFileText(QFile *f);

	private slots:
		void openFile();
		void SaveFile(QFile *f);

	private:
		QFile* activeFile ;
		bool checkExtension(QFile *f);
		QString text;
		const QString fileextension[12] = {"txt","html","css","java","cpp","c","hpp","h","py","php","js","xml"};
};

#endif
