/******************************************************
作用：显示动脉测量结果，包括以下内容：
      1. 四肢的收缩压及舒张压
	  2. ABI及PWV结果显示
	  3. 测量结果的分析
	  4. 诊断结论

******************************************************/
#pragma once
#include "stdafx.h"


class QShowMeasureResult:public QWidget
{
	Q_OBJECT
public:
	
	// 构造函数
	QShowMeasureResult(QWidget *pParent = NULL);

	// 析构函数
	~QShowMeasureResult();

public slots:

	// 测量结果
	void slotMeasureResult(const QString &,const tagPersonInfo&);

protected slots:
	// 打印按钮
	void slotPrint();

protected:
	void paintEvent(QPaintEvent *pEvent);
	//bool eventFilter(QObject *obj, QEvent *event);

	void resizeEvent(QResizeEvent *pEvent);

private:

	void _initUI();

	// 绘制图形结果
	void _paintGraphShow();

	// 绘制图标的显示，painter在调用前已经设置好视图坐标和窗口坐标
	void _paintGraphShow(QPainter &painter);

	// 绘制表格结果
	void _paintTableShow();

	// 绘制表格显示，painter在调用前已经设置好视图坐标和窗口坐标
	void _paintTableShow(QPainter &painter);

	void _updateResult();

	// 调用dll中的方法，获取测量结果
	void _getABIAndPWV(const QString & strFileName);

	QPointF _getPointByABIAndPWV(const float &fABI, const int &nPWV, const int &nFontHeight, const int &nDeltaX, const int &nDeltaY); 
private:

	//// 表格形式显示收缩压、舒张压、ABI及PWV
	//QWidget *m_pTableShow;
	//
	//// 图表形式显示ABI及PWV
	//QWidget *m_pGraphShow;

	// 诊断结果
	//QTextEdit *m_pedShowResult;

	// 打印按钮
	QPushButton *m_pbtnPrint;

	// 布局管理器
	QVBoxLayout *m_pMainLayout;

	// 左踝PWV
	int m_nLeftPWV;

	// 右踝PWV
	int m_nRightPWV;

	// 左侧ABI
	float m_fLeftABI;

	// 右侧ABI
	float m_fRightABI;

	// 顺序：左上臂收缩压、平均压、舒张压
	QStringList m_listLeftArmPressure;
	
	// 顺序：右上臂收缩压、平均压、舒张压
	QStringList m_listRightArmPressure;

	// 顺序：左脚踝收缩压、平均压、舒张压
	QStringList m_listLeftAnklePressure;

	// 顺序：右脚踝收缩压、平均压、舒张压
	QStringList m_listRightAnklePressure;

	// 测量结果信息
	tagPersonInfo m_infoAndValue;
};

