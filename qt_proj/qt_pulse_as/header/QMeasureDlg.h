/******************************************************
作用：动脉硬化检测仪的主要测量画面，包括目前测试状态的显示，
      实时波形，及控制测量开始与停止功能

******************************************************/

#ifndef QMEASUREDLG_H
#define QMEASUREDLG_H


class QPersonBasicInfoDlg;
class QSerialRecvThread;
class QDrawWaveWidget;
class QSavePressure;
class QSavePressureThread;
class QSavePressureWorker;
class QPackageReceiveWorker;
class QDrawWaveWorker;

#include "QFilterData.h"

class QMeasureDlg :public QWidget
{
	Q_OBJECT
public:
	QMeasureDlg(QWidget *pParent = NULL);
	
	~QMeasureDlg();

	// 设置患者信息
	void setPersonInfo(QPersonBasicInfoDlg *pPersonInfoDlg);

signals:
	// 给下位机发送命令
	void sigSendMessage(const QString &);

	// 收到下位机的测试结束命令
	void sigRecvTestEndCmd(const QString &, const tagPersonInfo &);

	void sigSaveThread(const QString &);
protected slots:

	// 测量控制按钮的槽函数
	void slotControlMeasure();

	// 接受到消息
	void slotRecvMessage(QByteArray *pRecvMessage);

	// 接受到数据
	void slotRecvData(int *);

	// 接受数据
	void slotRecvData(QVector<int> arrPressure);

	// 接受到命令
	void slotRecvCmd(short nCmdType, short nStatus);

	// 更新显示波形
	void slotUpdateWaveAndPressure(QVector<float>, QVector<int>, bool);
private:
	
	// 初始化窗口
	void _initUI();

	// 控制串口接收线程的启动及停止
	// 参数：[bStart] 若为true,则启动接受线程，否则停止接受线程
	void _startOrEndMeasure(bool bStart = true);

	// 初始化保存四路压力值为txt格式
	void _initSavePressure();

	//  判断袖带是否绑定四肢
	bool _isNoLoad(int *arrPressure);

	// 计算二次升压的压力值
	void _sendSecondBoostCmd();

	// 计算二次升压的压力值
	void _sendSecondBoostPressure(int *pPressure);
private:

	// 描画波形的控件
	QDrawWaveWidget *m_pDrawWaveWidget;

	QDrawWaveWorker *m_pDrawWaveWorker;
	QThread *m_pDrawWaveThread;

	// 显示当前测量状态
	QLabel *m_pShowStatus;

	// 控制测量开始及停止的按钮
	QPushButton *m_pbtnControlMeasure;

	// 主的布局管理器
	QGridLayout *m_pMainLayout;

	// 串口接收线程
	QPackageReceiveWorker *m_pRecvWorker;
	QThread *m_pRecvWorkerThread;

	// 患者信息
	QPersonBasicInfoDlg *m_pPersonInfo;

	// 保存四路压力值为txt格式
	//QSavePressure *m_pSavePressure;

	QSavePressureWorker *m_pSaveFileWorker;
	QThread *m_pSaveFileWorkerThread;

	//// 判断是否为重新测量
	//bool m_bArrive200Pressure[4];
	//// 判断四路压力值是否到达
	//bool m_bHasMax[4];

	//// 臂能到达的最大值
	//int m_nArmMaxValue;

	//// 踝能到达的最大值
	//int m_nAnkleMaxValue;
	//
	//volatile bool m_isSecondMeasure;// 是否为第二次测量

	//int m_nSecondMaxValue[4];

	QFilterData m_filter;
};

#endif
