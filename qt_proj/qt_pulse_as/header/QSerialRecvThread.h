#ifndef QSERIALRECVTHREAD_H
#define QSERIALRECVTHREAD_H

#include <QThread>
#include <QSerialPort>
#include <QMutex>
#include <QSemaphore>

class Win_QextSerialPort;

class QSerialRecvThread:public QThread
{
	Q_OBJECT
public:
    // 构造函数
    QSerialRecvThread(QObject *pParent = NULL);

    virtual ~QSerialRecvThread();

    // 初始化串口
    bool initSerialPort();

    // 停止接受
    void stop();

	void start();

    // 设置串口接收数据的个数(默认100)
    void setReadBufferSize(const int nSize = 100);
signals:

    // 收到数据的信息
    void sigRecvData(QByteArray *);

	// 收到压力值
	void sigRecvPressure(int *);

	// 保存数据
	void sigSaveData(const QString &);

	// 收到命令
	void sigRecvCmd(short, short);
public slots:

    // 通过串口向动脉硬化仪下位机发送数据或者命令
    // 参数：strMessage [IN] 要发送的命令或者数据
    void slotSendData(const QString &strMessage);

protected slots:

    // 接受数据
    void slotReadData();

protected:

    void run();

private:
	// 验证握手规则
	bool _checkHandle(const QByteArray &byteMsg);

	inline QString _formatPressure(int *pPressure) const;

private:

    // 读写的串口类
    Win_QextSerialPort *m_pSerialPort;

    // 接受到的数据缓存区
    QByteArray m_arrRecvData;

    // 是否停止发送收到的消息
    volatile bool m_bIsStop;

    // 保护是否可以停止
    QMutex m_IsStopMutex;

    // 读取数据
    QSemaphore m_readSema;

    // 处理数据
    QSemaphore m_dealSema;
};
#endif // QSERIALRECVTHREAD_H
