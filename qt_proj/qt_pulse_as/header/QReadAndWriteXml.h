/******************************************************
作用：保存患者信息及测量结果，格式为xml,
      文件名由患者姓名及编号组成，如(20151028150545(张三).xml)

******************************************************/
#pragma once
#include "stdafx.h"
class QDomDocument;
class QDomNode;

class QReadAndWriteXml
{
public:
	QReadAndWriteXml();
	~QReadAndWriteXml();

	// 功能：保存患者信息及测量结果
	// 参数：tagInfo 患者基本信息及测量结果信息
	void saveInfoAndMeasureValue(const tagPersonInfo& tagInfo);

	// 根据条件查询患者的历史病历
	// 参数：strNo 患者编号
	//       strName 患者姓名
	//       mapIndexToInfo 查询结果与测量信息对应关系
	//       strStartTime 测量区间的开始时间，
	//       strEndTime 测量区间的结束时间  
	QList<QStandardItem *> queryHistoryRecord(const QString &strNo, const QString &strName, QMap<int, tagPersonInfo*> &mapIndexToInfo, 
		                                   const QString &strStartTime = QString(), const QString &strEndTime = QString());

private:
	// 更新患者信息
	void _updateInfo(const tagPersonInfo& tagInfo, QDomDocument &);

	// 更新节点值
	void _updateNodeValue(QDomNode &node, const QString &strValue);

	// 创建Data目录
	void _createDir();
};

