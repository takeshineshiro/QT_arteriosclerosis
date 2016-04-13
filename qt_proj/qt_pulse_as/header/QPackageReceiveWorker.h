#pragma once
#include <QObject>
#include <QMutex>
#include <QSemaphore>

class Win_QextSerialPort;

class QPackageReceiveWorker :
	public QObject
{
	Q_OBJECT
public:
	QPackageReceiveWorker(QObject *pParent = NULL);
	virtual ~QPackageReceiveWorker();

    // 初始化串口
    bool initSerialPort();

    // 停止接受
    void stop();

	// 开始接受数据
	void start();

signals:

	// 收到压力值
	void sigRecvPressure(QVector<int>  arrPressure);
	
	// 收到命令
	void sigRecvCmd(short, short);

public slots:
    // 通过串口向动脉硬化仪下位机发送数据或者命令
    // 参数：strMessage [IN] 要发送的命令或者数据
    void slotSendData(const QString &strMessage);

protected slots:

    // 接受数据
    void slotReadData();

private:
	// 验证握手规则
	bool _checkHandle(const QByteArray &byteMsg);

	inline QString _formatPressure(int *pPressure) const;

    // 读写的串口类
    Win_QextSerialPort *m_pSerialPort;

	// 接受到的数据
	QByteArray m_arrRecvData;

    // 是否停止发送收到的消息
    volatile bool m_bIsStop;

    // 保护是否可以停止
    QMutex m_IsStopMutex;
};

