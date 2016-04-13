/******************************************************
作用：动脉硬化测量结果管理，具体包括以下功能：
      1. 测量结果的增、删、查
	  2. 测量结果的展示
	  3. 测量结果的对比展示

******************************************************/

#pragma once

class QWidget;

namespace Ui {
    class MedicalRecordDlg;
} 

class QMedicalRecordDlg :public QWidget
{
	Q_OBJECT
public:
	QMedicalRecordDlg(QWidget *pParent = NULL);

	~QMedicalRecordDlg();

protected slots:
	
	// 查询按钮
	void slotQuery();

	// 改变鼠标事件
	void slotScanDetail();
private:

	// 初始化UI
	void _initUI();

private:
	// 查询对话框
	Ui::MedicalRecordDlg *m_pRecordDlg;

	// 测量结果对应关系
	QMap<int, tagPersonInfo *> m_mapRowToInfo;
};

