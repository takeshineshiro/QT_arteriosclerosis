#include "./header/stdafx.h"
#include "./header/QDrawWaveWorker.h"


QDrawWaveWorker::QDrawWaveWorker(QObject *pParent)
	:QObject(pParent)
{

	// 初始化踝臂能到达的最大值(默认值为200mmHg,考虑到误差，设置为1990)
	m_nArmMaxValue = nDEFAULT_MAX_VALUE;
	m_nAnkleMaxValue = nDEFAULT_MAX_VALUE;

	m_filter.reset();
	memset(m_bHasMax, 0x00, sizeof(m_bHasMax));

	QFile file1(QString::fromLocal8Bit("E:/Lepu_20150810/LepuMedical/SvnFile/MeasureArteriosclerosis(VS)/MeasureArteriosclerosis/MeasureArteriosclerosis/Data/201603171406(重复性测试_孙明01)_2.txt"));
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

			QVector<int> arrPressure;
			arrPressure.append(nRightArm);
			arrPressure.append(nLeftArm);
			arrPressure.append(nRightAnkle);
			arrPressure.append(nLeftAnkle);

			slotFiterWave(arrPressure);
		}
	}
	file1.close();

	m_filter.getValleyPeak();

	//QString strFileName = QString::fromLocal8Bit("E:/Lepu_20150810/LepuMedical/SvnFile/MeasureArteriosclerosis(VS)/MeasureArteriosclerosis/MeasureArteriosclerosis/Data/201603171406(重复性测试_孙明01)_2.txt");
	//QVector<float> leftAnkle;
	//QVector<float> rightAnkle;
	//QVector<float> leftArm;
	//QVector<float> rightArm;
	//QVector<int> arrMapValue;
	//int nLeftUT = 0;
	//int nRightUT = 0;
	//m_filter.getWaveData(strFileName, leftArm, leftAnkle, rightArm, rightAnkle, arrMapValue, nLeftUT, nRightUT);
	
	//connect(&m_timerDraw, SIGNAL(timeout()), this, SLOT(slotUpdateTimer()));
}

QDrawWaveWorker::~QDrawWaveWorker(void)
{
}

void QDrawWaveWorker::slotFiterWave(QVector<int> arrPressure)
{
	m_lastPressure = arrPressure;
	//qDebug()<<m_lastPressure.at(0);
	for(int i = 0; i <arrPressure.size(); ++i)
	{
		// 判断臂最大值
		if(i<2)
		{
			if(arrPressure.at(i) >= m_nArmMaxValue)
			{
				m_bHasMax[i] = true;
			}
		}
		// 判断踝最大值
		else
		{
			if(arrPressure.at(i) >= m_nAnkleMaxValue)
			{
				m_bHasMax[i] = true;
			}
		}
	}

	bool bCanSendPressure = true;
	for(int i = 0; i<4; ++i)
	{
		if(!m_bHasMax[i])
		{
			bCanSendPressure = false;
		}
	}

	if(bCanSendPressure)
	{
		m_arrLast240.append(arrPressure);
		if(m_arrLast240.size() == 960)
		{
			// 保存数据
			m_filter.addReceiveData(m_arrLast240);
			m_arrLast240.clear();
		}
	}

	//if(timer.hasExpired(1))
	//{
	//	slotUpdateTimer();
	//	timer.restart();
	//}
	//slotUpdateTimer();
}

void QDrawWaveWorker::slotResetMaxValue(int nMaxArmValue, int nMaxAnkleValue)
{
	m_nArmMaxValue = nMaxArmValue;
	m_nAnkleMaxValue = nMaxAnkleValue;

	memset(m_bHasMax, 0x00, sizeof(m_bHasMax));
	m_filter.reset();
}

void QDrawWaveWorker::start()
{
	memset(m_bHasMax, 0x00, sizeof(m_bHasMax));
}

void QDrawWaveWorker::slotUpdateTimer()
{
	QVector<float> *arrDrawWave = m_filter.getDrawValue();

	bool bCleardata = false;
	for(int i = 0; i<4; ++i)
	{
		if(!m_bHasMax[i])
		{
			bCleardata = true;
			break;
		}
	}

	//qDebug()<<"Timer:  "<<timer.elapsed();
	emit sigUpdateWavePressure(arrDrawWave, m_lastPressure, bCleardata);
}

void QDrawWaveWorker::stop()
{
	m_filter.reset();
	m_arrLast240.clear();
}
