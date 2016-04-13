#include "./header/stdafx.h"
#include <QMutex>
#include "./header/QSavePressure.h"
#include "./header/PackageSendCmd.h"
#include "./header/QSavePressureWorker.h"


QSavePressureWorker::QSavePressureWorker(QObject *pParent)
	:QObject(pParent)
{
	m_pSaveFile = NULL;

	_reset();
}

QSavePressureWorker::~QSavePressureWorker()
{
	if(m_pSaveFile != NULL)
	{
		Q_DELETE(m_pSaveFile);
	}
}

void QSavePressureWorker::setFileName(const QString &strFileName)
{
	QMutexLocker lockFile(&m_filePressure);
	if(m_pSaveFile == NULL)
	{
		m_pSaveFile = new QSavePressure();
	}

	if(strFileName != m_pSaveFile->fileName())
	{
		m_pSaveFile->setFileName(strFileName);
	}
}

QString QSavePressureWorker::getAbsoluteFileName()
{
	QMutexLocker lockFile(&m_filePressure);
	m_pSaveFile->save();
	QString strFileName = m_pSaveFile->getAbsoluteFileName();
	return strFileName;
}

void QSavePressureWorker::savePressure(QVector<int> arrPressure)
{
	if(arrPressure.isEmpty())
	{
		return;
	}

	// 判断是否到了发送二次升压的时刻
	if(!m_isSecondMeasure)
	{
		_isCanSecondBoost(arrPressure);
	}

	QMutexLocker lockFile(&m_filePressure);
	if(m_pSaveFile == NULL)
	{
		m_pSaveFile = new QSavePressure();
	}
	
	QString strValue;
	for(int i = arrPressure.size()-1;i > 0; --i)
	{
		strValue += QString::number(arrPressure.at(i));
		strValue += "; ";
	}
	strValue += QString::number(arrPressure.at(0));
	strValue += "\r\n";
	m_pSaveFile->writeData(strValue);
}

void QSavePressureWorker::stop()
{
	if(m_pSaveFile != NULL)
	{
		m_pSaveFile->save();
	}

	_reset();
}

void QSavePressureWorker::_reset()
{
	memset(m_bArrive200Pressure, 0x00, sizeof(m_bArrive200Pressure)); 	

	//// 判断四路压力值是否到达
	//memset(m_bHasMax, 0x00, sizeof(m_bHasMax));

	// 初始化踝臂能到达的最大值(默认值为200mmHg,考虑到误差，设置为1990)
	m_nArmMaxValue = nDEFAULT_MAX_VALUE;
	m_nAnkleMaxValue = nDEFAULT_MAX_VALUE;
	
	// 是否为第二次测量
	m_isSecondMeasure = false;

	// 二次压力到最大值
	//memset(m_nSecondMaxValue, 0x00 ,sizeof(m_nSecondMaxValue));
}

void QSavePressureWorker::_isCanSecondBoost(QVector<int> arrPressure)
{
	bool bHasLessThan70[4] = {};
	for (int j = 0; j < 4; ++j)
	{
		int nCurrValue = arrPressure.at(j);
		// 处理大于200mmHg和小于70mmHg
		if(nCurrValue >= nDEFAULT_MAX_VALUE && !m_bArrive200Pressure[j])
		{
			m_bArrive200Pressure[j] = true;
		}
		else if(nCurrValue/10 <= nDEFAULT_MIN_VALUE)
		{
			bHasLessThan70[j] = true;
		}
	}

	bool bHasAllArrive200 = true;
	bool bLessThan70 = true;

	// 判断是否到了第二次升压的时刻
	for (int i =0 ;i<4; ++i)
	{
		// 未到过最高压
		if(!m_bArrive200Pressure[i] && bHasAllArrive200)
		{
			bHasAllArrive200 = false;
			break;
		}

		// 未降到最低压
		if(!bHasLessThan70[i] && bLessThan70)
		{
			bLessThan70 = false;
			break;
		}
	}

	// 第一次测量结束的标志就是四肢压力上升过200，现在又降回到70(考虑到误差，现在降到75)
	if(bHasAllArrive200 && bLessThan70)
	{
		//qDebug()<<"AAAAA"<<timer.elapsed();
		memset(m_bArrive200Pressure, 0x00, sizeof(m_bArrive200Pressure));
		_sendSecondBoostCmd();
#if !_WAVE_ANALYSIS
		m_isSecondMeasure = true;
#endif
	}
}

void QSavePressureWorker::_sendSecondBoostCmd()
{
	PackageSendCmd sendCmd;
	QString strMessage;

	if(m_pSaveFile == NULL)
	{
		return;
	}
	
	m_pSaveFile->save();
	QString strFileName = m_pSaveFile->getAbsoluteFileName();
	int nLastIndex = strFileName.indexOf("_2.txt");
	if(nLastIndex == -1)
	{
		int nSize = strFileName.lastIndexOf("_");
		QString strNewFileName = strFileName.left(nSize);
		strNewFileName += "_2.txt";
		setFileName(strNewFileName);
	}

	// TODO:调用DLL算法，分析第一次测量结果
	// TODO：这一段很恶心，有时间修改
	QString strTemp = strFileName;
	strTemp.replace(QString("/"), QString("\\\\"));

	QString strDir = strTemp.left(strTemp.lastIndexOf("\\\\") + 2);

	QByteArray baFileName = strTemp.toLocal8Bit();
	char *pFileName = baFileName.data();

	QByteArray baFileDir = strDir.toLocal8Bit();
	char *pFileDir = baFileDir.data();


	double dAnkle_left_dingdian = 0;
	double dAnkle_right_dingdian = 0;
	double dBrachial_left_dingdian = 0;
	double dBrachial_right_dingdian = 0;

	//suanfa1(pFileName, pFileDir, &dAnkle_left_dingdian, &dAnkle_right_dingdian,
	//	&dBrachial_left_dingdian, &dBrachial_right_dingdian);
	//qDebug()<<dAnkle_left_dingdian<<"\t"<<dAnkle_right_dingdian<<"\t"<<dBrachial_left_dingdian<<
	//	""<<dBrachial_right_dingdian;
	int nArm = qMax(dBrachial_right_dingdian, dBrachial_left_dingdian)/10;
	int nAnkle = qMax(dAnkle_left_dingdian, dAnkle_right_dingdian)/10;
	
	if(nArm<10)
	{
		nArm = nMIN_ARM_PRESSURE;
	}

	if(nAnkle<10)
	{
		nAnkle = nMIN_ANKLE_VALUE;
	}

	strMessage  = sendCmd.getBoostMeasureCmd(nArm, nArm,
		nAnkle, nAnkle);

	emit sigSecondBoostCmd(strMessage);
	m_nArmMaxValue = nArm*100 - 10;
	m_nAnkleMaxValue = nAnkle*100 - 10;

	emit sigResetMaxValue(m_nArmMaxValue, m_nAnkleMaxValue);
}
