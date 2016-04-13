#ifndef _Q_DRAW_MEASURE_H
#define _Q_DRAW_MEASURE_H

class QPainter;

// 主要用来描绘 打印的报告和历史测量结果的查看功能
class QDrawMeasure
{
public:
	// 构造函数
	QDrawMeasure();
	// 析构函数
	~QDrawMeasure();

public:
	// 个人基本信息绘制
	void drawPersonInfo(QPainter *painter, tagPersonInfo &info);

	// 波形文件的绘制
	void drawWaveInfo(QPainter *painter, const tagPersonInfo &info);

	// 绘制测量数据
	void drawPressure(QPainter *painter, tagPersonInfo &info);

	// 绘制结论部分
	void drawResult(QPainter *painter, tagPersonInfo &info);

	// 绘制ABI及PWV二维分析图
	void drawAnalysis(QPainter *painter, tagPersonInfo &info, bool bDrawRect = false);

	// 绘制PWV分析图
	void drawPwvAnalysis(QPainter *painter, tagPersonInfo &info);
private:
	QPointF _getPointByABIAndPWV(const float &fABI, const int &nPWV, const int &nFontHeight, const float &nDeltaX, const float &nDeltaY, const int &nStartX, const int &nHeight);

	// 根据PWV和年龄，求得坐标
	QPointF _getPointByPWVAndAge(const float &nPWV, const int &nAge, const float &fXStartPos, const float &nYStart, const float &nDeltaX, const float &nDeltaY) const;
	
	// 根据年龄计算出+1SD的PWV
	float _getPositive1SDPWV(const int &nAge, const bool &isMen = true) const;

	// 根据年龄计算出当前年龄的平均PWV
	float _getAveragePWV(const int &nAge, const bool &isMen = true) const;

	// 根据年龄计算出-1SD的PWV
	float _getNegative1SDPWV(const int &nAge, const bool &isMen = true) const;

	// 设置画笔的粗细
	void _setPenWidthF(QPainter &painter) const;

	// 设置画笔
	void _setFont(QPainter &painter, const int nFontSize = 4,const bool bIsBold = false) const;
};

#endif
