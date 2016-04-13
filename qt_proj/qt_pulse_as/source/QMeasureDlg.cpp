#include "./header/stdafx.h"
//#include "QSerialRecvThread.h"
#include "./header/QDrawWaveWidget.h"
#include "./header/packagecommon.h"
#include "./header/PackageRecvCmd.h"
#include "./header/PackageSendCmd.h"
#include "./header/QPersonBasicInfoDlg.h"
#include "./header/QSavePressure.h"
#include "./header/QMeasureDlg.h"
//#include "QSavePressureThread.h"
#include "./header/QSavePressureWorker.h"
#include "./header/QPackageReceiveWorker.h"
#include "./header/QDrawWaveWorker.h"
#include "./header/QFilterData.h"

// 引入DLL
#ifdef __cplusplus
extern "C"
{
#include "fenlishuju_and_lvbo_1.h"
}
#else
#include "fenlishuju_and_lvbo_1.h"
#endif

QMeasureDlg::QMeasureDlg(QWidget *pParent)
	:QWidget(pParent)
{
	qRegisterMetaType<QVector<int>>("QVector<int>");
	qRegisterMetaType<QVector<float>>("QVector<float>");
	qRegisterMetaType<QVector<float> *>("QVector<float> *");
	_initUI();

	//m_pSavePressure = NULL;

	//memset(m_bArrive200Pressure, 0x00, sizeof(m_bArrive200Pressure));
	//memset(m_bHasMax, 0x00, sizeof(m_bHasMax));
	//m_bLArmHasMax = false;
	//m_bRArmHasMax = false;
	//m_bLAnkleHasMax = false;
	//m_bRAnkleHasMax = false;

	//memset(m_nSecondMaxValue,0x00,sizeof(m_nSecondMaxValue));

	// 初始化踝臂能到达的最大值(默认值为200mmHg,考虑到误差，设置为1990)
	//m_nArmMaxValue = 1990;
	//m_nAnkleMaxValue = 1990;
}

QMeasureDlg::~QMeasureDlg(void)
{
	Q_DELETE(m_pShowStatus);
	Q_DELETE(m_pbtnControlMeasure);
	Q_DELETE(m_pDrawWaveWidget);
	Q_DELETE(m_pMainLayout);

	//m_pRecvThread->stop();
	//m_pRecvThread->wait();
	//Q_DELETE(m_pRecvThread);

	//if(m_pSaveFileThread != NULL)
	//{
	//	m_pSaveFileThread->stop();
	//	m_pSaveFileThread->wait();
	//	Q_DELETE(m_pSaveFileThread);
	//}
}

// 初始化窗口
void QMeasureDlg::_initUI()
{
	// 当前测量信息
	m_pShowStatus = new QLabel();
	m_pShowStatus->setText(QString::fromLocal8Bit("当前状态:"));

	// 描绘波形区域
	m_pDrawWaveWidget = new QDrawWaveWidget(this);
	m_pDrawWaveWidget->resize(this->size());

	// 控制测量按钮
	m_pbtnControlMeasure = new QPushButton(QString::fromLocal8Bit("开始"), this);
	QFont font(QStringLiteral("宋体"), 30);
	font.setBold(true);
	QFontMetrics fontMetrics(font, this);
	m_pbtnControlMeasure->setFont(font);
	int nTextWidth = fontMetrics.width(m_pbtnControlMeasure->text());

	m_pbtnControlMeasure->setMinimumWidth(3*nTextWidth);

	m_pbtnControlMeasure->setFixedHeight(80);
	QIcon icon(":/AppIcon/Images/AppIcon.png");
	QString strStyle = QString("border-color:rgb(238,246,252);background: rgb(216,229,242);border-radius: %1px;border-width: 2px;border-style: solid;").arg(m_pbtnControlMeasure->height()/4);
	m_pbtnControlMeasure->setStyleSheet(strStyle);

	m_pMainLayout = new QGridLayout(this);
	m_pMainLayout->setSpacing(6);
	m_pMainLayout->setMargin(3);

	m_pMainLayout->addWidget(m_pShowStatus, 0, 0, 1, 1, Qt::AlignLeft);
	m_pMainLayout->setRowStretch(0, 0);

	m_pMainLayout->addWidget(m_pDrawWaveWidget, 1, 0, 1, 1);
	m_pMainLayout->setRowStretch(1, 8);

	m_pMainLayout->addWidget(m_pbtnControlMeasure, 2, 0, 1, 1, Qt::AlignCenter);
	m_pMainLayout->setRowStretch(2, 1);

	// 通过串口接收数据的线程
	m_pRecvWorkerThread = new QThread();
	m_pRecvWorker = new QPackageReceiveWorker();
	m_pRecvWorker->initSerialPort();
	m_pRecvWorker->moveToThread(m_pRecvWorkerThread);
	
	connect(m_pbtnControlMeasure, SIGNAL(clicked()), this, SLOT(slotControlMeasure()));
	connect(this, SIGNAL(sigSendMessage(const QString &)), m_pRecvWorker, SLOT(slotSendData(const QString &)));
	connect(m_pRecvWorker, SIGNAL(sigRecvCmd(short, short)), this, SLOT(slotRecvCmd(short, short)));

	m_pSaveFileWorker = new QSavePressureWorker();
	m_pSaveFileWorkerThread = new QThread();

	m_pSaveFileWorker->moveToThread(m_pSaveFileWorkerThread);
	connect(m_pRecvWorker, SIGNAL(sigRecvPressure(QVector<int>)), m_pSaveFileWorker, SLOT(savePressure(QVector<int>)));

	//connect(m_pSaveFileWorker, SIGNAL(sigSaveData(const QString &)), m_pSaveFileThread, SLOT(savePressure(const QString &)));
	connect(m_pSaveFileWorker, SIGNAL(sigSecondBoostCmd(const QString &)), m_pRecvWorker, SLOT(slotSendData(const QString &)));

	//m_isSecondMeasure = false;

    m_pDrawWaveWorker = new QDrawWaveWorker();
	m_pDrawWaveThread = new QThread();
	m_pDrawWaveWorker->moveToThread(m_pDrawWaveThread);
	
	connect(m_pRecvWorker, SIGNAL(sigRecvPressure(QVector<int>)), m_pDrawWaveWorker, SLOT(slotFiterWave(QVector<int>)));
	connect(m_pDrawWaveWorker, SIGNAL(sigUpdateWavePressure(QVector<float> *, QVector<int>, bool)),
	  m_pDrawWaveWidget, SLOT(appendData(QVector<float> *, QVector<int>, bool)));
	connect(m_pSaveFileWorker, SIGNAL(sigResetMaxValue(int, int)), m_pDrawWaveWorker, SLOT(slotResetMaxValue(int,int)));

	setLayout(m_pMainLayout);
}

void QMeasureDlg::slotControlMeasure()
{
	if(NULL == m_pPersonInfo)
	{
		return;
	}

	static QString strPreName;
	// 开始按钮的话，需要判断患者信息是否已经填写完整
	if(m_pbtnControlMeasure->text().contains(QString::fromLocal8Bit("开始")))
	{
		// 首先，判断填入的患者信息是否正确
		if (!m_pPersonInfo->isValidInput())
		{
			return;
		}

		QString strCurrName = m_pPersonInfo->getPersonName();
		if (0 == strCurrName.compare(strPreName))
		{
			QMessageBox::StandardButton ret =  QMessageBox::question(this, QString::fromLocal8Bit("患者信息"),
				QString::fromLocal8Bit("患者基本信息没有变化，是否继续测量?"),
				QMessageBox::Yes|QMessageBox::No);
			if(ret == QMessageBox::No)
			{
				m_pPersonInfo->resetInfo();
				return;
			}
		}
		strPreName  =  strCurrName;
	}

	// 设置接受线程的状态
	if(m_pbtnControlMeasure->text().contains(QString::fromLocal8Bit("开始")))
	{
#if !_WAVE_ANALYSIS 
		_initSavePressure();
#endif

		_startOrEndMeasure(true);
		m_pShowStatus->setText(QString::fromLocal8Bit("当前状态:准备测量..."));
	}
	else
	{
		//m_filter.getValleyPeak();
		_startOrEndMeasure(false);
		//m_pSavePressure->save();
		m_pShowStatus->setText(QString::fromLocal8Bit("当前状态:停止测量..."));
	}
}

void QMeasureDlg::slotRecvMessage(QByteArray *pRecvMessage)
{
	//if(NULL == pRecvMessage)
	//{
	//	return;
	//}
	////qDebug()<<*(pRecvMessage);
	//PackageCommon packageInfo;
	//int nPackageLength = packageInfo.getPackageLength();

	//for(int nIndex = 0; nIndex < pRecvMessage->size();)
	//{
	//	ParseRecvCmd parse(pRecvMessage->mid(nIndex, nPackageLength));
	//	// 检查消息长度、头、尾、校验码是否正确
	//	if(!parse.isLengthOk() || !parse.isHeaderOk()
	//		|| !parse.isTailOk() || !parse.isSumOk())
	//	{
	//		//qDebug()<<"Delete";
	//		int nPreIndex = nIndex;
	//		nIndex = pRecvMessage->indexOf(packageInfo.getPackageHeader(), nIndex + 1);
	//		if(nIndex == -1)
	//		{
	//			break;
	//		}
	//		else
	//		{
	//			continue;
	//		}
	//	}

	//	ParseRecvCmd::RecvType eType = parse.recvDataType();

	//	// 测量结束的标志
	//	bool bIsEndFlag = false;

	//	// 根据不同的命令，做出不同的相应
	//	switch (eType)
	//	{
	//		// 收到测量数据
	//	case ParseRecvCmd::eRECV_DATA:
	//		m_pShowStatus->setText(QString::fromLocal8Bit("当前状态:测量中..."));
	//		{
	//			int arrPressure[4] = {};
	//			parse.getPressure(arrPressure);


	//			m_pDrawWaveWidget->appendData(arrPressure);

	//			bool bHasLessThan70[4] = {};
	//			//// 第二步:保存测量数据
	//			//if(m_pSavePressure != NULL)
	//			//{
	//			// 获取测量值
	//			QString strValue;
	//			for (int j = 0; j < 4; ++j)
	//			{
	//				int nCurrValue = *(arrPressure + 3 - j);
	//				// 处理大于200mmHg和小于70mmHg
	//				if(nCurrValue > 1990 && !m_bArrive200Pressure[j])
	//				{
	//					m_bArrive200Pressure[j] = true;
	//				}
	//				else if(nCurrValue/10 <= 70 )
	//				{
	//					bHasLessThan70[j] = true;
	//				}

	//				strValue += QString::number(nCurrValue);

	//				if(j + 1 < 4)
	//				{
	//					strValue += "; ";
	//				}
	//			}
	//			strValue += "\r\n";
	//			//m_pSavePressure->writeData(strValue);
	//			emit sigSaveThread(strValue);
	//			//}

	//			bool bHasAllArrive200 = true;
	//			bool bLessThan70 = true;
	//			// 判断是否到了第二次升压的时刻
	//			for(int i =0 ;i<4; ++i)
	//			{
	//				// 未到过最高压
	//				if(!m_bArrive200Pressure[i] && bHasAllArrive200)
	//				{
	//					bHasAllArrive200 = false;
	//					break;
	//				}

	//				// 未降到最低压
	//				if(!bHasLessThan70[i] && bLessThan70)
	//				{
	//					bLessThan70 = false;
	//					break;
	//				}
	//			}

	//			// 第一次测量结束的标志就是四肢压力上升过200，现在又降回到70
	//			if(bHasAllArrive200 && bLessThan70)
	//			{
	//				_sendSecondBoostCmd();
	//				bIsEndFlag = true;
	//			}

	//			// 判断是否结束
	//			if(arrPressure[0]/10 <= 40 && arrPressure[1]/10 <= 40 &&
	//				arrPressure[2]/10 <= 40 && arrPressure[3]/10 <= 40 && timer.hasExpired(150*1000))
	//			{
	//				bIsEndFlag = true;
	//				// 停止测量
	//				_startOrEndMeasure(false);

	//				// 更新测量状态
	//				m_pShowStatus->setText(QString::fromLocal8Bit("当前状态:测量结束..."));
	//				//m_pSavePressure->save();

	//				//emit sigRecvTestEndCmd(m_pSavePressure->getAbsoluteFileName(), m_pPersonInfo->getPersonInfo());
	//				emit sigRecvTestEndCmd(m_pSaveFileWorker->getAbsoluteFileName(), m_pPersonInfo->getPersonInfo());
	//				pRecvMessage->resize(0);
	//				Q_DELETE(pRecvMessage);
	//				return;
	//			}
	//		}
	//		break;

	//		// 收到测试结束
	//	case ParseRecvCmd::eSend_TEST_CMD:
	//		if (parse.isNormalEndCmd())
	//		{
	//			// 停止测量
	//			_startOrEndMeasure(false);

	//			m_pShowStatus->setText(QString::fromLocal8Bit("当前状态:测量结束..."));
	//			//m_pSavePressure->save();

	//			//emit sigRecvTestEndCmd(m_pSavePressure->getAbsoluteFileName(), m_pPersonInfo->getPersonInfo());
	//			emit sigRecvTestEndCmd(m_pSaveFileWorker->getAbsoluteFileName(), m_pPersonInfo->getPersonInfo());
	//			bIsEndFlag = true;
	//		}
	//		else if (parse.isNormalStartCmd())
	//		{
	//			m_pShowStatus->setText(QString::fromLocal8Bit("当前状态:测量开始..."));
	//			break;
	//		}
	//		break;

	//		// 收到正常测试反馈信息
	//	case ParseRecvCmd::eRECV_TEST_END_CMD:
	//		{
	//			switch(parse.getStatus())
	//			{
	//				// 启动状态
	//			case ParseRecvCmd::eStart_Status:
	//				{
	//					m_pShowStatus->setText(QString::fromLocal8Bit("当前状态:测量开始..."));
	//				}
	//				break;
	//				// 停止状态
	//			case ParseRecvCmd::eStop_Status:
	//				{
	//					m_pShowStatus->setText(QString::fromLocal8Bit("当前状态:停止测量..."));
	//					_startOrEndMeasure(false);
	//				}
	//				break;
	//				// 二次升压状态
	//			case ParseRecvCmd::eSecond_BoostStatus:
	//				{
	//					// 重置标志
	//					memset(m_bArrive200Pressure, 0x00, sizeof(m_bArrive200Pressure));

	//					m_pShowStatus->setText(QString::fromLocal8Bit("当前状态:准备第二次测量..."));
	//				}
	//				break;
	//			default:
	//				break;
	//			}
	//		}
	//		break;
	//	default:
	//		break;
	//	}

	//	// 测量结束，直接跳出
	//	if(bIsEndFlag)
	//	{
	//		break;
	//	}

	//	nIndex += nPackageLength;
	//}

	//pRecvMessage->resize(0);
	//Q_DELETE(pRecvMessage);
}

void QMeasureDlg::slotRecvData(int *pPressure)
{
	////qDebug()<<"SlotRecv :"<<this->thread()->currentThreadId()<<"\t"<<*pPressure;
	//if(NULL == pPressure)
	//{
	//	return;
	//}

	//// 存储原始压力值,依次存放右臂、左臂、右腿、左腿
	//static QVector<int> vectOriginal[4];

	//// 存储处理后的压力值,依次存放右臂、左臂、右腿、左腿
	//static QVector<float> vectAnalysisPressure[4];

	//static QElapsedTimer tempTimer;
	//if(!tempTimer.isValid())
	//{
	//	tempTimer.start();
	//}

	////qDebug()<<"Timer1"<<tempTimer.elapsed();
	////bool bIsSecondMeasure = true;
	////for(int i = 0; i<4; ++i)
	////{
	////	if(!m_bArrive200Pressure[i])
	////	{
	////		bIsSecondMeasure = false;
	////		break;
	////	}
	////}

	//// 判断是否到了发送二次升压的时刻
	//if(!m_isSecondMeasure)
	//{
	//	_sendSecondBoostPressure(pPressure);
	//}
	//
	//// 判断是否到了最大值时刻
	//{
	//	//qDebug()<<"AA\t"<<pPressure[0]<<"\t"<<pPressure[1]<<"\t"<<pPressure[2]<<"\t"<<pPressure[3];
	//	// 右臂
	//	if(pPressure[0] >= m_nArmMaxValue)
	//	{
	//		m_bHasMax[0] = true;
	//	}
	//	
	//	if(pPressure[0] >= m_nSecondMaxValue[0])
	//	{
	//		m_nSecondMaxValue[0] = pPressure[0];
	//	}

	//	// 左臂
	//	if(pPressure[1] >= m_nArmMaxValue)
	//	{
	//		m_bHasMax[1] = true;
	//	}
	//	
	//	if(pPressure[1] >= m_nSecondMaxValue[1])
	//	{
	//		m_nSecondMaxValue[1] = pPressure[1];
	//	}
	//	
	//	// 右踝
	//	if(pPressure[2] >= m_nAnkleMaxValue)
	//	{
	//		m_bHasMax[2] = true;
	//	}
	//	
	//	if(pPressure[2] >= m_nSecondMaxValue[2])
	//	{
	//		m_nSecondMaxValue[2] = pPressure[2];
	//	}
	//	
	//	// 左踝
	//	if(pPressure[3] >= m_nAnkleMaxValue)
	//	{
	//		m_bHasMax[3] = true;
	//	}

	//	if(pPressure[3] >= m_nSecondMaxValue[3])
	//	{
	//		m_nSecondMaxValue[3] = pPressure[3];
	//	}
	//}

	//if(m_bHasMax[0] && m_bHasMax[1] && m_bHasMax[2] && m_bHasMax[3])
	//{
	//	m_filter.addReceiveData(pPressure);
	//}
	//else
	//{
	//	m_filter.reset();
	//}

	//static bool bHasDraw = false;
	//float fDealPressure[4] = {};
	//QVector<float> arrDrawWave = m_filter.getDrawValue();
	//return;
	//if(arrDrawWave.size()<4)
	//{
	//	bHasDraw = false;
	//	m_pDrawWaveWidget->appendData(fDealPressure, pPressure, bHasDraw);
	//	delete pPressure;
	//	pPressure = NULL;
	//	return;
	//}

	//bHasDraw = true;
	//for(int i = 0; i <4; ++i)
	//{
	//	if(arrDrawWave.size()>i)
	//	{
	//		fDealPressure[i] = arrDrawWave.at(i);
	//	}
	//}

	//m_pDrawWaveWidget->appendData(fDealPressure, pPressure);
	//delete pPressure;
	//pPressure = NULL;
}

void QMeasureDlg::slotRecvData(QVector<int> arrPressure)
{
	//static int nCount = 0;
	////qDebug()<<"SlotRecv :"<<this->thread()->currentThreadId()<<"\t"<<*pPressure;
	//if(arrPressure.isEmpty())
	//{
	//	return;
	//}

	//// 判断是否到了发送二次升压的时刻
	//if(!m_isSecondMeasure)
	//{
	//	_sendSecondBoostPressure(pPressure);
	//}
	//
	//// 判断是否到了最大值时刻
	//{
	//	//qDebug()<<"AA\t"<<pPressure[0]<<"\t"<<pPressure[1]<<"\t"<<pPressure[2]<<"\t"<<pPressure[3];
	//	// 右臂
	//	if(pPressure[0] >= m_nArmMaxValue)
	//	{
	//		m_bHasMax[0] = true;
	//	}
	//	
	//	if(pPressure[0] >= m_nSecondMaxValue[0])
	//	{
	//		m_nSecondMaxValue[0] = pPressure[0];
	//	}

	//	// 左臂
	//	if(pPressure[1] >= m_nArmMaxValue)
	//	{
	//		m_bHasMax[1] = true;
	//	}
	//	
	//	if(pPressure[1] >= m_nSecondMaxValue[1])
	//	{
	//		m_nSecondMaxValue[1] = pPressure[1];
	//	}
	//	
	//	// 右踝
	//	if(pPressure[2] >= m_nAnkleMaxValue)
	//	{
	//		m_bHasMax[2] = true;
	//	}
	//	
	//	if(pPressure[2] >= m_nSecondMaxValue[2])
	//	{
	//		m_nSecondMaxValue[2] = pPressure[2];
	//	}
	//	
	//	// 左踝
	//	if(pPressure[3] >= m_nAnkleMaxValue)
	//	{
	//		m_bHasMax[3] = true;
	//	}

	//	if(pPressure[3] >= m_nSecondMaxValue[3])
	//	{
	//		m_nSecondMaxValue[3] = pPressure[3];
	//	}
	//}
	//
	//
	////qDebug()<<"Timer2"<<tempTimer.elapsed();
	//if(m_bHasMax[0] && m_bHasMax[1] && m_bHasMax[2] && m_bHasMax[3])
	//{
	//	m_filter.addReceiveData(pPressure);
	//}
	//else
	//{
	//	m_filter.reset();
	//}
	////qDebug()<<"Timer3"<<tempTimer.elapsed();
	//
	////QApplication::processEvents();

	//static bool bHasDraw = false;
	//float fDealPressure[4] = {};
	//QVector<float> arrDrawWave = m_filter.getDrawValue();
	//return;
	//if(arrDrawWave.size()<4)
	//{
	//	bHasDraw = false;
	//	m_pDrawWaveWidget->appendData(fDealPressure, pPressure, bHasDraw);
	//	delete pPressure;
	//	pPressure = NULL;
	//	return;
	//}

	//bHasDraw = true;
	//for(int i = 0; i <4; ++i)
	//{
	//	if(arrDrawWave.size()>i)
	//	{
	//		fDealPressure[i] = arrDrawWave.at(i);
	//	}
	//}

	//m_pDrawWaveWidget->appendData(fDealPressure, pPressure);
	//delete pPressure;
	//pPressure = NULL;
}

void QMeasureDlg::slotUpdateWaveAndPressure(QVector<float> arrDrawWave, QVector<int> arrPressure, bool bOnlyUpdatePressure)
{
	//m_pDrawWaveWidget->appendData(arrDrawWave, arrPressure, bOnlyUpdatePressure);
}

void QMeasureDlg::slotRecvCmd(short nCmdType, short nStatus)
{
	switch (nCmdType)
	{
		// 收到测试结束
	case ParseRecvCmd::eSend_TEST_CMD:
		switch(nStatus)
		{
			// 测量结束
		case ParseRecvCmd::eTest_End:
			{
				// 停止测量
				_startOrEndMeasure(false);

				m_pShowStatus->setText(QString::fromLocal8Bit("当前状态:测量结束..."));
			    //m_nSecondMaxValue[0]

				emit sigRecvTestEndCmd(m_pSaveFileWorker->getAbsoluteFileName(), m_pPersonInfo->getPersonInfo());
				//QString strRet = QString::fromLocal8Bit("右臂:%1\n左臂:%2\n右踝:%3\n左踝:%4")
				//	.arg(m_nSecondMaxValue[0]).arg(m_nSecondMaxValue[1]).arg(m_nSecondMaxValue[2]).arg(m_nSecondMaxValue[3]);
				//QMessageBox::information(this, QString::fromLocal8Bit("最大值"), strRet);
			}
			break;

			// 测量开始
		case ParseRecvCmd::eTest_Start:
			{
				m_pShowStatus->setText(QString::fromLocal8Bit("当前状态:测量开始..."));
			}
		default:
			break;
		}
		break;
		// 收到正常测试反馈信息
	case ParseRecvCmd::eRECV_TEST_END_CMD:
		{
			switch(nStatus)
			{
				// 启动状态
			case ParseRecvCmd::eStart_Status:
				{
					m_pShowStatus->setText(QString::fromLocal8Bit("当前状态:测量开始..."));
				}
				break;

				// 停止状态
			case ParseRecvCmd::eStop_Status:
				{
					m_pShowStatus->setText(QString::fromLocal8Bit("当前状态:停止测量..."));
					_startOrEndMeasure(false);
				}
				break;

			// 二次升压状态
			case ParseRecvCmd::eSecond_BoostStatus:
				{
					m_pShowStatus->setText(QString::fromLocal8Bit("当前状态:准备第二次测量..."));
				}
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
}

void QMeasureDlg::_startOrEndMeasure(bool bStart)
{
	PackageSendCmd sendCmd;
	QString strMessage;
	if(bStart)
	{
		//// 重置最大值
		//memset(m_bHasMax, 0x00, sizeof(m_bHasMax));
		//memset(m_nSecondMaxValue,0x00,sizeof(m_nSecondMaxValue));

		// 启动数据接收线程
		m_pRecvWorkerThread->start();

		// 启动保存数据线程
		m_pSaveFileWorkerThread->start();

		// 启动波形描绘线程
		m_pDrawWaveThread->start();

		m_pRecvWorker->start();

		m_pDrawWaveWorker->start();
		//// 开始计时
		//timer.start();

		// 更新按钮的文字
		m_pbtnControlMeasure->setText(QString::fromLocal8Bit("停止"));

		// 描画窗口开始描绘
		m_pDrawWaveWidget->startMeasure();

		// 获取开始命令
		strMessage = sendCmd.getNoramlStart();

		//// 重新测量标志
		//memset(m_bArrive200Pressure, 0x00, sizeof(m_bArrive200Pressure));

		//m_isSecondMeasure = false;
	}
	else
	{
		// 停止数据接收线程
		//m_pRecvWorkerThread->terminate();
		// 停止接受数据
		m_pRecvWorker->stop();

		// 停止保存数据
		m_pSaveFileWorker->stop();

		m_pDrawWaveWorker->stop();
		// 停止保存文件
		//m_pSaveFileWorkerThread->terminate();

		//// 计时器失效
		//timer.invalidate();

		// 更新按钮文字状态
		m_pbtnControlMeasure->setText(QString::fromLocal8Bit("开始"));

		// 更新测量状态
		m_pShowStatus->setText(QString::fromLocal8Bit("当前状态:停止测量..."));

		// 描画窗口停止描绘
		m_pDrawWaveWidget->stopMeasure();

		// 获取停止命令
		strMessage = sendCmd.getNomalEnd();
	}

	//// 初始化踝臂能到达的最大值(默认值为200mmHg,考虑到误差，设置为1990)
	//m_nArmMaxValue = 1990;
	//m_nAnkleMaxValue = 1990;

	emit sigSendMessage(strMessage);
}

void QMeasureDlg::setPersonInfo(QPersonBasicInfoDlg *pPersonInfoDlg)
{
	m_pPersonInfo = pPersonInfoDlg;
}


void QMeasureDlg::_initSavePressure()
{
	QString strFileName = m_pPersonInfo->getPersonInfo().m_strNo;
	strFileName += "(";
	strFileName += m_pPersonInfo->getPersonInfo().m_strName + ")_1.txt";
	//if (m_pSavePressure == NULL)
	//{
	//	m_pSavePressure = new QSavePressure(strFileName);
	//}

	//Q_ASSERT(m_pSavePressure != NULL);

	//if(m_pSavePressure->fileName() != strFileName)
	//{
	//	m_pSavePressure->save();
	//	m_pSavePressure->setFileName(strFileName);
	//}

	//if (m_pSaveFileWorker == NULL)
	//{
	//	m_pSaveFileWorker = new QSavePressureThread();
	//}

	//Q_ASSERT(m_pSaveFileThread != NULL);

	//if(m_pSavePressure->fileName() != strFileName)
	//{
	//	m_pSavePressure->save();
	//	m_pSavePressure->setFileName(strFileName);
	//}
	m_pSaveFileWorker->setFileName(strFileName);
}

//bool QMeasureDlg::_isNoLoad(int *arrPressure)
//{
//	bool bRet = false;
//	if (arrPressure == NULL)
//	{
//		return bRet;
//	}
//
//	// 开始到现在已经耗时3秒钟
//	if(timer.hasExpired(3000))
//	{
//		// 判断是否有小于30mmHg的值
//		for(int i = 0; i < 4; ++i)
//		{
//			if(*(arrPressure+i) < 300)
//			{
//				bRet = true;
//				break;
//			}
//		}
//	}
//
//	return bRet;
//}

void QMeasureDlg::_sendSecondBoostCmd()
{
	//PackageSendCmd sendCmd;
	//QString strMessage;

	//// TODO:调用DLL算法，分析第一次测量结果
	//// TODO：这一段很恶心，有时间修改
	//QString strTemp = m_pSaveFileWorker->getAbsoluteFileName();
	//strTemp = QString::fromLocal8Bit("E:/Lepu_20150810/LepuMedical/SvnFile/MeasureArteriosclerosis(VS)/MeasureArteriosclerosis/MeasureArteriosclerosis/Data/1(wanghanbo)_1.txt");
	//strTemp.replace(QString("/"),QString("\\\\"));

	//QString strDir = strTemp.left(strTemp.lastIndexOf("\\\\") + 2);

	//QByteArray baFileName = strTemp.toLocal8Bit();
	//char *pFileName = baFileName.data();

	//QByteArray baFileDir = strDir.toLocal8Bit();
	//char *pFileDir = baFileDir.data();


	//double dAnkle_left_dingdian = 0.0;
	//double dAnkle_right_dingdian = 0.0;
	//double dBrachial_left_dingdian = 0.0;
	//double dBrachial_right_dingdian = 0.0;

	////suanfa1(pFileName, pFileDir, &dAnkle_left_dingdian, &dAnkle_right_dingdian,
	////	&dBrachial_left_dingdian, &dBrachial_right_dingdian);
	////qDebug()<<dAnkle_left_dingdian<<"\t"<<dAnkle_right_dingdian<<"\t"<<dBrachial_left_dingdian<<
	////	""<<dBrachial_right_dingdian;
	//int nArm = qMax(dBrachial_right_dingdian, dBrachial_left_dingdian)/10;
	//int nAnkle = qMax(dAnkle_left_dingdian, dAnkle_right_dingdian)/10;
	//
	//if(nArm<10)
	//{
	//	nArm = 20;
	//}

	//if(nAnkle<10)
	//{
	//	nAnkle = 22;
	//}

	////qDebug()<<nArm<<"\t"<<nAnkle;

	//strMessage  = sendCmd.getBoostMeasureCmd(nArm, nArm,
	//	nAnkle, nAnkle);

	//emit sigSendMessage(strMessage);
	////qDebug()<<strMessage;
	//m_nArmMaxValue = nArm*100 - 10;
	//m_nAnkleMaxValue = nAnkle*100 - 10;

	//// 重置最大值
	//memset(m_bHasMax, 0x00, sizeof(m_bHasMax));
	//memset(m_nSecondMaxValue,0x00,sizeof(m_nSecondMaxValue));
}

void QMeasureDlg::_sendSecondBoostPressure(int *arrPressure)
{
//	//if(!m_timerSecondStart.hasExpired(5*1000))
//	//{
//	//	m_timerSecondStart.start();
//	//	return;
//	//}
//	if(NULL == arrPressure)
//	{
//		return;
//	}
//
//#if _WAVE_ANALYSIS
//	//if (m_isSecondMeasure)
//	//{
//		return;
//	//}
//#endif
//
//	bool bHasLessThan70[4] = {};
//	for (int j = 0; j < 4; ++j)
//	{
//		int nCurrValue = *(arrPressure + 3 - j);
//		// 处理大于200mmHg和小于70mmHg
//		if(nCurrValue > 1990 && !m_bArrive200Pressure[j])
//		{
//			m_bArrive200Pressure[j] = true;
//		}
//		else if(nCurrValue/10 <= 75)
//		{
//			bHasLessThan70[j] = true;
//		}
//	}
//
//	bool bHasAllArrive200 = true;
//	bool bLessThan70 = true;
//
//	// 判断是否到了第二次升压的时刻
//	for (int i =0 ;i<4; ++i)
//	{
//		// 未到过最高压
//		if(!m_bArrive200Pressure[i] && bHasAllArrive200)
//		{
//			bHasAllArrive200 = false;
//			break;
//		}
//
//		// 未降到最低压
//		if(!bHasLessThan70[i] && bLessThan70)
//		{
//			bLessThan70 = false;
//			break;
//		}
//	}
//
//	// 第一次测量结束的标志就是四肢压力上升过200，现在又降回到70(考虑到误差，现在降到75)
//	if(bHasAllArrive200 && bLessThan70)
//	{
//		//qDebug()<<"AAAAA"<<timer.elapsed();
//		memset(m_bArrive200Pressure, 0x00, sizeof(m_bArrive200Pressure));
//		_sendSecondBoostCmd();
//#if !_WAVE_ANALYSIS
//		m_isSecondMeasure = true;
//#endif
//	}
}
