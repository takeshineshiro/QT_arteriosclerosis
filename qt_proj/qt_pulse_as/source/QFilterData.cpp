#include "./header/stdafx.h"
#include "./header/QFilterData.h"


QFilterData::QFilterData()
{
	m_nFirst1000 = 0;
	reset();
}


QFilterData::~QFilterData(void)
{
}

bool QFilterData::isHasSlope(const QVector<int> &vectPressure, QVector<int> &arrIndex) const
{
	//QVector<int> vectFilterData = _filterOrignalData(vectPressure);
	QVector<int> vectIndex = getSlopeIndex(vectPressure);
	// 如果出现两个上升点，则判断当前数据中包含有一个数据的脉搏波
	if(vectIndex.isEmpty() || vectIndex.size() < 2)
	{
		return false;
	}
	// 上升点的位置
	arrIndex.append(vectIndex);
	return true;
}

QVector<int> QFilterData::_filterOrignalData(const QVector<int> &vectPressure) const
{
	QVector<int> vectRet;
	int yv[nPOLES] ={};
	QVector<int>::const_iterator iter = vectPressure.begin();
	for (;iter != vectPressure.end(); ++iter)
	{
		int nSum = 0.0f;
		for(int i = 1; i <nPOLES; ++i)
		{
			nSum += yv[nPOLES - i - 1];
			yv[nPOLES - i] = yv[nPOLES - i - 1];
		}
		yv[0] = *iter;
		nSum += yv[0];
		vectRet.append(nSum/12);
	}

	return vectRet;
}

// 获取波谷的位置
// 参考自：http://www.tuicool.com/articles/2ErqMj
QVector<int> QFilterData::getSlopeIndex(const QVector<int> &vectPressure) const
{
	// 过滤一次原始数据
	QVector<int> vectOriginalData = _filterOrignalData(vectPressure);

	//for(int i = 0; i<vectOriginalData.size(); ++i)
	//{
	//	qDebug()<<vectOriginalData.at(i)<<"\t"<<vectPressure.at(i);
	//}

	int nSize = vectOriginalData.size();

	QVector<int> slopeArr(vectOriginalData.size()-1, 0);
	for(int i=0; i < vectOriginalData.size()-1; ++i)
	{
		int nSlope = vectOriginalData.at(i+1) - vectOriginalData.at(i);
		if(nSlope>0)
		{
			slopeArr.replace(i, 1);
		}
		else if(nSlope<0)
		{
			slopeArr.replace(i, -1);
		}
	}

	for(int i = slopeArr.size() -1; i >= 0; --i)
	{
		if(slopeArr.at(i) == 0 && i == slopeArr.size() - 1)
		{
			slopeArr.replace(i, 1);
		}
		else if(slopeArr.at(i) == 0 && i != slopeArr.size() - 1)
		{
			if(slopeArr.at(i+1) >= 0)
			{
				slopeArr.replace(i, 1);
			}
			else 
			{
				slopeArr.replace(i, -1);
			}
		}
	}

	QVector<int> arrIndex;
	int nLastValley = 0;
	for(int i = 0; i < slopeArr.size() - 1; ++i)
	{
		if(slopeArr.at(i+1) - slopeArr.at(i) == 2)
		{
			if((i+1 - nLastValley)>65 || nLastValley == 0)
			{
				nLastValley = i+1;
				arrIndex.append(i+1);
			}
			else if((i+1 - nLastValley)<10)
			{
				arrIndex.replace(arrIndex.size()-1, i+1);
				nLastValley = i+1;
			}
		}
	}

	if(arrIndex.isEmpty() && vectPressure.size()>200)
	{
		arrIndex.append(0);
		arrIndex.append(vectPressure.size()-1);
	}
	else if (arrIndex.size() == 1 && vectPressure.size()>200)
	{
		arrIndex.append(vectPressure.size()-1);
	}
	return arrIndex;
}

void QFilterData::_filterPlus(QVector<float> &vectPressure, const int &nIndex)
{   
	//static float yv[4][nPOLES] = {};
	//if(bReset)
	//{
	//	memset(yv, 0x00, sizeof(yv[nIndex]));
	//}
	static float fLastAdd[4] = {};

	int nMaxValue = 0;
	int nMinValue = 0;
	QVector<float>::iterator iter = vectPressure.begin();
	for (;iter != vectPressure.end(); ++iter)
	{
		float fSum = 0.0f;
		for(int i = 1; i <nPOLES; ++i)
		{
			fSum += m_yvFilter[nIndex][nPOLES - i - 1];
			m_yvFilter[nIndex][nPOLES - i] = m_yvFilter[nIndex][nPOLES - i - 1];
		}

		m_yvFilter[nIndex][0] = *iter;
		fSum += m_yvFilter[nIndex][0];
		*iter = fSum/8.0f;
		nMaxValue = (int)(*iter) > nMaxValue ? (int)(*iter):nMaxValue;
		nMinValue = (int)(*iter) < nMinValue ? (int)(*iter):nMinValue;
	}

	// TODO:
	// 如果脉搏波波峰和波谷的相差较小，可以考虑画成直线
	// 否则如果最小值小于0，可以考虑整个波形平移到0点
	//if(nMinValue<0)
	//{
	iter = vectPressure.begin();
	float fDiff = *iter - nMinValue;
	for (;iter != vectPressure.end(); ++iter)
	{
		*iter = *iter + fDiff;
	}

	//iter = vectPressure.begin();
	//fDiff = fLastAdd[nIndex] - *iter;
	//for (;iter != vectPressure.end(); ++iter)
	//{
	//	*iter = *iter + fDiff;
	//}

	//}
	//if((int)(fLastAdd[nIndex])  != 0 )
	//{
	//float fStartPos =  vectPressure.at(0)  - fLastAdd[nIndex];
	//iter = vectPressure.begin();
	//for (;iter != vectPressure.end(); ++iter)
	//{
	//	*iter = *iter - fStartPos;
	//}
	//}

	fLastAdd[nIndex] = vectPressure.at(vectPressure.size()-1);
}

QVector<float> QFilterData::dealOriginalData(QVector<int> &vectPressure, QVector<int> &vectIndex, const int &nFilterIndex)
{
	QVector<float> vectPlusWave;
	// 如果脉搏波的上升点不在开头，且需要上升点之前的数据
	if(vectIndex.at(0) != 0)
	{
		int nRange = vectPressure.at(vectIndex.at(0)) - vectPressure.at(0);
		int nIndex = 1;
		int nMin = 0;
		int nMax = 0;
		for(int i = 1; i <vectIndex.at(0); ++i)
		{
			float nValue = vectPressure.at(i) - vectPressure.at(0) - nRange * nIndex/(vectIndex.at(0));
			vectPlusWave.append(nValue);
			nIndex++;
		}
	}

	{
		int nIndexSize = vectIndex.size();
		for(int i = 0; i < vectIndex.size() - 1; ++i)
		{
			int nRange =  vectPressure.at(vectIndex.at(i+1)) - vectPressure.at(vectIndex.at(i));
			int nPlusSize = vectIndex.at(i+1) - vectIndex.at(i);
			int nIndex = 1;
			for(int j = vectIndex.at(i); j <= vectIndex.at(i+1); ++j)
			{
				float nValue = vectPressure.at(j) - vectPressure.at(vectIndex.at(i)) - nRange * nIndex/(nPlusSize);
				vectPlusWave.append(nValue);
				nIndex++;
			}
		}
		vectPressure.remove(0, vectIndex.at(vectIndex.size()-1));
	}

	_filterPlus(vectPlusWave, nFilterIndex);
	return vectPlusWave;
}

void ButterworthLowpassFilter0100SixthOrder(float src, double &dest)
{   
	//   const int NZEROS = 6;
	//   const int NPOLES = 6;
	//   const double GAIN = 2.936532839e+03;
	//
	//static double xv[NZEROS+1] = {0.0}, yv[NPOLES+1] = {0.0};

	//xv[0] = xv[1]; xv[1] = xv[2]; xv[2] = xv[3]; xv[3] = xv[4]; xv[4] = xv[5]; xv[5] = xv[6]; 
	//xv[6] = src / GAIN;
	//yv[0] = yv[1]; yv[1] = yv[2]; yv[2] = yv[3]; yv[3] = yv[4]; yv[4] = yv[5]; yv[5] = yv[6]; 
	//yv[6] =   (xv[0] + xv[6]) + 6.0 * (xv[1] + xv[5]) + 15.0 * (xv[2] + xv[4])
	//	+ 20.0 * xv[3]
	//+ ( -0.0837564796 * yv[0]) + (  0.7052741145 * yv[1])
	//	+ ( -2.5294949058 * yv[2]) + (  4.9654152288 * yv[3])
	//	+ ( -5.6586671659 * yv[4]) + (  3.5794347983 * yv[5]);
	//dest = yv[6];

	// 32Hz6阶滤波
	const int NZEROS = 6;
	const int NPOLES = 6;
	const double GAIN = 4.004448900e+05;
	static double xv[NZEROS+1] = {0.0}, yv[NPOLES+1] = {0.0};
	xv[0] = xv[1]; xv[1] = xv[2]; xv[2] = xv[3]; xv[3] = xv[4]; xv[4] = xv[5]; xv[5] = xv[6]; 
	xv[6] = src*1.0 / GAIN;
	yv[0] = yv[1]; yv[1] = yv[2]; yv[2] = yv[3]; yv[3] = yv[4]; yv[4] = yv[5]; yv[5] = yv[6]; 
	yv[6] =   (xv[0] + xv[6]) + 6.0 * (xv[1] + xv[5]) + 15.0 * (xv[2] + xv[4])
		+ 20.0 * xv[3]
	+ ( -0.3774523864 * yv[0]) + (  2.6310551285 * yv[1])
		+ ( -7.6754745482 * yv[2]) + ( 11.9993158160 * yv[3])
		+ (-10.6070421840 * yv[4]) + (  5.0294383514 * yv[5]);
	dest = yv[6];

  //static int n = 16;
  //static int nCount = 4;
  //static double s = 800;
  //static double f1 = 0.5;
  //static double f2 = 10;
  //static double a = cos(M_PI*(f1+f2)/s)/cos(M_PI*(f1-f2)/s);
  //static double a2 = a*a;
  //static double b = tan(M_PI*(f1-f2)/s);
  //static double b2 = b*b;
  //static double r;

  //static double *A = (double *)malloc(4*sizeof(double));
  //static double *d1 = (double *)malloc(4*sizeof(double));
  //static double *d2 = (double *)malloc(4*sizeof(double));
  //static double *d3 = (double *)malloc(4*sizeof(double));
  //static double *d4 = (double *)malloc(4*sizeof(double));
  //static double *w0 = (double *)calloc(4, sizeof(double));
  //static double *w1 = (double *)calloc(4, sizeof(double));
  //static double *w2 = (double *)calloc(4, sizeof(double));
  //static double *w3 = (double *)calloc(4, sizeof(double));
  //static double *w4 = (double *)calloc(4, sizeof(double));
  //static bool bInit = false;

  //if(!bInit)
  //{
	 // bInit = true;
	 // for(int i=0; i<nCount; ++i)
	 // {
		//  r = sin(M_PI*(2.0*i+1.0)/(4.0*nCount));
		//  s = b2 + 2.0*b*r + 1.0;
		//  A[i] = b2/s;
		//  d1[i] = 4.0*a*(1.0+b*r)/s;
		//  d2[i] = 2.0*(b2-2.0*a2-1.0)/s;
		//  d3[i] = 4.0*a*(1.0-b*r)/s;
		//  d4[i] = -(b2 - 2.0*b*r + 1.0)/s;
	 // }
  //}

  //dest = src;
  //for(int i=0; i<nCount; ++i)
  //{
  //    w0[i] = d1[i]*w1[i] + d2[i]*w2[i]+ d3[i]*w3[i]+ d4[i]*w4[i] + dest;
  //    dest = A[i]*(w0[i] - 2.0*w2[i] + w4[i]);
  //    w4[i] = w3[i];
  //    w3[i] = w2[i];
  //    w2[i] = w1[i];
  //    w1[i] = w0[i];
  //}
}

void QFilterData::getWaveData(const QString &strFileName, QVector<float> &leftArm,
							  QVector<float> &leftAnkle,
							  QVector<float> &rightArm,
							  QVector<float> &rightAnkle, QVector<int> &arrMapValue, int &nLeftUT, int &nRightUT)
{
	QVector<int> vectOriginal[4];

	// 第一步:读取文件
	QFile file(strFileName);

	if(!file.exists())
	{
		return;
	}

	QVector<int> arrPressure;
	if(file.open(QFile::ReadOnly | QFile::Text))
	{
		QTextStream in(&file);
		while(!in.atEnd())
		{
			QStringList strValue = in.readLine().remove("\r\n").split("; ");

			if(strValue.size() < 4)
			{
				continue;
			}
			int nLeftAnkle = strValue.at(0).toInt();
			int nRightAnkle = strValue.at(1).toInt();
			int nLeftArm = strValue.at(2).toInt();
			int nRightArm = strValue.at(3).toInt();

			if(arrPressure.size() >= 84000)
			{
				for(int i = 0; i<4; ++i)
				{
					arrPressure.pop_front();
				}
			}

			arrPressure.append(nRightArm);
			arrPressure.append(nLeftArm);
			arrPressure.append(nRightAnkle);
			arrPressure.append(nLeftAnkle);


			//QVector<float> arrFiltered =  _filterOrignal(arrTemp);
			//for(int i = 0; i<4; ++i)
			//{
			//	//qDebug()<<"index Filtered: "<<i<<"\t"<<vectOrignal.at(i+j*4);
			//	m_arrFiltedData[i].append(arrFiltered.at(i));
			//}

			//for(int i = 0; i<4; ++i)
			//{

			//	arrPressure[i].append(arrTemp.at(i));
			//}
		}
	}
	file.close();

	for(int i = 0; i<arrPressure.size(); i += 4)
	{
		qDebug()<<arrPressure.at(i)<<"\t"<<arrPressure.at(i+1)<<"\t"<<arrPressure.at(i+2)<<"\t"<<arrPressure.at(i+3);
	}
	QVector<float> vectFilter = _filterOrignal(arrPressure);
	for(int i = 0; i<vectFilter.size();i += 4)
	{
		qDebug()<<vectFilter.at(i)<<"\t"<<vectFilter.at(i+1)<<"\t"<<vectFilter.at(i+2)<<"\t"<<vectFilter.at(i+3);
	}

	//double dDest = 0.0f;
	//for(int i = 0; i<vectFilter.size();i += 4)
	//{
	//	ButterworthLowpassFilter0100SixthOrder(vectFilter.at(i), dDest);
	//	qDebug()<<vectFilter.at(i)<<"\t"<<dDest;
	//}
	_filterSecond(vectFilter);

	for(int i = 0; i<vectFilter.size();i += 4)
	{
		qDebug()<<vectFilter.at(i)<<"\t"<<vectFilter.at(i+1)<<"\t"<<vectFilter.at(i+2)<<"\t"<<vectFilter.at(i+3);
	}

	_saveFiltedPressure(vectFilter);
	//// 滤波处理
	for(int i = 0; i <4; ++i)
	{
		//m_arrFiltedData[i].append();
		//_filterSecond(m_arrFiltedData[i]);
		//// 丢掉前1000个点
		m_arrFiltedData[i].remove(0, 1000);
	}

	QVector<int> arrValley[4];
	_getValleyPeak(arrValley);

	//double dDest = 0.0;
	//for(int i = 0; i<m_arrFiltedData[0].size(); ++i)
	//{
	//	ButterworthLowpassFilter0100SixthOrder(m_arrFiltedData[0].at(i), dDest);
	//	qDebug()<<m_arrFiltedData[0].at(i)<<"\t"<<dDest;
	//}

 // // static int n = 8;
 // // static int nCount = 8/4;
 // // static double s = 800;
 // // static double f1 = 30;
 // // static double f2 = 300;
 // // static double a = cos(M_PI*(f1+f2)/s)/cos(M_PI*(f1-f2)/s);
 // // static double a2 = a*a;
 // // static double b = tan(M_PI*(f1-f2)/s);
 // // static double b2 = b*b;
 // // static double r;

 // // static double *A = (double *)malloc(nCount*sizeof(double));
 // // static double *d1 = (double *)malloc(nCount*sizeof(double));
 // // static double *d2 = (double *)malloc(nCount*sizeof(double));
 // // static double *d3 = (double *)malloc(nCount*sizeof(double));
 // // static double *d4 = (double *)malloc(nCount*sizeof(double));
 // // static double *w0 = (double *)calloc(nCount, sizeof(double));
 // // static double *w1 = (double *)calloc(nCount, sizeof(double));
 // // static double *w2 = (double *)calloc(nCount, sizeof(double));
 // // static double *w3 = (double *)calloc(nCount, sizeof(double));
 // // static double *w4 = (double *)calloc(nCount, sizeof(double));
 // // static bool bInit = false;

 // //if(!bInit)
 // //{
	// // bInit = true;
	// // for(int i=0; i<nCount; ++i)
	// // {
	//	//  r = sin(M_PI*(2.0*i+1.0)/(4.0*nCount));
	//	//  s = b2 + 2.0*b*r + 1.0;
	//	//  A[i] = b2/s;
	//	//  d1[i] = 4.0*a*(1.0+b*r)/s;
	//	//  d2[i] = 2.0*(b2-2.0*a2-1.0)/s;
	//	//  d3[i] = 4.0*a*(1.0-b*r)/s;
	//	//  d4[i] = -(b2 - 2.0*b*r + 1.0)/s;
	// // }
 // //}

 // 
	////for(int j = 0; j <arrPressure[0].size(); ++j)
	////{
	////	//ButterworthLowpassFilter0100SixthOrder(arrPressure[0].at(i), dDest);
	////	dDest = arrPressure[0].at(j);
	////	for(int i=0; i<nCount; ++i)
	////	{
	////		w0[i] = d1[i]*w1[i] + d2[i]*w2[i]+ d3[i]*w3[i]+ d4[i]*w4[i] + dDest;
	////		dDest = A[i]*(w0[i] - 2.0*w2[i] + w4[i]);
	////		w4[i] = w3[i];
	////		w3[i] = w2[i];
	////		w2[i] = w1[i];
	////		w1[i] = w0[i];
	////	}
	////	qDebug()<<arrPressure[0].at(j)<<"\t"<<dDest;
	////	//qDebug()<<arrPressure[0].at(i)<<"\t"<<arrPressure[1].at(i)<<"\t"<<arrPressure[2].at(i)<<"\t"<<arrPressure[3].at(i);
	////}

	////for(int i = 1000; i <arrPressure[0].size(); ++i)
	////{
	////	qDebug()<<arrPressure[0].at(i)<<"\t"<<arrPressure[1].at(i)<<"\t"<<arrPressure[2].at(i)<<"\t"<<arrPressure[3].at(i);
	////}

	//for(int j = 0 ; j<m_arrFiltedData[0].size();++j)
	//{
	//	qDebug()<<m_arrFiltedData[0].at(j)<<"\t"<<m_arrFiltedData[1].at(j)<<"\t"<<m_arrFiltedData[2].at(j)<<"\t"<<m_arrFiltedData[3].at(j);
	//}

	for(int j = 0; j < m_arrSloped[0].size(); ++j)
	{
		qDebug()<<m_arrSloped[0].at(j)<<"\t"<<m_arrSloped[1].at(j)<<"\t"<<m_arrSloped[2].at(j)<<"\t"<<m_arrSloped[3].at(j);		
	}

	for(int i = 0; i<4; ++i)
	{
		for(int j = 0; j <arrValley[i].size();++j)
		{
			qDebug()<<arrValley[i].at(j);
		}
	}

	//// 计算MAP值
	arrMapValue.append(_computeMap(arrValley));
	////arrMapValue.append(_computeMap(vectOriginal[1]));
	////arrMapValue.append(_computeMap(vectOriginal[2]));
	////arrMapValue.append(_computeMap(vectOriginal[3]));

	// 第二步:处理获得的四肢数据
	//QVector<int> leftAnkleIndex = getSlopeIndex(vectOriginal[0]);
	//for(int i = 0; i<vectOriginal[0].size(); ++i)
	//{
	//	qDebug()<<vectOriginal[0].at(i);
	//}
	nLeftUT = _getUTTime(arrValley[3], 3);
	//leftAnkle.append(dealOriginalData(vectOriginal[0], leftAnkleIndex, 0));

	//QVector<int> rightAnkleIndex = getSlopeIndex(vectOriginal[1]);
	////for(int i = 0; i<vectOriginal[1].size(); ++i)
	////{
	////	qDebug()<<vectOriginal[1].at(i);
	////}
	nRightUT = _getUTTime(arrValley[2], 2);
	////for(int i=0; i < rightAnkleIndex.size(); ++i)
	////{
	////	qDebug()<<rightAnkleIndex.at(i);
	////}
	//rightAnkle.append(dealOriginalData(vectOriginal[1], rightAnkleIndex, 1));
	////for(int i=0; i < rightAnkle.size(); ++i)
	////{
	////	qDebug()<<rightAnkle.at(i);
	////}
	////getUTTime(rightAnkle);

	//QVector<int> leftArmIndex = getSlopeIndex(vectOriginal[2]);
	//leftArm.append(dealOriginalData(vectOriginal[2], leftArmIndex, 2));

	//QVector<int> rightArmIndex = getSlopeIndex(vectOriginal[3]);
	//rightArm.append(dealOriginalData(vectOriginal[3], rightArmIndex, 3));

	for(int i = 0; i<4; ++i)
	{
		if(arrValley[i].size()>=2)
		{
			// 步骤4: 如果新的波谷，则解析波形数据
			_dealPlusWave(arrValley[i], i);
		}
	}

	rightArm.append(m_arrDrawValue[0]);
	leftArm.append(m_arrDrawValue[1]);
	rightAnkle.append(m_arrDrawValue[2]);
	leftAnkle.append(m_arrDrawValue[3]);
}

void QFilterData::_storePressure(const QVector<int> &vectPressure)
{
	QVector<float> arrFilter = _averageFilter(vectPressure);
	for(int i = 0; i<4; ++i)
	{
		float fCurrPressure = arrFilter.at(i);
		if(m_arrFiltedData[i].size() >= 20000)
		{
			m_arrFiltedData[i].pop_front();
			m_arrFiltedData[i].push_back(fCurrPressure);
		}
		else
		{
			m_arrFiltedData[i].append(fCurrPressure);
		}
	}
}

void QFilterData::reset()
{
	for(int i=0; i <4; ++i)
	{
		memset(m_arrFilterOrignal[i], 0x00, sizeof(m_arrFilterOrignal[i]));
		memset(m_arrSecondFilter[i], 0x00, sizeof(m_arrSecondFilter[i]));
		memset(m_yvFilter[i], 0x00, sizeof(m_yvFilter[i]));

		// 用于保存滤波后的原始数据
		m_arrFiltedData[i].clear();
		// 用于保存滤波后的
		m_arrSloped[i].clear();
		m_arrDrawValue[i].clear();
	}

	m_nFirst1000 = 0;
}

QVector<float> QFilterData::_averageFilter(const QVector<int> &vectPressure) const
{
	QVector<float> vectRet;
	int nMin = 0;
	int nMax = 0;
	int nSum = 0;
	int yv[nPOLES] ={};
	QVector<int>::const_iterator iter = vectPressure.begin();
	for (;iter != vectPressure.end(); ++iter)
	{
		nMin = nMax = yv[nPOLES  - 1];
		for(int i = 1; i <nPOLES; ++i)
		{
			if(nMin>yv[nPOLES - i - 1])
			{
				nMin = yv[nPOLES - i - 1];
			}

			if(nMax<yv[nPOLES - i - 1])
			{
				nMax = yv[nPOLES - i - 1];
			}

			nSum += yv[nPOLES - i - 1];
			yv[nPOLES - i] = yv[nPOLES - i - 1];
		}
		yv[0] = *iter;

		if(nMin>yv[0])
		{
			nMin = yv[0];
		}

		if(nMax<yv[0])
		{
			nMax = yv[0];
		}
		nSum += yv[0];
		nSum -= nMin;
		nSum -= nMax;
		vectRet.append(nSum/6.0f);
		nSum = 0;
	}

	return vectRet;
}

QVector<float> averageFilter(const QVector<float> &vectPressure)
{
	QVector<float> vectRet;
	int nMin = 0;
	int nMax = 0;
	int nSum = 0;
	int yv[nPOLES] = {};
	QVector<float>::const_iterator iter = vectPressure.begin();
	for (;iter != vectPressure.end(); ++iter)
	{
		nMin = nMax = yv[nPOLES  - 1];
		for(int i = 1; i <nPOLES; ++i)
		{
			if(nMin>yv[nPOLES - i - 1])
			{
				nMin = yv[nPOLES - i - 1];
			}

			if(nMax<yv[nPOLES - i - 1])
			{
				nMax = yv[nPOLES - i - 1];
			}

			nSum += yv[nPOLES - i - 1];
			yv[nPOLES - i] = yv[nPOLES - i - 1];
		}
		yv[0] = *iter;

		if(nMin>yv[0])
		{
			nMin = yv[0];
		}

		if(nMax<yv[0])
		{
			nMax = yv[0];
		}
		nSum += yv[0];
		nSum -= nMin;
		nSum -= nMax;
		vectRet.append(nSum/6.0f);
		nSum = 0;
	}

	return vectRet;
}

// 根据滤波后的波形，计算出波形的上升时间
int QFilterData::_getUTTime(const QVector<int> &arrValleyIndex, const int &nWaveIndex) const
{
	//QVector<float> arrfilterWave = _averageFilter(vectWave);
	//QVector<float> arrFilter = averageFilter(arrfilterWave);
	////for(int i = 0; i<arrFilter.size(); ++i)
	////{
	////	qDebug()<<arrFilter.at(i);
	////}

	//int nSize = arrFilter.size();
	//QVector<float> slopeArr(arrFilter.size()-1, 0);
	//for(int i=0; i < arrFilter.size()-1; ++i)
	//{
	//	float fSlope = arrFilter.at(i+1) - arrFilter.at(i);
	//	if(fSlope>0.33f)
	//	{
	//		slopeArr.replace(i, 1);
	//	}
	//	else
	//	{
	//		slopeArr.replace(i, -1);
	//	}
	//}

	//for(int i = slopeArr.size() -1; i >= 0; --i)
	//{
	//	if(slopeArr.at(i) == 0 && i == slopeArr.size() - 1)
	//	{
	//		slopeArr.replace(i, 1);
	//	}
	//	else if(slopeArr.at(i) == 0 && i != slopeArr.size() - 1)
	//	{
	//		if(slopeArr.at(i+1) >= 0)
	//		{
	//			slopeArr.replace(i, 1);
	//		}
	//		else 
	//		{
	//			slopeArr.replace(i, -1);
	//		}
	//	}
	//}

	////for(int i = 0; i<slopeArr.size();++i)
	////{
	////	qDebug()<<slopeArr.at(i);
	////}

	//for(int i = slopeArr.size() - 4; i > 4; --i)
	//{
	//	int nFollowSum = 0;
	//	for(int j = i+1; j<i+3;++j)
	//	{
	//		nFollowSum += slopeArr.at(j);
	//	}
	//	int nPreSum = 0;
	//	for(int j = i-1; j>i-3;--j)
	//	{
	//		nPreSum += slopeArr.at(j);
	//	}

	//	if(nFollowSum == nPreSum)
	//	{
	//		slopeArr.replace(i, slopeArr.at(i-1));
	//	}
	//}

	////for(int i = 0; i<slopeArr.size();++i)
	////{
	////	qDebug()<<slopeArr.at(i);
	////}

	int nLastPeak = 0;
	QVector<int> vectPeak;
	{
		QVector<int>::const_iterator iter = arrValleyIndex.begin();
		for(; iter != arrValleyIndex.end();)
		{
			int nCurrValley = *iter;
			++iter;
			if(iter == arrValleyIndex.end())
			{
				break;
			}

			int nNextValley = *iter;
			int nPeakIndex = nCurrValley;
			for(; nPeakIndex < nNextValley; ++nPeakIndex)
			{
				float fPreDiff = 10.0f;
				if(nPeakIndex>0)
				{
					float fPreDiff = m_arrFiltedData[nWaveIndex].at(nPeakIndex) - m_arrFiltedData[nWaveIndex].at(nPeakIndex-1);
				}

				if(m_arrSloped[nWaveIndex].at(nPeakIndex) != 1 || qAbs(fPreDiff) < nCURR_WITH_PRE_DIFF)
				{
					break;
				}
			}
			vectPeak.append(nPeakIndex);
		}
	}

	//// 去除掉开始的波形
	int nCountWave = 0;
	int nSumUTTime = 0;
	
	QVector<int>::const_iterator iterValley = arrValleyIndex.begin();
	QVector<int>::const_iterator iterPeak = vectPeak.begin();
	for(; iterValley != arrValleyIndex.end() && iterPeak != vectPeak.end(); ++iterValley,++iterPeak)
	{
		nSumUTTime += (*iterPeak - *iterValley);
		++nCountWave;
	}

	int nRet  = 0;
	if(nCountWave != 0)
	{
		nRet = nSumUTTime*1.25/nCountWave;
	}
	////QVector<int> arrRet;
	////while(!vectValley.isEmpty() && !vectPeak.isEmpty())
	////{
	////	int nVIndex = vectValley.at(0);
	////	int nPeakIndex = vectPeak.at(0);
	////	arrRet.append(nPeakIndex - nVIndex);
	////	//nSumUTTime += (nPeakIndex - nVIndex);
	////	//++nCountWave;
	////	vectValley.pop_front();
	////	vectPeak.pop_front();
	////}

	////qSort(arrRet.begin(), arrRet.end());
	////int nRet = arrRet.at(arrRet.size()/2);
	return nRet;
}

// 计算脉搏波的
// MAP% 为 波形面积的平均面积的平均值除以脉搏的振幅。 当发生狭窄·闭塞的时候，%MAP 的数值变大。
// 简化处理：由于脉搏波的函数不是很好求的，简化脉搏波为一个由相邻波谷(T1,T2)和其波峰(P)组成的三角形，那么
//           脉搏波的MAP%就为两个波谷位置差值的一半 ,其公式表示如下：
//           %MAP = (T2-T1)*P/2/P = (T2-T1)/2
// 参数：arrSlopeIndex 出现波谷的位置
QVector<int> QFilterData::_computeMap(const QVector<int> arrValley[]) const
{
	int nCount = 0;
	int nPreValley = 0;
	int nSumDiff = 0;

	QVector<int> arrMap;
	//arrSlopeIndex.pop_front();

	//QVector<int>::const_iterator iter = arrSlopeIndex.begin();
	//while(iter != arrSlopeIndex.end())
	//{
	//	if(nPreValley != 0)
	//	{
	//		nSumDiff += (*iter - nPreValley);
	//		++nCount;
	//	}
	//	nPreValley = *iter;
	//	++iter;
	//}

	//QVector<float> arrfilterWave = _averageFilter(arrSlopeIndex);
	//QVector<float> arrFilter = averageFilter(arrfilterWave);
	////for(int i = 0; i<arrFilter.size(); ++i)
	////{
	////	qDebug()<<arrFilter.at(i);
	////}

	//int nSize = arrFilter.size();
	//QVector<float> slopeArr(arrFilter.size()-1, 0);
	//for(int i=0; i < arrFilter.size()-1; ++i)
	//{
	//	float fSlope = arrFilter.at(i+1) - arrFilter.at(i);
	//	if(fSlope>0.35f)
	//	{
	//		slopeArr.replace(i, 1);
	//	}
	//	else
	//	{
	//		slopeArr.replace(i, -1);
	//	}
	//}

	//for(int i = slopeArr.size() -1; i >= 0; --i)
	//{
	//	if(slopeArr.at(i) == 0 && i == slopeArr.size() - 1)
	//	{
	//		slopeArr.replace(i, 1);
	//	}
	//	else if(slopeArr.at(i) == 0 && i != slopeArr.size() - 1)
	//	{
	//		if(slopeArr.at(i+1) >= 0)
	//		{
	//			slopeArr.replace(i, 1);
	//		}
	//		else 
	//		{
	//			slopeArr.replace(i, -1);
	//		}
	//	}
	//}

	//for(int i = slopeArr.size() - 5; i > 5; --i)
	//{
	//	int nFollowSum = 0;
	//	for(int j = i+1; j<i+4;++j)
	//	{
	//		nFollowSum += slopeArr.at(j);
	//	}
	//	int nPreSum = 0;
	//	for(int j = i-1; j>i-4;--j)
	//	{
	//		nPreSum += slopeArr.at(j);
	//	}

	//	if(nFollowSum == nPreSum)
	//	{
	//		slopeArr.replace(i, slopeArr.at(i-1));
	//	}
	//}
	//const float EPSINON = 0.00001;
	//for(int i = 0; i<4; ++i)
	//{
	//	m_arrSloped[i].clear();
	//	int nSize = m_arrFiltedData[i].size();
	//	if(nSize<160)
	//	{
	//		continue;
	//	}

	//	for(int j = 0; j < m_arrFiltedData[i].size()-1; ++j)
	//	{
	//		float fAddDiff =  m_arrFiltedData[i].at(j+1) - m_arrFiltedData[i].at(j);
	//		if(fAddDiff > EPSINON)
	//		{
	//			m_arrSloped[i].append(1);
	//		}
	//		else if(fAddDiff < -EPSINON)
	//		{
	//			m_arrSloped[i].append(-1);
	//		}
	//		else
	//		{
	//			m_arrSloped[i].append(0);
	//		}
	//	}

	//	for(int j = m_arrSloped[i].size() - 1; j >= 1; --j)
	//	{
	//		//TODO: 计算当时波形的潮流
	//		if(j != m_arrSloped[i].size() - 1 && m_arrSloped[i].at(j) == 0)
	//		{
	//			if(m_arrSloped[i].at(j-1) == m_arrSloped[i].at(j+1))
	//			{
	//				m_arrSloped[i].replace(j, m_arrSloped[i].at(j-1));
	//				continue;
	//			}

	//			int nCurrSloped = j-1;
	//			while(m_arrSloped[i].at(nCurrSloped) == 0 && nCurrSloped>0)
	//			{
	//				--nCurrSloped;
	//			}
	//			m_arrSloped[i].replace(j, m_arrSloped[i].at(nCurrSloped));
	//		}
	//		else if(j != m_arrSloped[i].size() - 1 && m_arrSloped[i].at(j) == 0)
	//		{
	//			float fAddDiff =  m_arrFiltedData[i].at(j+2) - m_arrFiltedData[i].at(j-1);
	//			if(fAddDiff > EPSINON)
	//			{
	//				m_arrSloped[i].replace(j, 1);
	//			}
	//			else if(fAddDiff < -EPSINON)
	//			{
	//				m_arrSloped[i].replace(j, -1);
	//			}
	//			else
	//			{
	//				int nCurrSloped = j-1;
	//				while(m_arrSloped[i].at(nCurrSloped) == 0 && nCurrSloped>0)
	//				{
	//					--nCurrSloped;
	//				}
	//				m_arrSloped[i].replace(j, m_arrSloped[i].at(nCurrSloped));
	//			}
	//		}
	//	}

	//	// 对波动的点进行滤波计算
	//	for(int j = m_arrSloped[i].size() - 5; j > 4; --j)
	//	{
	//		int nFollowSum = 0;
	//		for(int k = j+1; k<=j+4;++k)
	//		{
	//			nFollowSum += m_arrSloped[i].at(k);
	//		}

	//		int nPreSum = 0;
	//		for(int k = j-1; k>=j-4;--k)
	//		{
	//			nPreSum += m_arrSloped[i].at(k);
	//		}

	//		if(nFollowSum == nPreSum)
	//		{
	//			m_arrSloped[i].replace(j, m_arrSloped[i].at(j-1));
	//		}
	//	}
	//}




	//int nLastValley = 0;
	//QVector<int> vectValley;
	//for(int i = 0; i < slopeArr.size() - 1; ++i)
	//{
	//	if(slopeArr.at(i+1) - slopeArr.at(i) == 2)
	//	{
	//		if((i+1 - nLastValley) >= 60 || nLastValley == 0)
	//		{
	//			nLastValley = i+1;
	//			vectValley.append(i+1);
	//		}
	//		else if((i+1 - nLastValley)<4)
	//		{
	//			vectValley.replace(vectValley.size()-1, i+1);
	//			nLastValley = i+1;
	//		}
	//	}
	//}

	for(int i = 0; i<4; ++i)
	{
		QVector<int>::const_iterator iter = arrValley[i].begin();
		while(iter != arrValley[i].end())
		{
			if(nPreValley != 0)
			{
				nSumDiff += (*iter - nPreValley);
				++nCount;
			}
			nPreValley = *iter;
			++iter;
		}

		int nMap = 0;
		if(nCount != 0)
		{
			nMap = nSumDiff/nCount/2;
		}
		arrMap.append(nMap);
	}
	return arrMap;
}

void filterFloatOrignal(QVector<float> &vecPressure)
{
	float nMin = 0;
	float nMax = 0;
	float nSum = 0;

	static float m_arrFilterOrignal[4][nPOLES] ={};
	QVector<float>::iterator iter = vecPressure.begin();
	int iIndex = 0;
	for(; iter != vecPressure.end(); ++iter,++iIndex)
	{
		nMax = nMin = m_arrFilterOrignal[iIndex][nPOLES - 1];
		for(int i = 1; i <nPOLES; ++i)
		{
			if(nMin>m_arrFilterOrignal[iIndex][nPOLES - i - 1])
			{
				nMin = m_arrFilterOrignal[iIndex][nPOLES - i - 1];
			}

			if(nMax<m_arrFilterOrignal[iIndex][nPOLES - i - 1])
			{
				nMax = m_arrFilterOrignal[iIndex][nPOLES - i - 1];
			}

			nSum += m_arrFilterOrignal[iIndex][nPOLES - i - 1];
			m_arrFilterOrignal[iIndex][nPOLES - i] = m_arrFilterOrignal[iIndex][nPOLES - i - 1];
		}

		m_arrFilterOrignal[iIndex][0] = *iter;

		if(nMin>m_arrFilterOrignal[iIndex][0])
		{
			nMin = m_arrFilterOrignal[iIndex][0];
		}

		if(nMax<m_arrFilterOrignal[iIndex][0])
		{
			nMax = m_arrFilterOrignal[iIndex][0];
		}
		nSum += m_arrFilterOrignal[iIndex][0];
		nSum -= nMin;
		nSum -= nMax;
		*iter = nSum/6.0f;
		nSum = 0;
	}
}


// 将获取到的原始数据进行保存，并判断是否有一个完整脉搏波的到来，如果有，
// 则解析出来脉搏波数据来
void QFilterData::addReceiveData(const QVector<int> arrPressure)
{
	// 步骤1：滤波处理
	QVector<float> filterRet;
	filterRet = _filterOrignal(arrPressure);
	_filterSecond(filterRet);

	//qDebug()<<"SrcData:"<<"\t"<<filterRet.at(0)<<"\t"<<pPressure[0]<<"\t"<<filterRet.at(1)<<"\t"<<pPressure[1]<<"\t"<<filterRet.at(2)<<"\t"<<pPressure[2]<<"\t"<<filterRet.at(3)<<"\t"<<pPressure[3];

	// 抛弃掉前1000个点
	++m_nFirst1000;
	if(m_nFirst1000<4)
	{
		return;
	}

	// 步骤2: 保存滤波后的数据
	_saveFiltedPressure(filterRet);

	//qDebug()<<"SrcData:"<<"\t"<<filterRet.at(0)<<"\t"<<pPressure[0]<<"\t"<<filterRet.at(1)<<"\t"<<pPressure[1]<<"\t"<<filterRet.at(2)<<"\t"<<pPressure[2]<<"\t"<<filterRet.at(3)<<"\t"<<pPressure[3];
	//return;
	//if(m_arrFiltedData[0].size()<160 && m_arrFiltedData[1].size()<160 && m_arrFiltedData[2].size()<160 && m_arrFiltedData[3].size()<160)
	//{
	//	return;
	//}
	return;
	// 步骤3: 判断是否是新的波谷,并且根据返回波谷出现的位置
	QVector<int> arrValley[4];
	_getValleyPeak(arrValley);

	//for(int i =0; i<4; ++i)
	//{
	//	if(arrValley[i].isEmpty() || arrValley[i].size()<2)
	//	{
	//		return;
	//	}
	//}
	for(int i = 0; i<4; ++i)
	{
		if(arrValley[i].size()>=2)
		{
			// 步骤4: 如果新的波谷，则解析波形数据
			_dealPlusWave(arrValley[i], i);
		}
	}
}

// 采用防脉冲干扰平均滤波法，思想如下：
// 说明：在一组数据中，去掉最大最小值，求平均值
// 优点：对于偶然出现的脉冲性干扰，可消
//       除有其引起的采样值偏差。对周期干扰有良好的抑制作用，
//       平滑度高。
// 缺点：测量速度慢
QVector<float> QFilterData::_filterOrignal(const QVector<int> &arrPressure)
{
	QVector<float> vectRet;
	if(arrPressure.isEmpty() || arrPressure.size() <4)
	{
		return vectRet;
	}

	static int nPreValue[4] = {};
	int nMin = 0;
	int nMax = 0;
	int nSum = 0;

	for(int j = 0; j <arrPressure.size()/4; ++j)
	{
		for(int iIndex = 0; iIndex<4; ++iIndex)
		{
			nMax = nMin = m_arrFilterOrignal[iIndex][1];
			for(int i = 1; i <nPOLES; ++i)
			{
				if(nMin>m_arrFilterOrignal[iIndex][nPOLES - i - 1])
				{
					nMin = m_arrFilterOrignal[iIndex][nPOLES - i - 1];
				}

				if(nMax<m_arrFilterOrignal[iIndex][nPOLES - i - 1])
				{
					nMax = m_arrFilterOrignal[iIndex][nPOLES - i - 1];
				}

				nSum += m_arrFilterOrignal[iIndex][nPOLES - i - 1];
				m_arrFilterOrignal[iIndex][nPOLES - i] = m_arrFilterOrignal[iIndex][nPOLES - i - 1];
			}

			if(nPreValue[iIndex] == 0)
			{
				nPreValue[iIndex] = arrPressure.at(j*4 + iIndex);
			}

			if(qAbs(nPreValue[iIndex] - arrPressure.at(j*4 + iIndex)) > 2 && qAbs(nPreValue[iIndex] - arrPressure.at(j*4 + iIndex)) <= 50)
			{
				//m_arrFilterOrignal[iIndex][0] = m_arrFilterOrignal[iIndex][0];
				//qDebug()<<"AAA";
			}
			else
			{
				m_arrFilterOrignal[iIndex][0] = arrPressure.at(j*4 + iIndex);
			}

			nPreValue[iIndex] = arrPressure.at(j*4 + iIndex);

			if(nMin>m_arrFilterOrignal[iIndex][0])
			{
				nMin = m_arrFilterOrignal[iIndex][0];
			}

			if(nMax<m_arrFilterOrignal[iIndex][0])
			{
				nMax = m_arrFilterOrignal[iIndex][0];
			}
			nSum += m_arrFilterOrignal[iIndex][0];
			nSum -= nMin;
			nSum -= nMax;
			vectRet.append(nSum/6.0f);
			nSum = 0;
		}
	}
	return vectRet;
}

void QFilterData::_filterSecond(QVector<float> &vectOrignal)
{
	{
		float nSum = 0.0f;
		QVector<float>::iterator iter = vectOrignal.begin();
		int iIndex = 0;
		for(; iter != vectOrignal.end(); ++iter,++iIndex)
		{
			nSum = 0.0f;
			for(int i = 1; i <12; ++i)
			{
				nSum += m_arrSecondFilter[iIndex%4][12 - i - 1];
				m_arrSecondFilter[iIndex%4][12 - i] = m_arrSecondFilter[iIndex%4][12 - i - 1];
			}

			m_arrSecondFilter[iIndex%4][0] = *iter;


			nSum += m_arrSecondFilter[iIndex%4][0];

			*iter = nSum/12.0f;
		}
	}
}

void QFilterData::_saveFiltedPressure(const QVector<float> &vectOrignal)
{
	for(int j = 0; j < vectOrignal.size()/4; ++j)
	{
		for(int i = 0; i<4; ++i)
		{
			//qDebug()<<"index Filtered: "<<i<<"\t"<<vectOrignal.at(i+j*4);
			m_arrFiltedData[i].append(vectOrignal.at(i+j*4));
		}
	}
}

void QFilterData::_getValleyPeak(QVector<int> arrIndex[])
{
	//for(int i=0; i < 4; ++i)
	//{
	//	if(m_arrFiltedData[i].isEmpty() || m_arrFiltedData[i].size() < 2)
	//	{
	//		continue;
	//	}

	//	// 步骤一：对新加入的点进行计算，求出新增量
	//	int nSize = m_arrFiltedData[i].size();
	//	float fAddDiff =  m_arrFiltedData[i].at(nSize-1) - m_arrFiltedData[i].at(nSize-2);
	//	if(fAddDiff > 0.33f)
	//	{
	//		m_arrSloped[i].append(1);
	//	}
	//	else if(fAddDiff < 0.33f)
	//	{
	//		m_arrSloped[i].append(-1);
	//	}
	//	else
	//	{
	//		m_arrSloped[i].append(0);
	//	}

	//	if(m_arrSloped[i].size() < 30)
	//	{
	//		continue;
	//	}

	//	// 计算当时的波形方向的潮流
	//	for(int j = m_arrSloped[i].size() -1; j >= 0; --j)
	//	{
	//		if(m_arrSloped[i].at(j) == 0 && j == m_arrSloped[i].size() - 1)
	//		{
	//			if(fAddDiff > 0.0f)
	//			{
	//				m_arrSloped[i].replace(j, 1);
	//			}
	//			else
	//			{
	//				m_arrSloped[i].replace(j, -1);
	//			}

	//		}
	//		else if(m_arrSloped[i].at(j) == 0 && j != m_arrSloped[i].size() - 1)
	//		{
	//			if(m_arrSloped[i].at(j+1) >= 0)
	//			{
	//				m_arrSloped[i].replace(j, 1);
	//			}
	//			else 
	//			{
	//				m_arrSloped[i].replace(j, -1);
	//			}
	//		}
	//	}

	//	// 对波动的点进行滤波计算
	//	for(int j = m_arrSloped[i].size() - 4; j > 4; --j)
	//	{
	//		int nFollowSum = 0;
	//		for(int k = j+1; k<j+3;++k)
	//		{
	//			nFollowSum += m_arrSloped[i].at(j);
	//		}

	//		int nPreSum = 0;
	//		for(int k = j-1; k>j-3;--k)
	//		{
	//			nPreSum += m_arrSloped[i].at(k);
	//		}

	//		if(nFollowSum == nPreSum)
	//		{
	//			m_arrSloped[i].replace(j, m_arrSloped[i].at(j-1));
	//		}
	//	}
	//	arrIndex[i].append(_computeValley(i));
	//}

	for(int i = 0; i<4; ++i)
	{
		m_arrSloped[i].clear();
		int nSize = m_arrFiltedData[i].size();
		if(nSize<nMIN_WAVE_PRESSURECOUNT)
		{
			continue;
		}

		for(int j = 0; j < m_arrFiltedData[i].size()-1; ++j)
		{
			float fAddDiff =  m_arrFiltedData[i].at(j+1) - m_arrFiltedData[i].at(j);
			if(fAddDiff > nCURR_WITH_PRE_DIFF)
			{
				m_arrSloped[i].append(1);
			}
			else if(fAddDiff < -nCURR_WITH_PRE_DIFF)
			{
				m_arrSloped[i].append(-1);
			}
			else
			{
				m_arrSloped[i].append(0);
			}
		}

		for(int j = m_arrSloped[i].size() - 1; j >= 1; --j)
		{
			//TODO: 计算当时波形的潮流
			if(j == m_arrSloped[i].size() - 1 && m_arrSloped[i].at(j) == 0)
			{
				//if(m_arrSloped[i].at(j-1) == m_arrSloped[i].at(j+1))
				//{
				//	m_arrSloped[i].replace(j, m_arrSloped[i].at(j-1));
				//	continue;
				//}

				int nCurrSloped = j-1;
				while(m_arrSloped[i].at(nCurrSloped) == 0 && nCurrSloped>0)
				{
					--nCurrSloped;
				}

				m_arrSloped[i].replace(j, m_arrSloped[i].at(nCurrSloped));
			}
			else if(j != m_arrSloped[i].size() - 1 && m_arrSloped[i].at(j) == 0)
			{
				float fAddDiff =  m_arrFiltedData[i].at(j+2) - m_arrFiltedData[i].at(j-1);
				if(fAddDiff > nCURR_WITH_PRE_DIFF)
				{
					m_arrSloped[i].replace(j, 1);
				}
				else if(fAddDiff < -nCURR_WITH_PRE_DIFF)
				{
					m_arrSloped[i].replace(j, -1);
				}
				else
				{
					int nCurrSloped = j-1;
					if(m_arrSloped[i].at(nCurrSloped) == 0)
					{
						while(m_arrSloped[i].at(nCurrSloped) == 0 && nCurrSloped>0)
						{
							--nCurrSloped;
						}
					}

					if(m_arrSloped[i].at(nCurrSloped) == 0)
					{
						nCurrSloped = j+1;
						while(nCurrSloped <m_arrSloped[i].size() && m_arrSloped[i].at(nCurrSloped) == 0 && nCurrSloped<j+30)
						{
							++nCurrSloped;
						}
					}

					m_arrSloped[i].replace(j, m_arrSloped[i].at(nCurrSloped));
				}
			}
		}

		//// 对波动的点进行滤波计算
		//for(int j = m_arrSloped[i].size() - 5; j > 4; --j)
		//{
		//	int nFollowSum = 0;
		//	for(int k = j+1; k<=j+4;++k)
		//	{
		//		nFollowSum += m_arrSloped[i].at(k);
		//	}

		//	int nPreSum = 0;
		//	for(int k = j-1; k>=j-4;--k)
		//	{
		//		nPreSum += m_arrSloped[i].at(k);
		//	}

		//	if(nFollowSum == nPreSum)
		//	{
		//		m_arrSloped[i].replace(j, m_arrSloped[i].at(j-1));
		//	}
		//}

		// 获取波谷
		arrIndex[i].append(_computeValley(i));
	}
}

void QFilterData::getValleyPeak()
{
	//const float EPSINON = 0.00001;

	//for(int i = 0; i<4; ++i)
	//{
	//	int nSize = m_arrFiltedData[i].size();
	//	for(int j = 0; j < m_arrFiltedData[i].size()-1; ++j)
	//	{
	//		float fAddDiff =  m_arrFiltedData[i].at(j+1) - m_arrFiltedData[i].at(j);
	//		if(fAddDiff > EPSINON)
	//		{
	//			m_arrSloped[i].append(1);
	//		}
	//		else if(fAddDiff < -EPSINON)
	//		{
	//			m_arrSloped[i].append(-1);
	//		}
	//		else
	//		{
	//			m_arrSloped[i].append(0);
	//		}
	//	}

	//	for(int j = m_arrSloped[i].size() - 1; j >= 1; --j)
	//	{
	//		//TODO: 计算当时波形的潮流
	//		if(j != m_arrSloped[i].size() - 1 && m_arrSloped[i].at(j) == 0)
	//		{
	//			if(m_arrSloped[i].at(j-1) == m_arrSloped[i].at(j+1))
	//			{
	//				m_arrSloped[i].replace(j, m_arrSloped[i].at(j-1));
	//				continue;
	//			}

	//			int nCurrSloped = j-1;
	//			while(m_arrSloped[i].at(nCurrSloped) == 0 && nCurrSloped>0)
	//			{
	//				--nCurrSloped;
	//			}
	//			m_arrSloped[i].replace(j, m_arrSloped[i].at(nCurrSloped));
	//		}
	//		else if(j != m_arrSloped[i].size() - 1 && m_arrSloped[i].at(j) == 0)
	//		{
	//			float fAddDiff =  m_arrFiltedData[i].at(j+2) - m_arrFiltedData[i].at(j-1);
	//			if(fAddDiff > EPSINON)
	//			{
	//				m_arrSloped[i].replace(j, 1);
	//			}
	//			else if(fAddDiff < -EPSINON)
	//			{
	//				m_arrSloped[i].replace(j, -1);
	//			}
	//			else
	//			{
	//				int nCurrSloped = j-1;
	//				while(m_arrSloped[i].at(nCurrSloped) == 0 && nCurrSloped>0)
	//				{
	//					--nCurrSloped;
	//				}
	//				m_arrSloped[i].replace(j, m_arrSloped[i].at(nCurrSloped));
	//			}
	//		}
	//	}

	//	//// 计算当时的波形方向的潮流
	//	//for(int j = m_arrSloped[i].size() -1; j >= 1; --j)
	//	//{
	//	//	float fAddDiff = m_arrFiltedData[i].at(j) - m_arrFiltedData[i].at(j-1);
	//	//	if(m_arrSloped[i].at(j) == 0 && j == m_arrSloped[i].size() - 1)
	//	//	{
	//	//		if(fAddDiff > 0.0f)
	//	//		{
	//	//			m_arrSloped[i].replace(j, 1);
	//	//		}
	//	//		else
	//	//		{
	//	//			m_arrSloped[i].replace(j, -1);
	//	//		}
	//	//	}
	//	//	else if(m_arrSloped[i].at(j) == 0 && j != m_arrSloped[i].size() - 1)
	//	//	{
	//	//		if(m_arrSloped[i].at(j+1) >= 0)
	//	//		{
	//	//			m_arrSloped[i].replace(j, 1);
	//	//		}
	//	//		else 
	//	//		{
	//	//			m_arrSloped[i].replace(j, -1);
	//	//		}
	//	//	}
	//	//}

	//	// 对波动的点进行滤波计算
	//	for(int j = m_arrSloped[i].size() - 5; j > 4; --j)
	//	{
	//		int nFollowSum = 0;
	//		for(int k = j+1; k<=j+4;++k)
	//		{
	//			nFollowSum += m_arrSloped[i].at(k);
	//		}

	//		int nPreSum = 0;
	//		for(int k = j-1; k>=j-4;--k)
	//		{
	//			nPreSum += m_arrSloped[i].at(k);
	//		}

	//		if(nFollowSum == nPreSum)
	//		{
	//			m_arrSloped[i].replace(j, m_arrSloped[i].at(j-1));
	//		}
	//	}
	//}

	//int nSize = m_arrSloped[0].size();
	//nSize = m_arrSloped[1].size();
	//nSize = m_arrSloped[2].size();
	//nSize = m_arrSloped[3].size();
    
    QVector<int> arrIndex[4];
    _getValleyPeak(arrIndex);
	for(int i = 0; i <4; ++i)
	{
		for(int j = 0; j < m_arrFiltedData[i].size(); ++j)
		{
			qDebug()<<m_arrFiltedData[i].at(j);
		}
	}

	for(int j = 0; j<m_arrSloped[0].size(); ++j)
	{
		qDebug()<<"\t"<<m_arrSloped[0].at(j)<<"\t"<<m_arrSloped[1].at(j)<<"\t"<<m_arrSloped[2].at(j)<<"\t"<<m_arrSloped[3].at(j);
	}

	for(int i = 0; i<4;++i)
	{
		for(int j = 0 ;j <arrIndex[i].size();++j)
		{
			qDebug()<<arrIndex[i].at(j);
		}
	}

	//QVector<int> valley0 = _computeValley(0);
	//QVector<int> valley1 = _computeValley(1);
	//QVector<int> valley2 = _computeValley(2);
	//QVector<int> valley3 = _computeValley(3);

	//for(int i = 0; i<valley0.size(); ++i)
	//{
	//	qDebug()<<valley0.at(i);
	//}

	//for(int i = 0; i<valley1.size(); ++i)
	//{
	//	qDebug()<<valley1.at(i);
	//}

	//for(int i = 0; i<valley2.size(); ++i)
	//{
	//	qDebug()<<valley2.at(i);
	//}

	//for(int i = 0; i<valley3.size(); ++i)
	//{
	//	qDebug()<<valley3.at(i);
	//}

	//_dealPlusWave(valley0, 0);
	//_dealPlusWave(valley1, 1);
	//_dealPlusWave(valley2, 2);
	//_dealPlusWave(valley3, 3);

	//for(int i = 0; i<4;++i)
	//{
	//	for(int j=0;j<m_arrDrawValue[i].size();++j)
	//	{
	//		qDebug()<<m_arrDrawValue[i].at(j);
	//	}
	//}
}

QVector<int> QFilterData::_computeValley(const int &nIndex)
{
	int nLastValley = 0;
	//QVector<int> vectValley;
	//for(int i = 0; i < m_arrSloped[nIndex].size() - 1; ++i)
	//{
	//	if(m_arrSloped[nIndex].at(i+1) - m_arrSloped[nIndex].at(i) == 2)
	//	{
	//		if((i+1 - nLastValley) >= nMIN_WAVE_PRESSURECOUNT || nLastValley == 0)
	//		{
	//			bool bIsValley = true;
	//			for(int j = i+1; j<=i+nSLOPE_CONTINUOUS_COUNT && j<m_arrSloped[nIndex].size() - 1;++j)
	//			{
	//				if(m_arrSloped[nIndex].at(j) != m_arrSloped[nIndex].at(i+1))
	//				{
	//					bIsValley = false;
	//					break;
	//				}
	//			}

	//			if(bIsValley)
	//			{
	//				nLastValley = i+1;
	//				vectValley.append(i+1);
	//			}
	//		}
	//		else if((i+1 - nLastValley)<5)
	//		{
	//			bool bIsValley = true;
	//			for(int j = i+1; j<=i+nSLOPE_CONTINUOUS_COUNT && j<m_arrSloped[nIndex].size() - 1;++j)
	//			{
	//				if(m_arrSloped[nIndex].at(j) != m_arrSloped[nIndex].at(i+1))
	//				{
	//					bIsValley = false;
	//					break;
	//				}
	//			}

	//			if(bIsValley)
	//			{
	//				vectValley.replace(vectValley.size()-1, i+1);
	//				nLastValley = i+1;
	//			}
	//		}
	//	}
	//}

	QVector<int> vectValley;
	for(int i = 0; i < m_arrSloped[nIndex].size() - 1; ++i)
	{
		if(m_arrSloped[nIndex].at(i+1) - m_arrSloped[nIndex].at(i) == 2)
		{
			if((i+1 - nLastValley) >= nMIN_WAVE_PRESSURECOUNT || nLastValley == 0)
			{
				bool bIsValley = _isJudgeValley(nIndex, i+1);
				//for(int j = i+1; j<=i+nSLOPE_CONTINUOUS_COUNT && j<m_arrSloped[nIndex].size() - 1;++j)
				//{
				//	if(m_arrSloped[nIndex].at(j) != m_arrSloped[nIndex].at(i+1))
				//	{
				//		bIsValley = false;
				//		break;
				//	}
				//}

				if(bIsValley)
				{
					nLastValley = i+1;
					vectValley.append(i+1);
				}
			}
			else if((i+1 - nLastValley)<5)
			{
				bool bIsValley = _isJudgeValley(nIndex, i+1);
				//for(int j = i+1; j<=i+nSLOPE_CONTINUOUS_COUNT && j<m_arrSloped[nIndex].size() - 1;++j)
				//{
				//	if(m_arrSloped[nIndex].at(j) != m_arrSloped[nIndex].at(i+1))
				//	{
				//		bIsValley = false;
				//		break;
				//	}
				//}

				if(bIsValley)
				{
					vectValley.replace(vectValley.size()-1, i+1);
					nLastValley = i+1;
				}
			}
		}
	}
	return vectValley;
}

// 判断当前点是否适合做波谷
bool QFilterData::_isJudgeValley(const int &nWaveIndex, const int &nBegin)
{
	bool bRet = true;

	int nLastPositive = nBegin;
	// 想后找寻30个1(即连续30个点上升)
	for(; nLastPositive<=nBegin+nSLOPE_CONTINUOUS_COUNT && nLastPositive<m_arrSloped[nWaveIndex].size() - 1;++nLastPositive)
	{
		if(m_arrSloped[nWaveIndex].at(nLastPositive) != 1)
		{
			bRet = false;
			break;
		}
	}

	if(bRet)
	{
		for(; nLastPositive<m_arrSloped[nWaveIndex].size() - 1;++nLastPositive)
		{
			if(m_arrSloped[nWaveIndex].at(nLastPositive) != 1)
			{
				break;
			}
		}
		
		int nLessPreIndex = nBegin - 1;
		bool bLessPre = false;
        while(nLessPreIndex>0 && 
			  nBegin -  nLessPreIndex<= nMIN_WAVE_PRESSURECOUNT)
		{
			if(m_arrFiltedData[nWaveIndex].at(nLessPreIndex--) - m_arrFiltedData[nWaveIndex].at(nLastPositive) > nCURR_WITH_PRE_DIFF)
			{
				bLessPre = true;
				break;
			}
		}

		float fFollowDiff = m_arrFiltedData[nWaveIndex].at(nLastPositive) - m_arrFiltedData[nWaveIndex].at(nBegin-1);
		if(bLessPre || fFollowDiff < fMIN_WAVE_AMPLITUDE)
		{
			bRet = false;
		}
	}
	return bRet;
}

void QFilterData::_dealPlusWave(const QVector<int> arrIndex[])
{
	if (NULL == arrIndex)
	{
		return;
	}

	for(int i =0; i<4; ++i)
	{
		if(arrIndex[i].isEmpty() || arrIndex[i].size()<2)
		{
			return;
		}
	}

	for(int k = 0; k<4; ++k)
	{
		QVector<float> vectPlusWave;
		if(arrIndex[k].at(0) != 0)
		{
			int nRange = m_arrFiltedData[k].at(arrIndex[k].at(0)) - m_arrFiltedData[k].at(0);
			int nIndex = 1;
			int nMin = 0;
			int nMax = 0;
			for(int i = 1; i <arrIndex[k].at(0); ++i)
			{
				float nValue = m_arrFiltedData[k].at(i) - m_arrFiltedData[k].at(0) - nRange * nIndex/(arrIndex[k].at(0));
				vectPlusWave.append(nValue);
				nIndex++;
			}
		}

		for(int i = 0; i < arrIndex[k].size() - 1; ++i)
		{
			float nRange =  m_arrFiltedData[k].at(arrIndex[k].at(i+1)) - m_arrFiltedData[k].at(arrIndex[k].at(i));
			int nPlusSize = arrIndex[k].at(i+1) - arrIndex[k].at(i);
			int nIndex = 1;
			for(int j = arrIndex[k].at(i); j <= arrIndex[k].at(i+1); ++j)
			{
				float nValue = m_arrFiltedData[k].at(j) - m_arrFiltedData[k].at(arrIndex[k].at(i)) - nRange * nIndex/(nPlusSize);
				vectPlusWave.append(nValue);
				nIndex++;
			}
		}

		//for(int i = 0; i < arrIndex[k].size() ; ++i)
		//{
		//	qDebug()<<"arrIndex"<<k<<"\t"<<arrIndex[k].at(i);
		//}

		//for(int i = 0; i < m_arrFiltedData[k].size(); ++i)
		//{
		//	qDebug()<<"m_arrFiltedData Before:"<<k<<"\t"<<m_arrFiltedData[k].at(i);
		//}

		//for(int i = 0; i < m_arrSloped[k].size(); ++i)
		//{
		//	qDebug()<<"m_arrSloped Before:"<<k<<"\t"<<m_arrSloped[k].at(i);
		//}

		m_arrFiltedData[k].remove(0, arrIndex[k].last());
		m_arrSloped[k].remove(0, arrIndex[k].last());
		_filterPlus(vectPlusWave, k);

		//for(int i = 0; i < m_arrFiltedData[k].size(); ++i)
		//{
		//	qDebug()<<"m_arrFiltedData After:"<<k<<"\t"<<m_arrFiltedData[k].at(i);
		//}

		//for(int i = 0; i < m_arrSloped[k].size(); ++i)
		//{
		//	qDebug()<<"m_arrSloped After:"<<k<<"\t"<<m_arrSloped[k].at(i);
		//}

		//for(int i = 0; i <vectPlusWave.size();++i)
		//{
		//	qDebug()<<"vectPlusWave"<<k<<"\t"<<vectPlusWave.at(i);
		//}

		m_arrDrawValue[k].append(vectPlusWave);
	}
	//for(int i=0; i<1;++i)
	//{
	//	for(int j =0; j<m_arrDrawValue[i].size(); ++j)
	//	{
	//		qDebug()<<"DealData"<<"\t"<<m_arrDrawValue[i].at(j);
	//	}
	//}
}

void QFilterData::_dealPlusWave(const QVector<int> arrValleyIndex, const int nWaveIndex)
{
	if (arrValleyIndex.isEmpty())
	{
		return;
	}

	if(arrValleyIndex.at(0) != 0)
	{
		int nRange = m_arrFiltedData[nWaveIndex].at(arrValleyIndex.at(0)) - m_arrFiltedData[nWaveIndex].at(0);
		int nIndex = 1;
		int nMin = 0;
		int nMax = 0;
		for(int i = 1; i <arrValleyIndex.at(0); ++i)
		{
			float nValue = m_arrFiltedData[nWaveIndex].at(i) - m_arrFiltedData[nWaveIndex].at(0) - nRange * nIndex/(arrValleyIndex.at(0));
			m_arrDrawValue[nWaveIndex].append(nValue);
			nIndex++;
		}
	}

	for(int i = 0; i < arrValleyIndex.size() - 1; ++i)
	{
		float fRange =  m_arrFiltedData[nWaveIndex].at(arrValleyIndex.at(i+1)) - m_arrFiltedData[nWaveIndex].at(arrValleyIndex.at(i));
		int nPlusSize = arrValleyIndex.at(i+1) - arrValleyIndex.at(i);
		int nIndex = 1;
		float fBegin = m_arrFiltedData[nWaveIndex].at(arrValleyIndex.at(i));
		for(int j = arrValleyIndex.at(i); j <= arrValleyIndex.at(i+1); ++j)
		{
			float nValue = m_arrFiltedData[nWaveIndex].at(j) - fBegin - fRange * nIndex/(nPlusSize);
			m_arrDrawValue[nWaveIndex].append(nValue);
			nIndex++;
		}
	}
	m_arrFiltedData[nWaveIndex].remove(0, arrValleyIndex.last());
}

// 获取下一个需要描绘的波形数据，如果还没有数据则返回空值
QVector<float> * QFilterData::getDrawValue()
{
	int nMinSize = m_arrDrawValue[0].size();
	for(int i = 1; i<4; ++i)
	{
		if(nMinSize >  m_arrDrawValue[i].size())
		{
			nMinSize = m_arrDrawValue[i].size();
		}
	}

	if(nMinSize == 0)
	{
		return NULL;
	}

	QVector<float> *pArrRet = new QVector<float>();
	for(int i = 0; i<nMinSize; ++i)
	{
		for(int j = 0; j<4; ++j)
		{
			pArrRet->append(m_arrDrawValue[j].at(0));
			//qDebug()<<"Index Draw:"<<j<<"\t"<<m_arrDrawValue[j].at(0);
			m_arrDrawValue[j].pop_front();
		}
	}
	return pArrRet;
}
