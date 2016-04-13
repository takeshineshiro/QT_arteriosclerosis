#ifndef _STD_AFX_H
#define _STD_AFX_H

#include <QtWidgets>
#include "QConstValue.h"

#define Q_DELETE(p) if(p != NULL) {delete p; p = NULL;}

// 患者信息结构体
struct tagPersonInfo
{
	void operator=(const tagPersonInfo&info)
	{
		m_strNo = info.m_strNo;
		m_strName = info.m_strName;
		m_strSex = info.m_strSex;
		m_strAge = info.m_strAge;
		m_strHeight = info.m_strHeight;
		m_strWeight = info.m_strWeight;
		m_strSection = info.m_strSection;
		m_strDoctor = info.m_strDoctor;

		m_listHistory = info.m_listHistory;
		m_strNote = info.m_strNote;
		m_strDate = info.m_strDate;

		m_listLeftArm = info.m_listLeftArm;
		m_listRightArm = info.m_listRightArm;
		m_listLeftAnkle = info.m_listLeftAnkle;
		m_listRightAnkle = info.m_listRightAnkle;
		m_strDiagnose = info.m_strDiagnose;
	};

	QString m_strNo;// 编号
	QString m_strName;// 姓名
	QString m_strSex;// 性别
	QString m_strAge;// 年龄
	QString m_strHeight;// 身高
	QString m_strWeight;// 体重
	QString m_strSection;// 科室

	QString m_strDoctor;// 医生
	QStringList m_listHistory;// 患病史
	QString m_strNote;// 备注
	QString m_strDate; // 测量时间

	QStringList m_listLeftArm;// 左臂测量数据：依次为收缩压、平均压、舒张压
	QStringList m_listRightArm;// 右臂测量数据：依次为收缩压、平均压、舒张压
	QStringList m_listLeftAnkle;// 左踝测量数据：依次为收缩压、平均压、舒张压、ABI、PWV
	QStringList m_listRightAnkle;// 右踝测量数据：依次为收缩压、平均压、舒张压、ABI、PWV
	QString m_strDiagnose;
};

#define _WAVE_ANALYSIS 0

#endif //_STD_AFX_H