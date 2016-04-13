#pragma once

// 滑动滤波缓存的个数
const int nPOLES = 7;

class QFilterData
{
public:
	QFilterData();
	
	~QFilterData();

public:
	// 判断收到的数据，是否有两个脉搏波上升点
	bool isHasSlope(const QVector<int> &vectPressure, QVector<int> &arrIndex) const;

	// 获得脉搏波的上升点
	QVector<int> getSlopeIndex(const QVector<int> &vectPressure) const;

	// 对读取到的脉搏波做拉直及顾虑处理
	QVector<float> dealOriginalData(QVector<int> &vectPressure, QVector<int> &vectIndex, 
		const int &nFilterIndex);

	// 从DATA目录下，获取测量的最后1000个点组成的脉搏波数据(文件名称为 编号(姓名).txt 格式)
	void getWaveData(const QString &strFileName, QVector<float> &leftArm,
		QVector<float> &leftAnkle,
		QVector<float> &rightArm,
		QVector<float> &rightAnkle,
		QVector<int> &arrMapValue, int &nLeftUT, int &nRightUT);

	// 将获取到的原始数据进行保存，并判断是否有一个完整脉搏波的到来，如果有，
	// 则解析出来脉搏波数据来
	void addReceiveData(const QVector<int> arrPressure);

	// 获取下一个需要描绘的波形数据，如果还没有数据则返回空值
	QVector<float> * getDrawValue();

	void reset();

	void getValleyPeak();
private:
	
	// 对脉搏波数据进行滤波操作
	void _filterPlus(QVector<float> &vectPressure, const int &nIndex);

	// 过滤原始数据
	QVector<int> _filterOrignalData(const QVector<int> &vectPressure) const;

	// 判断当前的压力值中是否有4000个点
	void _storePressure(const QVector<int> &vectPressure);

	// 计算%map的值
	QVector<int> _computeMap(const QVector<int> []) const;

	// 利用算数平均滤波算法，对原始波形进行滤波
	QVector<float> _averageFilter(const QVector<int> &vectPressure) const;

	// 根据滤波后的波形，计算出波形的上升时间
	int _getUTTime(const QVector<int> &arrValleyIndex, const int &nWaveIndex) const;

	// 滤波处理
	QVector<float> _filterOrignal(const QVector<int> &arrPressure);

	// 二次滤波
	void _filterSecond(QVector<float> &vectOrignal);

	// 保存滤波后的数据
	void _saveFiltedPressure(const QVector<float> &vectOrignal);

	// 判断是否是新的波谷,并且根据返回波谷出现的位置
	void _getValleyPeak(QVector<int>[]);

	// 解析波形数据
	void _dealPlusWave(const QVector<int>[]);

	// 解析波形数据
	void _dealPlusWave(const QVector<int> arrValleyIndex, const int nWaveIndex);

	// 获取波谷位置点
	QVector<int> _computeValley(const int &nIndex);

	// 判断当前点是否适合做波谷
	bool _isJudgeValley(const int &nWaveIndex, const int &nBegin);

private:
	float m_yvFilter[4][nPOLES];

	// 用于过滤收到的下位机原始波形
	int m_arrFilterOrignal[4][nPOLES];

	// 用于存放第二次滤波的滑动窗口中的数据
	float m_arrSecondFilter[4][12];

	// 用于保存滤波后的原始数据
	QVector<float> m_arrFiltedData[4];

	// 用于保存滤波后的
	QVector<int> m_arrSloped[4];

	// 用于描绘的点结果
	QVector<float> m_arrDrawValue[4];

	// 每次滤波去除前1000个点
	int m_nFirst1000;
};

