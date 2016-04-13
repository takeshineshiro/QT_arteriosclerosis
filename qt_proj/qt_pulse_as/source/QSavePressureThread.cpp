#include "./header/stdafx.h"
#include <QMutex>
#include "./header/QSavePressure.h"
#include "./header/QSavePressureThread.h"


QSavePressureThread::QSavePressureThread(QObject *pParent):
	QThread(pParent),m_isStop(false)
{
	m_pSaveFile = NULL;
}

QSavePressureThread::~QSavePressureThread(void)
{
	stop();
	delete m_pSaveFile;
	m_pSaveFile = NULL;
}

void QSavePressureThread::run()
{
	while(!m_isStop)
	{
		QMutexLocker lockPressure(&m_pressureMutex);

		qDebug()<<"QSavePressureThread"<<currentThreadId()<<"\t"<<(int)thread();
		//qDebug()<<"SavePressure Thread"<<currentThreadId();
		if(m_arrPressure.isEmpty())
		{
			lockPressure.unlock();
			//msleep(50);
			continue;
		}

		QMutexLocker lockFile(&m_filePressure);
		if(m_pSaveFile == NULL)
		{
			m_pSaveFile = new QSavePressure();
		}
		for(int i =0 ;i<m_arrPressure.size(); ++i)
		{
			m_pSaveFile->writeData(m_arrPressure.at(i));
		}
		m_arrPressure.clear();
	}
	QMutexLocker lock(&m_stopMutex);
	m_isStop = false;

	QMutexLocker lockFile(&m_filePressure);
	if(!m_arrPressure.isEmpty())
	{
		for(int i =0 ;i<m_arrPressure.size(); ++i)
		{
			m_pSaveFile->writeData(m_arrPressure.at(i));
		}
		m_arrPressure.clear();
	}

	if(NULL != m_pSaveFile)
	{
		m_pSaveFile->save();
	}
}

void QSavePressureThread::savePressure(const QString & strPressure)
{
	//// 获取测量值
	//QString strValue;
	//for (int j = 0; j < 4; ++j)
	//{
	//	int nCurrValue = *(pPressureData + 3 - j);
	//	strValue += QString::number(nCurrValue);
	//	if(j + 1 < 4)
	//	{
	//		strValue += "; ";
	//	}
	//}
	//strValue += "\r\n";

	QMutexLocker lock(&m_pressureMutex);
	m_arrPressure.append(strPressure);
}

void QSavePressureThread::stop()
{
	//QMutexLocker lockPressure(&m_pressureMutex);
	//while(!m_arrPressure.isEmpty())
	//{
	//	continue;
	//}
	
	QMutexLocker lock(&m_stopMutex);
	m_isStop = true;
}

void QSavePressureThread::setFileName(const QString &strFileName)
{
	QMutexLocker lockFile(&m_filePressure);
	m_strFileName = strFileName;
	if(m_pSaveFile == NULL)
	{
		m_pSaveFile = new QSavePressure();
	}

	m_pSaveFile->setFileName(m_strFileName);
}

QString QSavePressureThread::getAbsoluteFileName()
{
	QMutexLocker lockFile(&m_filePressure);
	m_pSaveFile->save();
	QString strFileName = m_pSaveFile->getAbsoluteFileName();
	
	int nLastIndex = strFileName.indexOf("_2.txt");
	if(isRunning() && nLastIndex == -1)
	{
		int nSize = strFileName.lastIndexOf("_");
		QString strNewFileName = strFileName.left(nSize);
		strNewFileName += "_2.txt";
		m_pSaveFile->setFileName(strNewFileName);
	}

	return strFileName;
}
