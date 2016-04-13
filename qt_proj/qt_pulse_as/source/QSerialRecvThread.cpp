#include "./header/stdafx.h"
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QDebug>
#include "./header/packagecommon.h"
#include "./header/PackageSendCmd.h"
#include "./header/PackageRecvCmd.h"
#include "./header/QSerialRecvThread.h"
#include "./header/win_qextserialport.h"

QSerialRecvThread::QSerialRecvThread(QObject *pParent)
	:QThread(pParent),m_bIsStop(false),m_readSema(5),m_dealSema(0)
{
	// 创建串口读取类
	m_pSerialPort = new Win_QextSerialPort();
}

QSerialRecvThread::~QSerialRecvThread()
{
	if(m_pSerialPort != NULL)
	{
		m_pSerialPort->close();
		delete m_pSerialPort;
		m_pSerialPort = NULL;
	}
}

//void QSerialRecvThread::run()
//{
//	PackageCommon package;
//	// 获取每个包的长度及包头信息
//	int nPackageLength = package.getPackageLength();
//	QString strHeader = package.getPackageHeader();
//
//	static int nRCount = 0;
//	while(!m_bIsStop)
//	{
//		//continue;
//		if(0 == m_readSema.available())
//		{
//			m_readSema.release();
//		}
//
//		m_dealSema.acquire();
//		{
//			// 内存中已经接受到完整包的个数
//			int nRecvCount = m_arrRecvData.size() / nPackageLength;
//
//			if(nRecvCount == 0)
//			{
//				continue;
//			}
//
//			QByteArray *pRecvData = new QByteArray();
//			// 记录已经读取完整包的个数
//			int nCount = 0;
//
//			// 已经读取了的字节数
//			int nIndex = 0;
//			for(; nIndex < m_arrRecvData.size() && nCount < nRecvCount;
//				nIndex += nPackageLength)
//			{
//				QByteArray arrRecv = m_arrRecvData.mid(nIndex, nPackageLength);
//				ParseRecvCmd parse(arrRecv);
//
//				// 如果开头不是包头，校准包头
//				if(!parse.isLengthOk() || !parse.isHeaderOk()
//					|| !parse.isTailOk() || !parse.isSumOk())
//				{
//					// 重新找到包头
//					int nNextHeaderIndex = m_arrRecvData.indexOf(strHeader, nIndex + 1);
//					// 未找到包头的情况下
//					if (nNextHeaderIndex < 0)
//					{
//						break;
//					}
//
//					int nLength = m_arrRecvData.length();
//					//qDebug()<<"Before Remove"<<m_arrRecvData;
//
//					// 移除不完整包
//					m_arrRecvData.remove(0, nNextHeaderIndex);
//					nRCount += nLength;
//					//qDebug()<<"After Remove"<<m_arrRecvData;
//
//					// 重新计算完整包的个数
//					nRecvCount = m_arrRecvData.size() / nPackageLength;
//
//					// 重置发送包的数据量
//					nIndex = 0;
//					nCount = 0;
//					continue;
//				}
//
//				nCount++;
//				pRecvData->append(arrRecv);
//			}
//
//			// 移除已经发送的数据
//			m_arrRecvData.remove(0, m_arrRecvData.length() - m_arrRecvData.length()%nPackageLength);
//			//qDebug()<<"Release Data"<<m_arrRecvData;
//			// 通知主界面，处理数据
//			if (!pRecvData->isEmpty())
//			{
//				emit sigRecvData(pRecvData);
//			}
//			else
//			{
//				pRecvData->resize(0);
//				delete pRecvData;
//				pRecvData = NULL;
//			}
//		}
//	}
//	//qDebug()<<"nRCount:"<<nRCount;
//	QMutexLocker lock(&m_IsStopMutex);
//	m_bIsStop = false;
//}

#if _WAVE_ANALYSIS 
void QSerialRecvThread::run()
{
	while(!m_bIsStop)
	{
		QFile file1(QString::fromLocal8Bit("E:/Lepu_20150810/LepuMedical/SvnFile/MeasureArteriosclerosis(VS)/MeasureArteriosclerosis/MeasureArteriosclerosis/Data/20160310(800HZ王瀚博)_1.txt"));
		if(file1.open(QFile::ReadOnly))
		{
			QTextStream in(&file1);
			while(!in.atEnd())
			{
				QStringList strValue = in.readLine().remove("\r\n").split("; ");
				int nLeftAnkle = strValue.at(0).toInt();
				int nRightAnkle = strValue.at(1).toInt();
				int nLeftArm = strValue.at(2).toInt();
				int nRightArm = strValue.at(3).toInt();

				int *pPressure = new int[4];
				memset(pPressure, 0x00, sizeof(pPressure[0])*4);
				pPressure[0] = nRightArm;
				pPressure[1] = nLeftArm;
				pPressure[2] = nRightAnkle;
				pPressure[3] = nLeftAnkle;

				emit sigRecvPressure(pPressure);

				msleep(5);
			}
		}
		file1.close();
		break;

		QFile file2(QString::fromLocal8Bit("E:/Lepu_20150810/LepuMedical/SvnFile/MeasureArteriosclerosis(VS)/MeasureArteriosclerosis/MeasureArteriosclerosis/Data/20160310(800HZ王瀚博)_2.txt"));
		if(file2.open(QFile::ReadOnly))
		{
			QTextStream in(&file2);
			while(!in.atEnd())
			{
				QStringList strValue = in.readLine().remove("\r\n").split("; ");
				int nLeftAnkle = strValue.at(0).toInt();
				int nRightAnkle = strValue.at(1).toInt();
				int nLeftArm = strValue.at(2).toInt();
				int nRightArm = strValue.at(3).toInt();

				int *pPressure = new int[4];
				memset(pPressure, 0x00, sizeof(pPressure[0])*4);
				pPressure[0] = nRightArm;
				pPressure[1] = nLeftArm;
				pPressure[2] = nRightAnkle;
				pPressure[3] = nLeftAnkle;
				
				emit sigRecvPressure(pPressure);

				msleep(5);
			}
		}
		file2.close();
		
	}
	//qDebug()<<"nRCount:"<<nRCount;
	QMutexLocker lock(&m_IsStopMutex);
	m_bIsStop = false;
}

#else 
void QSerialRecvThread::run()
{
	PackageCommon package;
	// 获取每个包的长度及包头信息
	int nPackageLength = package.getPackageLength();
	QString strHeader = package.getPackageHeader();

	static int nRCount = 0;

	//qDebug()<<"m_arrRecvDataSize:"<<m_arrRecvData.size();
	//m_pSerialPort->readAll();
	//qDebug()<<"BeginRecv"<<m_pSerialPort->readAll().toHex();
	while(!m_bIsStop)
	{
		//qDebug()<<"Serial Thread"<<currentThreadId();
		//continue;
		//qDebug()<<"QSerialRecvThread"<<currentThreadId()<<"\t"<<(int)thread();
		if(5 != m_readSema.available())
		{
			m_readSema.release();
		}

		//qDebug()<<"nRCount:1";
		if(!m_dealSema.tryAcquire(1, 2))
		{
			//qDebug()<<"nRCount:2"<<m_readSema.available();
			msleep(5);
			continue;
		}

		//m_dealSema.acquire();
		//m_arrRecvMutex.lock();
		//qDebug()<<"nRCount:22";
		{
			// 内存中已经接受到完整包的个数
			int nRecvCount = m_arrRecvData.size() / nPackageLength;
			if(nRecvCount == 0)
			{
				//m_arrRecvMutex.unlock();
				msleep(1);
				continue;
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
					nRCount += nLength;
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
						int *pPressure = new int[4];
						if(NULL == pPressure)
						{
							continue;
						}
						memset(pPressure, 0x00, sizeof(pPressure[0])*4);
						parse.getPressure(pPressure);
						emit sigSaveData(_formatPressure(pPressure));
						emit sigRecvPressure(pPressure);
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
			
			//qDebug()<<"nRCount:5";
			// 移除处理过的命令
			m_arrRecvData.remove(0, m_arrRecvData.length() - m_arrRecvData.length()%nPackageLength);
			//m_arrRecvMutex.unlock();
		}
	}
	//qDebug()<<"nRCount:"<<nRCount;

	m_arrRecvData.clear();
	
	QMutexLocker lock(&m_IsStopMutex);
	m_bIsStop = false;
}
#endif

void QSerialRecvThread::slotReadData()
{
	qDebug()<<"QSerialRecvThread ReadData"<<currentThreadId()<<"\t"<<(int)thread();
	//qDebug()<<"AAAA"<<m_readSema.available();
	// 未开始之前，不接受数据
	if(!isRunning())
	{
		return;
	}

	if(m_readSema.tryAcquire(1, 200))
	//m_readSema.acquire();
	{
		//qDebug()<<"BBBB"<<m_readSema.available();
		// 读取数据
		m_arrRecvData.append(m_pSerialPort->readAll().toHex());
		m_dealSema.release();
	}
}

bool QSerialRecvThread::initSerialPort()
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
		//QMessageBox::information(NULL, "PortName", info.manufacturer());
		//file.write(info.manufacturer().toLocal8Bit() + "AAAA" + QString::number(timer.elapsed()).toLocal8Bit() + "\r\n");
		////// 仅识别FTDI芯片
		//if(info.manufacturer() != "FTDI")
		//{
		//	continue;
		//}
		//QString strPortName = info.portName();
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
		////以读写方式打开串口
		//m_pSerialPort->setBaudRate(BAUD256000);

		m_pSerialPort->setDataBits(DATA_8);

		////数据位设置，我们设置为8位数据位

		m_pSerialPort->setParity(PAR_NONE);

		////奇偶校验设置，我们设置为无校验

		m_pSerialPort->setStopBits(STOP_1);

		////停止位设置，我们设置为1位停止位

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

void QSerialRecvThread::stop()
{
	// 设置停止
	QMutexLocker lock(&m_IsStopMutex);
	m_bIsStop = true;

	//// 修改应用程序不能迅速从任务管理器中消失的bug
	if(!m_readSema.available())
	{
		m_readSema.release();
	}

	if(!m_dealSema.available())
	{
		m_dealSema.release();
	}
}

void QSerialRecvThread::start()
{
	m_pSerialPort->readAll().toHex();

	//// 设置停止
	QMutexLocker lock(&m_IsStopMutex);
	m_bIsStop = false;
	m_arrRecvData.clear();

	//// 修改应用程序不能迅速从任务管理器中消失的bug
	if(!m_readSema.available())
	{
		m_readSema.release();
	}

	
	if(!m_dealSema.available())
	{
		m_dealSema.release();
	}

	QThread::start();
}

// 设置串口接收数据的个数
void QSerialRecvThread::setReadBufferSize(int nSize)
{
	PackageCommon package;
	//m_pSerialPort->setReadBufferSize(nSize * package.getPackageLength());
	//m_pSerialPort->clear();
}

void QSerialRecvThread::slotSendData(const QString &strMessage)
{

#if _WAVE_ANALYSIS 
	return;
#endif

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

bool QSerialRecvThread::_checkHandle(const QByteArray &byteMsg)
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

QString QSerialRecvThread::_formatPressure(int *pPressure) const
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
