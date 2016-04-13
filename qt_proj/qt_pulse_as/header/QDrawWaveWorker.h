#ifndef Q_DRAWWAVE_WORKER_H
#define Q_DRAWWAVE_WORKER_H

#include "QFilterData.h"

class QDrawWaveWorker: public QObject
{
	Q_OBJECT
public:
	QDrawWaveWorker(QObject *pParent = NULL);

	virtual ~QDrawWaveWorker();

	// 停止显示
	void stop();

	// 开始描绘
	void start();
signals:

	void sigUpdateWavePressure(QVector<float> *, QVector<int>, bool);

public slots:

	// 接受压力值，并更新显示波形
	void slotFiterWave(QVector<int>);

	// 重置滤波
	void slotResetMaxValue(int, int);

private slots:

	void slotUpdateTimer();

private:
	// 滤波
	QFilterData m_filter;

	// 保存上次的压力值
	QVector<int> m_lastPressure;

	// 上次接受到的240个包，即300ms处理一次数据
	QVector<int> m_arrLast240;

	// 臂能到达的最大值
	int m_nArmMaxValue;

	// 踝能到达的最大值
	int m_nAnkleMaxValue;

	// 判断四路压力值是否到达
	bool m_bHasMax[4];
};

#endif