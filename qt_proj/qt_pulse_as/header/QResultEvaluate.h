#pragma once
// 作用：主要根据不同性别及不同年龄，评价测量结果

class QResultEvaluate
{
public:
	QResultEvaluate();
	~QResultEvaluate();

	// 获取总结论
	QString  getEvaluate(const int &nAge, const double &nLeftABI, const double &nRightABI, const double &dLeftPWV,  const double &dRightPWV, bool bIsMale = true) const;

	// 评价血管阻塞情况(ABI)结果
	QString getABIEvaluate(const double &nLeftABI, const double &nRightABI, int &nABIStatus) const;

	// 评价血管硬度(baPWV)结果
	QString getPWVEvaluate(const int &nAge, const double &dLeftPWV,  const double &dRightPWV,bool bIsMale = true) const;

	// 获取描述信息：主要返回baPWV及ABI的描述信息
	QString getDescription() const;

	// 根据年龄，获取-1SD、均值、1SD、2SD的值
	void getPWVByAgeAndSex(const int &nAge, const bool bIsMale, double &dN1SDPWV, double &dAveragePWV,double &dP1SDPWV,double &dP2SDPWV) const;

	//// 获取平均值
	//double getAveragePWVByAge(const int &nAge, const bool bIsMale) const;

	//// 获取1SD的值
	//double getP1SDPWVByAge(const int &nAge, const bool bIsMale) const;

	//// 获取2SD的值
	//double getP2SDPWVByAge(const int &nAge, const bool bIsMale) const;
private:
	// 女性标准(记录从20岁起，每四个为一组，分别记录-1SD、平均值、+1SD、+2SD的值)
	static double m_pFemaleStandard[71*4];
	
	// 男性标准(记录从20岁起，每四个为一组，分别记录-1SD、平均值、+1SD、+2SD的值)
	static double m_pMaleStandard[71*4];
};

