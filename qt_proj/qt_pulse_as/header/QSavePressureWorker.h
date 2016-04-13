#ifndef Q_SAVEPRESSURE_WORKER_H
#define Q_SAVEPRESSURE_WORKER_H

class QSavePressure;

class QSavePressureWorker :
	public QObject
{
	Q_OBJECT
public:
	QSavePressureWorker(QObject *pParent = NULL);

	~QSavePressureWorker();

	// 设置文件名
	void setFileName(const QString &strFileName);

	// 获取当前正在保存文件的绝对路径
	QString getAbsoluteFileName();

	// 停止
	void stop();
signals:

	// 发送二次升压命令
	void sigSecondBoostCmd(const QString &);

	// 重置滤波参数
	void sigResetMaxValue(int, int);
public slots:

	void savePressure(QVector<int> arrPressure);

private:
	
	// 初始化变量
	void _reset();

	// 是否需要二次测量
	void _isCanSecondBoost(QVector<int> arrPressure);

	// 调用算法dll,发送第二次升压命令
	void _sendSecondBoostCmd();

private:
	// 保存文件的类
	QSavePressure *m_pSaveFile;

	// 更新文件对象锁
	QMutex m_filePressure;

	// 判断是否为重新测量
	bool m_bArrive200Pressure[4];

	// 臂能到达的最大值
	int m_nArmMaxValue;

	// 踝能到达的最大值
	int m_nAnkleMaxValue;
	
	volatile bool m_isSecondMeasure;// 是否为第二次测量
};

#endif