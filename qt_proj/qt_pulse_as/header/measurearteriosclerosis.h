/********************************************************
作用：动脉硬化检测主界面

********************************************************/

#ifndef MEASUREARTERIOSCLEROSIS_H
#define MEASUREARTERIOSCLEROSIS_H

#include <QtWidgets/QDialog>

class QPersonBasicInfoDlg;
class QTabManagerDlg;
class QHBoxLayout;

class MeasureArteriosclerosis : public QDialog
{
	Q_OBJECT

public:
	MeasureArteriosclerosis(QWidget *parent = 0);

	~MeasureArteriosclerosis();

private:

	// UI初始化操作
	void _initUI();

private:

	// 患者信息录入对话框
	QPersonBasicInfoDlg *m_pBasicInfo;

	// 各个功能的总tab页
	QTabManagerDlg *m_pMainTab;

	// 版权信息
	QLabel *m_pVersionInfo;

	// 界面主布局管理器
	QHBoxLayout *m_pMainLayout;

	// 界面主布局管理器
	QVBoxLayout *m_pLayout;
};

#endif // MEASUREARTERIOSCLEROSIS_H
