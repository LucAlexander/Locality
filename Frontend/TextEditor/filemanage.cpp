#include "filemanage.h"
#include <QFileDialog>
#include <QWidget>
#include <QMessageBox>
#include <QDebug>
#include <QTextStream>

Tab::Tab(){
	this->setTabsClosable(true);
	myTextArea = new TextArea();
	this->addTab(myTextArea,"unnamed");
}

void Tab::createNewTab(){
	TextArea *ta = new TextArea();
	this->addTab(ta,"New Tab");
};

FileManage::FileManage(){
	activeFile = new QFile();
}

QString FileManage::getFileText(QFile *f){

}

void FileManage::openFile(){

}

bool FileManage::checkExtension(QFile *f){

}

void FileManage::SaveFile(QFile *f){

}
