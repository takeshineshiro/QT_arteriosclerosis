/******************************************************
作用：患者信息录入功能，包括录入信息的验证功能，
      并对不符合要求的信息，给出提示信息，具体的验证内容如下：
	  
	  编号是年月日时分秒组成：20151010162930（14位）
      姓名：30个字符
      年龄：两位数或者三位数
      身高：两位数或者三位数
      备注：300个字符

******************************************************/
#ifndef _QPERSONBASICINFODLG_H
#define _QPERSONBASICINFODLG_H
#include "qwidget.h"
#include "stdafx.h"

// 向前引用
namespace Ui {
    class PersonBasicInfo;
}

class QPersonBasicInfoDlg:public QWidget
{
	Q_OBJECT
public:
	
	QPersonBasicInfoDlg(QWidget *pParent = NULL);
	
	~QPersonBasicInfoDlg();

	// 获取患者信息
	tagPersonInfo getPersonInfo();

	// 获取患者姓名
	QString getPersonName() const;

	// 重新设置患者信息默认值
	void resetInfo();

	// 检查用户输入的数据是否正确
	bool isValidInput();

protected slots:
	// 备注的文本框
	void slotNoteTextChange();

private:
	Ui::PersonBasicInfo *m_pPersonInfo;
};

#endif