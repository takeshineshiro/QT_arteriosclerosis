#pragma once
#include "qthread.h"

class QSavePressure;

class QSavePressureThread :
	public QThread
{
	Q_OBJECT
public:
	QSavePressureThread(QObject *pParent = NULL);
	virtual ~QSavePressureThread();

	// 停止保存
	void stop();

	// 设置文件名
	void setFileName(const QString &strFileName);

	// 获取当前正在保存文件的绝对路径
	QString getAbsoluteFileName();

public slots:

	void savePressure(const QString & strPressure);

protected:
    
	void run();

private:
	
	// 停止标识
	QMutex m_stopMutex;

	// 更新压力值的锁
	QMutex m_pressureMutex;

	// 更新文件对象锁
	QMutex m_filePressure;
	
	// 控制线程停止
	volatile bool m_isStop;

	// 保存文件的类
	QSavePressure *m_pSaveFile;

	// 要保存的压力值
	QVector<QString> m_arrPressure;

	// 文件名
	QString m_strFileName;
};

