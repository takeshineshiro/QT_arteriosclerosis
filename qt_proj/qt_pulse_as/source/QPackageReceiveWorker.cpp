#include "./header/stdafx.h"
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QDebug>
#include "./header/packagecommon.h"
#include "./header/PackageSendCmd.h"
#include "./header/PackageRecvCmd.h"
#include "./header/win_qextserialport.h"
#include "./header/QPackageReceiveWorker.h"


QPackageReceiveWorker::QPackageReceiveWorker(QObject *pParent)
	:QObject(pParent)
{
	// 创建串口读取类
	m_pSerialPort = new Win_QextSerialPort();
}

QPackageReceiveWorker::~QPackageReceiveWorker()
{
	if(m_pSerialPort != NULL)
	{
		Q_DELETE(m_pSerialPort);
	}
}

bool QPackageReceiveWorker::initSerialPort()
{	
	// 判断串口是否打开
	if (m_pSerialPort->isOpen())
	{
		return true;
	}

	m_pSerialPort->close();

	PackageSendCmd package;
	QString strHandle = package.getNoramlStart();
	// TODO: 判断串口的唯一性标识
	foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
	{
		m_pSerialPort->setPortName(info.portName());
		m_pSerialPort->setQueryMode(QextSerialBase::EventDriven);
		bool bOpen = m_pSerialPort->open(QIODevice::ReadWrite);

		QSettings setttings("settings.ini", QSettings::IniFormat);
		int nBaudRate = setttings.value("BAUD", 256000).toInt();
		switch(nBaudRate)
		{
		case 128000:
			{
				m_pSerialPort->setBaudRate(BAUD128000);
			}
			break;
		case 115200:
			{
				m_pSerialPort->setBaudRate(BAUD115200);
			}
			break;
		case 256000:
			{
				m_pSerialPort->setBaudRate(BAUD256000);
			}
			break;
		default:
			m_pSerialPort->setBaudRate(BAUD256000);
			break;
		}

		//数据位设置，我们设置为8位数据位
		m_pSerialPort->setDataBits(DATA_8);

		
        //奇偶校验设置，我们设置为无校验
		m_pSerialPort->setParity(PAR_NONE);

		
        //停止位设置，我们设置为1位停止位
		m_pSerialPort->setStopBits(STOP_1);

		

		m_pSerialPort->setFlowControl(FLOW_OFF);

		
		//if (m_pSerialPort->open(QIODevice::ReadWrite) &&
		//    m_pSerialPort->setBaudRate(QSerialPort::Baud115200) &&
		//	m_pSerialPort->setDataBits(QSerialPort::Data8) &&
		//	m_pSerialPort->setStopBits(QSerialPort::UnknownStopBits) &&
		//	m_pSerialPort->setFlowControl(QSerialPort::NoFlowControl) &&
		//	m_pSerialPort->setParity(QSerialPort::NoParity))
		if(bOpen/* && bBaudRate &&  bDataBits && bStopBits &&  bFlowControl && bParity*/)
		{
			//int nWriteSize = m_pSerialPort->write(QByteArray::fromHex(strHandle.toLocal8Bit()), strHandle.size());
			////file.write(info.manufacturer().toLocal8Bit() + "BBBBB" + QString::number(nWriteSize).toLocal8Bit() + QString::number(timer.elapsed()).toLocal8Bit() + "\r\n");
			//if(nWriteSize == -1)
			//{
			//	//file.write(info.manufacturer().toLocal8Bit() + "CCCC" + QString::number(nWriteSize).toLocal8Bit() + "\t" + QString::number(timer.elapsed()).toLocal8Bit() + "\r\n");
			//	//QMessageBox::information(NULL, "PortName", "Failed Because！" + m_pSerialPort->errorString() + "\r\n");
			//	continue;
			//}

			//// 等待5秒钟
			//if(m_pSerialPort->waitForReadyRead(5*1000))
			//{
			//	//file.write(info.manufacturer().toLocal8Bit() + "DDDD" + QString::number(timer.elapsed()).toLocal8Bit() + "\r\n");
			//	QByteArray byteRecv = m_pSerialPort->readAll().toHex();
			//	bool bIsValiad = _checkHandle(byteRecv);
			//	//file.write(info.manufacturer().toLocal8Bit() + "EEEE" + byteRecv+ QString::number(timer.elapsed()).toLocal8Bit() + "\r\n");
			//	if (bIsValiad)
			//	{
			//		//file.write(info.manufacturer().toLocal8Bit() + "FFFFF" + byteRecv + QString::number(timer.elapsed()).toLocal8Bit() + "\r\n");
			//		// 设置串口最多存储100个包的大小
			//		setReadBufferSize(10000);
			connect(m_pSerialPort, SIGNAL(readyRead()), this, SLOT(slotReadData()));
			return true;
			//	}
			//}
			//else
			//{
			//	//file.write(info.manufacturer().toLocal8Bit() + "close" + QString::number(timer.elapsed()).toLocal8Bit() + "\r\n");
			//	m_pSerialPort->close();
			//	//m_pSerialPort->reset();
			//}
		}
	}
	//file.close();
	// 获取串口初始化失败的信息
	QString strErrors = m_pSerialPort->errorString();
	QMessageBox::information(NULL, QString::fromLocal8Bit("未连接错误"), strErrors+QString::fromLocal8Bit("\n请按以下顺序检查：\n1.动脉硬化检测仪是否与电脑连接;\n2.动脉硬化检测仪的电源是否接通;\n"));
	return false;
}

void QPackageReceiveWorker::slotSendData(const QString &strMessage)
{
	int  nRet = m_pSerialPort->write(QByteArray::fromHex(strMessage.toLocal8Bit()), strMessage.size());
	// 发送失败的情况下，再次初始化串口，并发送
	if (nRet != strMessage.size())
	{
		if(m_pSerialPort->isOpen())
		{
			nRet = m_pSerialPort->write(QByteArray::fromHex(strMessage.toLocal8Bit()));
		}
		// 打开串口成功后，再次发送
		else if(initSerialPort())
		{
			nRet = m_pSerialPort->write(QByteArray::fromHex(strMessage.toLocal8Bit()));
		}
	}
}

void QPackageReceiveWorker::slotReadData()
{

	QMutexLocker lock(&m_IsStopMutex);
	m_arrRecvData.append(m_pSerialPort->readAll().toHex());
	//qDebug()<<"QPackageReceiveWorker:"<<thread()->currentThreadId()<<"\t"<<(int)thread();
	PackageCommon package;
	// 获取每个包的长度及包头信息
	int nPackageLength = package.getPackageLength();
	QString strHeader = package.getPackageHeader();
	{
		// 内存中已经接受到完整包的个数
		int nRecvCount = m_arrRecvData.size() / nPackageLength;
		if(nRecvCount == 0)
		{

			return;
		}

		// 记录已经读取完整包的个数
		int nCount = 0;

		// 已经读取了的字节数
		int nIndex = 0;
		for(; nIndex < m_arrRecvData.size() && nCount < nRecvCount;
			nIndex += nPackageLength)
		{
			QByteArray arrRecv = m_arrRecvData.mid(nIndex, nPackageLength);
			//qDebug()<<"Recv:"<<arrRecv;

			ParseRecvCmd parse(arrRecv);

			// 如果开头不是包头，校准包头
			if(!parse.isLengthOk() || !parse.isHeaderOk()
				|| !parse.isTailOk() || !parse.isSumOk())
			{
				// 重新找到包头
				int nNextHeaderIndex = m_arrRecvData.indexOf(strHeader, nIndex + 1);
				// 未找到包头的情况下
				if (nNextHeaderIndex < 0)
				{
					//qDebug()<<"nRCount:3";
					break;
				}

				int nLength = m_arrRecvData.length();
				//qDebug()<<"Before Remove"<<m_arrRecvData;

				// 移除不完整包
				m_arrRecvData.remove(0, nNextHeaderIndex);
				//qDebug()<<"After Remove"<<m_arrRecvData;

				// 重新计算完整包的个数
				nRecvCount = m_arrRecvData.size() / nPackageLength;

				// 重置发送包的数据量
				nIndex = 0;
				nCount = 0;
				//qDebug()<<"nRCount:4";
				continue;
			}

			static ParseRecvCmd::RecvType preType = ParseRecvCmd::eINVALID_CMD;
			static short nPreStatus = 0xff;

			ParseRecvCmd::RecvType eType = parse.recvDataType();
			// 根据不同的命令，做出不同的相应
			switch (eType)
			{
				// 收到测量数据
			case ParseRecvCmd::eRECV_DATA:
				{
					int pPressure[4] = {};
					if(NULL == pPressure)
					{
						continue;
					}

					parse.getPressure(pPressure);
					QVector<int> arrPressure;
					for(int i=0; i<4; ++i)
					{
						arrPressure.append(*(pPressure+i));
					}

					emit sigRecvPressure(arrPressure);
				}
				break;
			case ParseRecvCmd::eSend_TEST_CMD:// 测试结束开始命令
			case ParseRecvCmd::eRECV_TEST_END_CMD:// 反馈命令
				{
					int nCurrStatus = parse.getStatus();
					if(preType == eType &&  nCurrStatus == nPreStatus)
					{
						break;
					}
					preType = eType;
					nPreStatus = nCurrStatus;
					emit sigRecvCmd((short)eType, nCurrStatus);
				}
				break;
			default:
				break;
			}
			nCount++;			
		}

		// 移除处理过的命令
		m_arrRecvData.remove(0, m_arrRecvData.length() - m_arrRecvData.length()%nPackageLength);
	}
}

// 停止接受
void QPackageReceiveWorker::stop()
{
	// 设置停止
	//QMutexLocker lock(&m_IsStopMutex);
	//m_bIsStop = true;
	disconnect(m_pSerialPort, SIGNAL(readyRead()), this, SLOT(slotReadData()));
	m_arrRecvData.clear();
}

// 开始接受数据
void QPackageReceiveWorker::start()
{
	//// 设置停止
	//QMutexLocker lock(&m_IsStopMutex);
	//m_bIsStop = false;
	m_pSerialPort->readAll();
	connect(m_pSerialPort, SIGNAL(readyRead()), this, SLOT(slotReadData()));
	m_arrRecvData.clear();
}

// 验证握手规则
bool QPackageReceiveWorker::_checkHandle(const QByteArray &byteMsg)
{
	PackageCommon packageInfo;
	int nPackageLength = packageInfo.getPackageLength();

	for(int nIndex = 0; nIndex < byteMsg.size();)
	{
		ParseRecvCmd parse(byteMsg.mid(nIndex, nPackageLength));
		if(!parse.isLengthOk() || !parse.isHeaderOk()
			|| !parse.isTailOk() || !parse.isSumOk())
		{
			int nPreIndex = nIndex;
			nIndex = byteMsg.indexOf(packageInfo.getPackageHeader(), nIndex + 1);
			if(nIndex == -1)
			{
				break;
			}
			continue;
		}

		ParseRecvCmd::RecvType eType = parse.recvDataType();
		if(eType == ParseRecvCmd::eHANDLE_CMD)
		{
			return true;
		}
	}

	return false;
}

inline QString QPackageReceiveWorker::_formatPressure(int *pPressure) const
{
	QString strValue;
	for (int j = 0; j < 4; ++j)
	{
		int nCurrValue = *(pPressure + 3 - j);
		strValue += QString::number(nCurrValue);

		if(j + 1 < 4)
		{
			strValue += "; ";
		}
	}

	strValue += "\r\n";
	return strValue;
}
