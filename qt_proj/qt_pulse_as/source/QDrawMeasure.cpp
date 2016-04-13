#include "./header/stdafx.h"
#include "./header/QDrawMeasure.h"
#include "./header/QFilterData.h"
#include "./header/QResultEvaluate.h"

// 最大的PWV
const int nMAX_PWV = 2600;
// 最大的ABI值
const float nMAX_ABI = 1.5f;


QDrawMeasure::QDrawMeasure()
{
}


QDrawMeasure::~QDrawMeasure()
{
}

// 绘制个人信息
void QDrawMeasure::drawPersonInfo(QPainter *painter,  tagPersonInfo &info)
{
	if(NULL == painter)
	{
		return;
	}

	QFontMetrics drawMetrics(painter->font());
	int nFontHeight = drawMetrics.height();

	int nNoWidth = drawMetrics.width(QString::fromLocal8Bit("编号:") +  info.m_strNo);
	int nNameWidth =  drawMetrics.width(QString::fromLocal8Bit("姓名:姓名姓"));
	int nDateWidth =  drawMetrics.width(QString::fromLocal8Bit("测量时间: ") + info.m_strDate);
	int nWidth = nNoWidth + nNameWidth + nDateWidth + 10;
	nWidth *= 1.4f;

	int nHeight = 5.5*nFontHeight;
	painter->setWindow(0, 0, nWidth, nHeight);

	_setPenWidthF(*painter);

	float fXPos = 5*1.4f;
	float fYPos = 0.25*nFontHeight;

	painter->drawRect(0, 0, nWidth, nHeight);
	QString strDrawText;
	// 患者信息
	{
		// 编号所在列的开始位置
		float fNoPos = fXPos;
		// 姓名所在列的开始位置
		float fNamePos = 5*1.4f + nNoWidth*1.4 + nDateWidth*0.2;
		// 测量时间所在列的开始位置
		float fDatePos = nWidth - 5*1.5f - nDateWidth;

		strDrawText = QString::fromLocal8Bit("编号:") +  info.m_strNo;
		// 编号
		painter->drawText(fNoPos, fYPos, drawMetrics.width(strDrawText), nFontHeight, Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		// 姓名
		strDrawText = QString::fromLocal8Bit("姓名:") +  info.m_strName;
		painter->drawText(fNamePos, fYPos, drawMetrics.width(strDrawText), nFontHeight, Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		// 测量时间
		strDrawText = QString::fromLocal8Bit("测量时间: ") +  info.m_strDate;
		painter->drawText(fDatePos, fYPos, drawMetrics.width(strDrawText), nFontHeight, Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		fYPos += 1.25f*nFontHeight;
		// 性别
		strDrawText = QString::fromLocal8Bit("性别:") +  info.m_strSex;
		painter->drawText(fNoPos, fYPos, drawMetrics.width(strDrawText),nFontHeight, Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		// 年龄
		strDrawText = QString::fromLocal8Bit("年龄:") +  info.m_strAge + QString::fromLocal8Bit("岁");
		painter->drawText(fNamePos, fYPos,drawMetrics.width(strDrawText), nFontHeight, Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		// 科室
		strDrawText = QString::fromLocal8Bit("    科室: ") +  info.m_strSection;
		painter->drawText(fDatePos, fYPos,drawMetrics.width(strDrawText), nFontHeight, Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		fYPos += 1.25f*nFontHeight;

		// 身高
		strDrawText = QString::fromLocal8Bit("身高:") +  info.m_strHeight + QString::fromLocal8Bit("cm");
		painter->drawText(fNoPos, fYPos,drawMetrics.width(strDrawText), nFontHeight, Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		// 体重
		strDrawText = QString::fromLocal8Bit("体重:") +  info.m_strWeight + QString::fromLocal8Bit("Kg");
		painter->drawText(fNamePos, fYPos, drawMetrics.width(strDrawText), nFontHeight, Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		//医生
		strDrawText = QString::fromLocal8Bit("    医生: ") +  info.m_strDoctor;
		painter->drawText(fDatePos, fYPos, drawMetrics.width(strDrawText), nFontHeight, Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		fYPos += 1.25f*nFontHeight;
		// 病史
		strDrawText = QString::fromLocal8Bit("病史:") +  info.m_listHistory.join(",");
		painter->drawText(fNoPos, fYPos, drawMetrics.width(strDrawText), nFontHeight, Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
	}
}

void QDrawMeasure::drawWaveInfo(QPainter *painter, const tagPersonInfo &info)
{
	if(NULL == painter)
	{
		return;
	}

	painter->setRenderHint(QPainter::Antialiasing);

	_setFont(*painter, 4, true);

	int nViewWidth = painter->viewport().width();
	int nViewHeight = painter->viewport().height();

	int nWidth = 750;
	int nHeight = 120;
	int nYCount = 20;
	int nXCount = nWidth / ((120/20) *(1.0f*nViewHeight/nHeight) / (nViewWidth*1.0f / nWidth));

	QFontMetrics drawMetrics(painter->font());
	int nFontHeight = drawMetrics.height();

	float nDelX = nWidth*1.0f/nXCount;
	float nDelY = nHeight*1.0f/nYCount;
	painter->setWindow(0, 0, nWidth, nHeight);

	_setPenWidthF(*painter);

	painter->save();
	{
		QPen pen;
		pen.setBrush(QColor(97, 97, 122));
		pen.setStyle(Qt::DashLine);
		pen.setWidthF(0.3f);
		painter->setPen(pen);
		for(int i = 1; i<nYCount; ++i)
		{
			painter->drawLine(0, i*nDelY, nWidth, i*nDelY);
		}

		for(int j = 1; j<nXCount; ++j)
		{
			painter->drawLine(j*nDelX, 0, j*nDelX, nHeight);
			//painter->drawRect(j*nDelX, i*nDelY, nDelX, nDelY);
		}
	}
	painter->restore();

	painter->drawRect(0, 0, nWidth, nHeight);

	QPen pen = painter->pen();
	pen.setWidthF(2*pen.widthF());
	painter->setPen(pen);

	// 绘制四肢的波形数据
	QPainterPath pathWave1;
	QPainterPath pathWave2;
	QPainterPath pathWave3;
	QPainterPath pathWave4;
	QString strFileName =  QString::fromLocal8Bit("%1(%2)_2.txt").arg(info.m_strNo).arg(info.m_strName);

	//#if _DEBUG
	//	strFileName = QString::fromLocal8Bit("E:/Lepu_20150810/LepuMedical/SvnFile/MeasureArteriosclerosis(VS)/MeasureArteriosclerosis/MeasureArteriosclerosis/Data/31(tj)_2.txt");
	//#endif
	// 获取波形数据
	QVector<float> leftAnkle;
	QVector<float> rightAnkle;
	QVector<float> leftArm;
	QVector<float> rightArm;
	QVector<int> arrMapValue;
	int nLeftUT = 0;
	int nRightUT = 0;
	QFilterData filterData;
	filterData.getWaveData(strFileName, leftArm, leftAnkle, rightArm, rightAnkle, arrMapValue, nLeftUT, nRightUT);
	int nMinCount = leftAnkle.size();

	if(nMinCount>rightAnkle.size())
	{
		nMinCount = rightAnkle.size();
	}

	if(nMinCount > leftArm.size())
	{
		nMinCount = leftArm.size();
	}

	if(nMinCount>rightArm.size())
	{
		nMinCount = rightArm.size();
	}

	int j = 0;
	float fRightArmPos = nHeight*0.25f;
	float fLeftArmPos = nHeight*0.5f;
	float fRightAnklePos = nHeight*0.75f;
	float fLeftAnklePos = nHeight;
	QPointF rightArmPrePos(0, fRightArmPos);
	QPointF leftArmPrePos(0, fLeftArmPos);
	QPointF rightAnklePrePos(0, fRightAnklePos);
	QPointF leftAnklePrePos(0, fLeftAnklePos);
	for(float i = 0.0f; i < nXCount *nDelX && j < nMinCount; i += nWidth*1.0/nMinCount,++j)
	{
		painter->drawLine(rightArmPrePos ,QPointF(i, fRightArmPos - rightArm.at(j)));
		painter->drawLine(leftArmPrePos, QPointF(i, fLeftArmPos - leftArm.at(j)));
		painter->drawLine(rightAnklePrePos, QPointF(i, fRightAnklePos - rightAnkle.at(j)));
		painter->drawLine(leftAnklePrePos, QPointF(i, fLeftAnklePos - leftAnkle.at(j)));
		rightArmPrePos = QPointF(i, fRightArmPos - rightArm.at(j));
		leftArmPrePos = QPointF(i, fLeftArmPos - leftArm.at(j));
		rightAnklePrePos = QPointF(i, fRightAnklePos - rightAnkle.at(j));
		leftAnklePrePos = QPointF(i, fLeftAnklePos - leftAnkle.at(j));
	}

	QString strText = QString::fromLocal8Bit("右臂");
	painter->drawText(QRectF(5, fRightArmPos - 2*nFontHeight, drawMetrics.width(strText), nFontHeight), Qt::AlignRight, strText);
	strText = QString::fromLocal8Bit("左臂");
	painter->drawText(QRectF(5, fLeftArmPos - 2*nFontHeight, drawMetrics.width(strText), nFontHeight), Qt::AlignRight, strText);
	strText = QString::fromLocal8Bit("右踝");
	painter->drawText(QRectF(5, fRightAnklePos - 3*nFontHeight, drawMetrics.width(strText), nFontHeight), Qt::AlignRight, strText);
	strText = QString::fromLocal8Bit("左踝");
	painter->drawText(QRectF(5, fLeftAnklePos - 3*nFontHeight, drawMetrics.width(strText), nFontHeight), Qt::AlignRight, strText);

	QVector<float> arrTextPos;
	arrTextPos.append(fRightArmPos - nFontHeight);
	arrTextPos.append(fLeftArmPos - nFontHeight);
	arrTextPos.append(fRightAnklePos - 2*nFontHeight);
	arrTextPos.append(fLeftAnklePos - 2*nFontHeight);

	////// 绘制MAP信息
	for(int i = 0;i<4 && arrMapValue.size()==4; ++i)
	{
		QString strMap = QString::fromLocal8Bit("  %MAP: %1%").arg(arrMapValue.at(i));
		painter->drawText(QRectF(5, arrTextPos.at(i), drawMetrics.width(strMap), nFontHeight), Qt::AlignRight, strMap);
	}

	if(arrMapValue.isEmpty())
	{
		for(int i = 0;i<4; ++i)
		{
			QString strMap = QString::fromLocal8Bit("  %MAP: %1%").arg(0);
			painter->drawText(QRectF(5, arrTextPos.at(i), drawMetrics.width(strMap), nFontHeight), Qt::AlignRight, strMap);
		}
	}

	// 绘制UT信息
	{
		QString strUT = QString::fromLocal8Bit("    UT:%1 ms").arg(nRightUT);
		painter->drawText(QRectF(5, fRightAnklePos - nFontHeight, drawMetrics.width(strUT), nFontHeight), Qt::AlignRight,strUT);
		strUT = QString::fromLocal8Bit("    UT:%1 ms").arg(nLeftUT);
		painter->drawText(QRectF(5, fLeftAnklePos - nFontHeight, drawMetrics.width(strUT), nFontHeight), Qt::AlignRight,strUT);
	}
}

void QDrawMeasure::drawPressure(QPainter *painter, tagPersonInfo &info)
{
	if(NULL == painter)
	{
		return;
	}

	QFontMetrics drawMetrics(painter->font());
	int nFontHeight = drawMetrics.height();

	int nRectWidth = drawMetrics.width(QString::fromLocal8Bit(" 收缩压: 100"));
	QPixmap pixBody(":/BodyDemo/Images/Body.png");

	int nPixWidth =  pixBody.width(); 
	int nHeight = 18*nFontHeight;

	//保持原先比例 
	QPixmap scaledMap = pixBody.scaled(nPixWidth, nHeight - 4.5*nFontHeight, Qt::KeepAspectRatio);
	nPixWidth = scaledMap.width();

	int nWidth = nRectWidth*2 + nPixWidth + 10;
	nWidth *= 1.1f;

	painter->setWindow(0, 0, nWidth, nHeight);

	_setPenWidthF(*painter);

	float fXPos = 5*1.1f;
	float fYPos = 0.25*nFontHeight;

	painter->drawRect(0, 0, nWidth, nHeight);

	// 绘制单位
	painter->save();
	QString strDrawText;
	{	
		strDrawText = QString::fromLocal8Bit("测量值");
		painter->drawText(QRectF(fXPos, fYPos, drawMetrics.width(strDrawText), nFontHeight), strDrawText);

		QFont currfont(QString::fromLocal8Bit("宋体"), 6);
		currfont.setBold(true);
		painter->setFont(currfont);
		QFontMetrics currMetrics = QFontMetrics(currfont);
		strDrawText = QString::fromLocal8Bit("血压单位:(mmHg)  PWV单位:(cm/s)");
		painter->drawText(QRectF(nWidth - 5*1.1f - currMetrics.width(strDrawText) , fYPos, currMetrics.width(strDrawText), nFontHeight), strDrawText);
	}
	painter->restore();

	fYPos += 1.25*nFontHeight;
	{
		float fYStart = fYPos;
		painter->drawRoundedRect(fXPos, fYStart, nRectWidth*1.05, 5.5*nFontHeight, 20.0, 15.0);
		fYStart += 0.25*nFontHeight;
		strDrawText = QString::fromLocal8Bit("右臂");
		painter->drawText(fXPos + (nRectWidth - drawMetrics.width(strDrawText))/2, fYStart, 
			drawMetrics.width(strDrawText), nFontHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		fYStart += 1.25*nFontHeight;
		strDrawText = QString::fromLocal8Bit("收缩压: ");
		if(!info.m_listRightArm.isEmpty())
		{
			strDrawText += info.m_listRightArm.at(0).rightJustified(3, ' ');
		}
		painter->drawText(2*fXPos, fYStart, drawMetrics.width(strDrawText), nFontHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		fYStart += 1.25*nFontHeight;
		strDrawText = QString::fromLocal8Bit("平均压: ");
		if(info.m_listRightArm.size() > 1)
		{
			strDrawText += info.m_listRightArm.at(1).rightJustified(3, ' ');
		}
		painter->drawText(2*fXPos , fYStart, drawMetrics.width(strDrawText), nFontHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		fYStart += 1.25*nFontHeight;
		strDrawText = QString::fromLocal8Bit("舒张压: ");
		if(info.m_listRightArm.size() > 2)
		{
			strDrawText += info.m_listRightArm.at(2).rightJustified(3, ' ');
		}
		painter->drawText(2*fXPos , fYStart, drawMetrics.width(strDrawText), nFontHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
	}

	{
		float fYStart = fYPos;
		float fXStart = nWidth - nRectWidth*1.05 - fXPos;
		painter->drawRoundedRect(fXStart, fYStart, nRectWidth*1.05, 5.5*nFontHeight, 20.0, 15.0);
		fYStart += 0.25*nFontHeight;
		strDrawText = QString::fromLocal8Bit("左臂");
		painter->drawText(fXStart + (nRectWidth - drawMetrics.width(strDrawText))/2, fYStart, 
			drawMetrics.width(strDrawText), nFontHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		fYStart += 1.25*nFontHeight;
		fXStart += 5;
		strDrawText = QString::fromLocal8Bit("收缩压: ");
		if(!info.m_listLeftArm.isEmpty())
		{
			strDrawText += info.m_listLeftArm.at(0).rightJustified(3, ' ');
		}
		painter->drawText(fXStart, fYStart, drawMetrics.width(strDrawText), nFontHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		fYStart += 1.25*nFontHeight;
		strDrawText = QString::fromLocal8Bit("平均压: ");
		if(info.m_listLeftArm.size() > 1)
		{
			strDrawText += info.m_listLeftArm.at(1).rightJustified(3, ' ');
		}
		painter->drawText(fXStart , fYStart, drawMetrics.width(strDrawText), nFontHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		fYStart += 1.25*nFontHeight;
		strDrawText = QString::fromLocal8Bit("舒张压: ");
		if(info.m_listLeftArm.size() > 2)
		{
			strDrawText += info.m_listLeftArm.at(2).rightJustified(3, ' ');
		}
		painter->drawText(fXStart , fYStart, drawMetrics.width(strDrawText), nFontHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
	}

	painter->drawPixmap(nRectWidth + fXPos + (nWidth - 2*nRectWidth - fXPos*2 - nPixWidth)/2, fYPos, scaledMap.width(), nHeight - 4.5*nFontHeight, scaledMap);

	fYPos += 5.75*nFontHeight;
	// 绘制右踝测量结果
	{
		float fYStart = fYPos;
		painter->drawRoundedRect(fXPos, fYStart, nRectWidth*1.05, 7.75*nFontHeight, 20.0, 15.0);

		fYStart += 0.25 *nFontHeight;
		strDrawText = QString::fromLocal8Bit("右踝");
		painter->drawText(fXPos + (nRectWidth - drawMetrics.width(strDrawText))/2, fYStart,
			drawMetrics.width(strDrawText), nFontHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		fYStart += 1.25 *nFontHeight;
		strDrawText = QString::fromLocal8Bit("收缩压: ");
		if(!info.m_listRightAnkle.isEmpty())
		{
			strDrawText += info.m_listRightAnkle.at(0).rightJustified(3, ' ');
		}
		painter->drawText(2*fXPos, fYStart, drawMetrics.width(strDrawText), 
			nFontHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		fYStart += 1.25 *nFontHeight;
		strDrawText = QString::fromLocal8Bit("平均压: ");
		if(info.m_listRightAnkle.size() >= 2)
		{
			strDrawText += info.m_listRightAnkle.at(1).rightJustified(3, ' ');
		}
		painter->drawText(2*fXPos , fYStart, 
			drawMetrics.width(strDrawText), nFontHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		fYStart += 1.25 *nFontHeight;
		strDrawText = QString::fromLocal8Bit("舒张压: ");
		if(info.m_listRightAnkle.size() >= 3)
		{
			strDrawText += info.m_listRightAnkle.at(2).rightJustified(3, ' ');
		}
		painter->drawText(2*fXPos, fYStart, 
			drawMetrics.width(strDrawText), nFontHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		fYStart += 1.25 *nFontHeight;
		strDrawText = QString::fromLocal8Bit("   ABI:");
		if(info.m_listRightAnkle.size() >= 4)
		{
			strDrawText += info.m_listRightAnkle.at(3).rightJustified(4, ' ');
		}
		painter->drawText(2*fXPos , fYStart, 
			drawMetrics.width(strDrawText), nFontHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		fYStart += 1.25 *nFontHeight;
		strDrawText = QString::fromLocal8Bit("   PWV:");
		if(info.m_listRightAnkle.size()  >= 5)
		{
			strDrawText += info.m_listRightAnkle.at(4).rightJustified(4, ' ');
		}
		painter->drawText(2*fXPos , fYStart, 
			drawMetrics.width(strDrawText), nFontHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
	}

	// 绘制左踝测量结果
	{
		float fYStart = fYPos;
		float fXStart = nWidth - nRectWidth*1.05 - fXPos;
		painter->drawRoundedRect(fXStart, fYStart, nRectWidth*1.05, 7.75*nFontHeight, 20.0, 15.0);

		fYStart += 0.25 *nFontHeight;
		strDrawText = QString::fromLocal8Bit("左踝");
		painter->drawText(fXStart + (nRectWidth - drawMetrics.width(strDrawText))/2, fYStart,
			drawMetrics.width(strDrawText), nFontHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		fYStart += 1.25 *nFontHeight;
		fXStart += 5;
		strDrawText = QString::fromLocal8Bit("收缩压: ");
		if(!info.m_listLeftAnkle.isEmpty())
		{
			strDrawText += info.m_listLeftAnkle.at(0).rightJustified(3, ' ');
		}
		painter->drawText(fXStart, fYStart, drawMetrics.width(strDrawText), 
			nFontHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		fYStart += 1.25 *nFontHeight;
		strDrawText = QString::fromLocal8Bit("平均压: ");
		if(info.m_listLeftAnkle.size() >= 2)
		{
			strDrawText += info.m_listLeftAnkle.at(1).rightJustified(3, ' ');
		}
		painter->drawText(fXStart , fYStart, 
			drawMetrics.width(strDrawText), nFontHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		fYStart += 1.25 *nFontHeight;
		strDrawText = QString::fromLocal8Bit("舒张压: ");
		if(info.m_listLeftAnkle.size() >= 3)
		{
			strDrawText += info.m_listLeftAnkle.at(2).rightJustified(3, ' ');
		}
		painter->drawText(fXStart, fYStart, 
			drawMetrics.width(strDrawText), nFontHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		fYStart += 1.25 *nFontHeight;
		strDrawText = QString::fromLocal8Bit("   ABI:");
		if(info.m_listLeftAnkle.size() >= 4)
		{
			strDrawText += info.m_listLeftAnkle.at(3).rightJustified(4, ' ');
		}
		painter->drawText(fXStart , fYStart, 
			drawMetrics.width(strDrawText), nFontHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		fYStart += 1.25 *nFontHeight;
		strDrawText = QString::fromLocal8Bit("   PWV:");
		if(info.m_listLeftAnkle.size()  >= 5)
		{
			strDrawText += info.m_listLeftAnkle.at(4).rightJustified(4, ' ');
		}
		painter->drawText(fXStart , fYStart, 
			drawMetrics.width(strDrawText), nFontHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
	}

	fYPos += 8.25*nFontHeight;
	// 绘制参考值
	{
		float fLeft = fXPos + 5;
		strDrawText = QString::fromLocal8Bit("参考值:");
		painter->drawText(fLeft , fYPos, drawMetrics.width(strDrawText), nFontHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		fYPos += 1.25*nFontHeight;
		strDrawText = QString::fromLocal8Bit("ABI: 0.9-1.4  PWV:800-1400");
		painter->drawText(fLeft , fYPos, drawMetrics.width(strDrawText), nFontHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
	}
}

// 绘制结论部分
void QDrawMeasure::drawResult(QPainter *painter, tagPersonInfo &info)
{
	if(NULL == painter)
	{
		return;
	}

	_setFont(*painter, 4, true);

	QFontMetrics drawMetrics(painter->font());
	int nFontHeight = drawMetrics.height();

	QResultEvaluate evaluate;

#if _DEBUG
	double dRightPWV = info.m_listRightAnkle.at(4).toDouble();
	double dRightABI = info.m_listRightAnkle.at(3).toDouble();
	double dLeftPWV = info.m_listLeftAnkle.at(4).toDouble();
	double dLeftABI = info.m_listLeftAnkle.at(3).toDouble();
	info.m_strDiagnose = evaluate.getEvaluate(info.m_strAge.toInt(), dLeftABI, dRightABI, dLeftPWV, dRightPWV
		, info.m_strSex == QString::fromLocal8Bit("男"));
#endif

	// 取出结论中最长的宽度
	int nWidth = 0;
	QStringList listRet = info.m_strDiagnose.split("\n");
	for(int i=0; i<listRet.size(); ++i)
	{
		int nTextWidth = drawMetrics.width(listRet.at(i));
		nWidth = nTextWidth > nWidth ? nTextWidth : nWidth;
	}

	QStringList listDescription = evaluate.getDescription().split("\n");
	for(int i=0; i<listDescription.size(); ++i)
	{
		int nTextWidth = drawMetrics.width(listDescription.at(i));
		nWidth = nTextWidth > nWidth ? nTextWidth : nWidth;
	}

	// 设置窗口坐标
	int nHeight = (listRet.size() + 8)*nFontHeight;
	painter->setWindow(0, 0, nWidth, nHeight);

	_setPenWidthF(*painter);

	painter->drawRect(0, 0, nWidth, nHeight);

	// 描绘结论
	float fYPos = 0.25*nFontHeight;
	if(info.m_strDiagnose.isEmpty())
	{
		painter->drawText(5, fYPos, nWidth, nFontHeight, Qt::AlignLeft |  Qt::AlignVCenter,  QString::fromLocal8Bit("结论:"));
	}
	else
	{
		painter->drawText(5, fYPos, nWidth, nFontHeight, Qt::AlignLeft |  Qt::AlignVCenter,  QString::fromLocal8Bit("结论:"));
		fYPos += nFontHeight;
		for(int i = 0; i <listRet.size(); ++i)
		{
			painter->drawText(5, fYPos + i*nFontHeight, nWidth, nFontHeight, Qt::AlignLeft |  Qt::AlignVCenter,  listRet.at(i));
		}
	}

	painter->save();
	{
		QFont font(painter->font());
		font.setPointSizeF(font.pointSizeF()*0.8);
		painter->setFont(font);
		QFontMetrics drawMetrics(painter->font());
		int nTempHeight = drawMetrics.height();
		for(int i = 0; i <listDescription.size(); ++i)
		{
			painter->drawText(5, nHeight - 4*nFontHeight-2*nTempHeight+i*nTempHeight, nWidth, nTempHeight, Qt::AlignLeft |  Qt::AlignVCenter,  listDescription.at(i));
		}
		//painter->drawText(5, fYPos + listRet.size()*nFontHeight + nFontHeight, evaluate.getDescription());
	}
	painter->restore();

	//描绘医师签名
	QString strDrawText = QString::fromLocal8Bit("医师签名:              ");
	painter->drawText(nWidth - drawMetrics.width(strDrawText) - 10, nHeight - 3.75*nFontHeight,
		drawMetrics.width(strDrawText), nFontHeight, Qt::AlignLeft |  Qt::AlignVCenter,  strDrawText);

	// 日期
	strDrawText = QString::fromLocal8Bit("        日期:    年  月  日");
	painter->drawText(nWidth - drawMetrics.width(strDrawText) - 10, nHeight - 2.5*nFontHeight,
		drawMetrics.width(strDrawText), nFontHeight, Qt::AlignLeft |  Qt::AlignVCenter,  strDrawText);

	// 下次测量时间
	strDrawText = QString::fromLocal8Bit("下次检查日期:    年  月  日");
	painter->drawText(nWidth - drawMetrics.width(strDrawText) - 10, nHeight - 1.25*nFontHeight,
		drawMetrics.width(strDrawText), nFontHeight, Qt::AlignLeft |  Qt::AlignVCenter,  strDrawText);
}

void QDrawMeasure::drawAnalysis(QPainter *painter, tagPersonInfo &info, bool bDrawRect)
{
	if(NULL == painter)
	{
		return;
	}

	//// 设置字体
	//QFont font(QStringLiteral("宋体"), 3);
	//font.setBold(true);
	//painter->setFont(font);
	_setFont(*painter, 3, true);

	QFontMetrics metrics = painter->fontMetrics();
	int nTextHeight = metrics.height();
	int nLeftWidth = metrics.width("2400 ");
	int nRightWidth = metrics.width("  ABI");

	int nWidth = 100 + nLeftWidth + nRightWidth;

	float fRatio = painter->viewport().width()*1.0/painter->viewport().height();
	painter->setWindow(0, 0, nWidth+5, nWidth+5);

	_setPenWidthF(*painter);

	if (bDrawRect)
	{
		painter->drawRect(painter->window());
	}

	int nXStartMin = nLeftWidth;
	int nYMax = nWidth - 2*nTextHeight;

	// X坐标的等分
	int nXCount = 11;

	// Y坐标的等分
	int nYCount = 9;

	// x坐标上每分的宽度
	float deltaX = 100*1.0f / nXCount;

	// y坐标上每分的宽度
	float deltaY = nYMax*1.0f / nYCount;

	// 绘制ABI在0.8以下，PWV在800-2600
	QPainterPath pathLine;
	QPointF pointTop = _getPointByABIAndPWV(0.0f, nMAX_PWV, nTextHeight, deltaX, deltaY, nXStartMin, nYMax);
	QPointF pointBottom = _getPointByABIAndPWV(0.8f, 800, nTextHeight, deltaX, deltaY, nXStartMin, nYMax);
	pathLine.setFillRule(Qt::WindingFill);
	pathLine.addRect(QRectF(pointTop, pointBottom));
	painter->setBrush(QBrush(QColor(157,127,215, 255)));
	painter->drawPath(pathLine);

	// 绘制ABI在0.8-0.9，PWV在800-2600
	QPainterPath pathLessNormal;
	pointTop = _getPointByABIAndPWV(0.8f, nMAX_PWV, nTextHeight, deltaX, deltaY, nXStartMin, nYMax);
	pointBottom = _getPointByABIAndPWV(0.9f, 800, nTextHeight, deltaX, deltaY, nXStartMin, nYMax);
	pathLessNormal.setFillRule(Qt::WindingFill);
	pathLessNormal.addRect(QRectF(pointTop, pointBottom));
	painter->setBrush(QBrush(QColor(157,127,215, 100)));
	painter->drawPath(pathLessNormal);

	// 绘制ABI在0.9-1.4，PWV在800-1400
	QPainterPath pathNormal;
	pointTop = _getPointByABIAndPWV(0.9f, 1200, nTextHeight, deltaX, deltaY, nXStartMin, nYMax);
	pointBottom = _getPointByABIAndPWV(1.4f, 800, nTextHeight, deltaX, deltaY, nXStartMin, nYMax);
	pathNormal.setFillRule(Qt::WindingFill);
	pathNormal.addRect(QRectF(pointTop, pointBottom));
	painter->setBrush(QBrush(QColor(252,252,252, 255)));
	painter->drawPath(pathNormal);

	// 绘制ABI在0.9-1.4，PWV在1200-1400
	QPainterPath pathPoint;
	pointTop = _getPointByABIAndPWV(0.9f, 1400, nTextHeight, deltaX, deltaY, nXStartMin, nYMax);
	pointBottom = _getPointByABIAndPWV(1.4f, 1200, nTextHeight, deltaX, deltaY, nXStartMin, nYMax);
	pathPoint.setFillRule(Qt::WindingFill);
	pathPoint.addRect(QRectF(pointTop, pointBottom));
	painter->setBrush(QBrush(QColor(203, 219, 234, 255)));
	painter->drawPath(pathPoint);

	// 绘制ABI在0.9-1.4，PWV在1400-1700
	painter->save();
	QPainterPath pathHighPoint;
	pointTop = _getPointByABIAndPWV(0.9f, 1700, nTextHeight, deltaX, deltaY, nXStartMin, nYMax);
	pointBottom = _getPointByABIAndPWV(1.4f, 1400, nTextHeight, deltaX, deltaY, nXStartMin, nYMax);
	pathHighPoint.setFillRule(Qt::WindingFill);
	pathHighPoint.addRect(QRectF(pointTop, pointBottom));
	painter->setPen(Qt::NoPen);
	painter->setBrush(QBrush(QColor(146, 165, 198, 255)));
	painter->drawPath(pathHighPoint);

	// 绘制ABI在0.9-1.4，PWV在1700-2000
	QPainterPath pathHighPoint1;
	pointTop = _getPointByABIAndPWV(0.9f, 2000, nTextHeight, deltaX, deltaY, nXStartMin, nYMax);
	pointBottom = _getPointByABIAndPWV(1.4f, 1700, nTextHeight, deltaX, deltaY, nXStartMin, nYMax);
	pathHighPoint1.setFillRule(Qt::WindingFill);
	pathHighPoint1.addRect(QRectF(pointTop, pointBottom));
	painter->setPen(Qt::NoPen);
	painter->setBrush(QBrush(QColor(99, 126, 173, 255)));
	painter->drawPath(pathHighPoint1);
	painter->restore();

	// 绘制ABI在0.9-1.4，PWV在2000-2600
	QPainterPath pathHighPoint2;
	pointTop = _getPointByABIAndPWV(0.9f, nMAX_PWV, nTextHeight, deltaX, deltaY, nXStartMin, nYMax);
	pointBottom = _getPointByABIAndPWV(1.4f, 2000, nTextHeight, deltaX, deltaY, nXStartMin, nYMax);
	pathHighPoint2.setFillRule(Qt::WindingFill);
	pathHighPoint2.addRect(QRectF(pointTop, pointBottom));
	painter->setBrush(QBrush(QColor(255, 255, 4, 255)));
	painter->drawPath(pathHighPoint2);

	// 绘制ABI大于1.4的情况
	QPainterPath pathMore;
	pointTop = _getPointByABIAndPWV(1.4f, nMAX_PWV, nTextHeight, deltaX, deltaY, nXStartMin, nYMax);
	pointBottom = _getPointByABIAndPWV(1.5f, 800, nTextHeight, deltaX, deltaY, nXStartMin, nYMax);
	pathMore.setFillRule(Qt::WindingFill);
	pathMore.addRect(QRectF(pointTop, pointBottom));
	painter->setBrush(QBrush(QColor(157,127,215, 255)));
	painter->drawPath(pathMore);

	float fMinABI = 0.5f;
	int nMinBaPWV = 800;
	int nYTextIndex = 0;
	// 绘制单元格
	QPainterPath *path = new QPainterPath();
	for (int i = 0; i < nXCount; ++i) {
		for (int j = 0; j <nYCount; ++j)
		{
			if(j == 0 && nMinBaPWV <= nMAX_PWV)
			{
				QString strPWVvalue = QString("%1").arg(nMinBaPWV);
				int nstrWidth = metrics.width(strPWVvalue);
				painter->drawText(QPointF(nXStartMin-nstrWidth, nYMax + 2 +nTextHeight - (deltaY * (nYTextIndex) - metrics.ascent()/2)),strPWVvalue);
				nMinBaPWV += 200;
				++nYTextIndex;
			}
			path->addRect(nXStartMin + deltaX * i, nTextHeight + 2 +  (deltaY * (j)), deltaX,  deltaY);
		}
		
		if(i %2 != 0)
		{
			fMinABI += 0.1f;
			continue;
		}

		QString strABIValue = QString::number(fMinABI, 'f', 1);
		int nStringWidth = metrics.width(strABIValue);
		// 绘制ABI的位置
		int nABIPos = nXStartMin+deltaX * (i+1) - nStringWidth/2;
		painter->drawText(nABIPos, nTextHeight + deltaY*nYCount +2 , metrics.width(strABIValue), nTextHeight, Qt::AlignCenter,
		strABIValue);
		fMinABI += 0.1f;
	}

	painter->setBrush(Qt::NoBrush);
	painter->drawPath(*path);
	delete path;
	path = NULL;

	int nLabelHeight = 2;
	// 绘制单位:baPWV(cm/s)
	painter->drawText(nXStartMin, nLabelHeight, metrics.width(QStringLiteral("baPWV(cm/s)")), nTextHeight, Qt::AlignCenter,
		QStringLiteral("baPWV(cm/s)"));

	// 绘制标识
	QString strText = QString::fromLocal8Bit("右");
	float fRightTextPos = nLeftWidth - nRightWidth + metrics.width(QStringLiteral("baPWV(cm/s)")) + ((nWidth - nLeftWidth - metrics.width(QStringLiteral("baPWV(cm/s)")) - metrics.width(strText)*2-6)/2);
	painter->drawText(fRightTextPos, nLabelHeight, metrics.width(strText), nTextHeight, Qt::AlignCenter, strText);

	strText = QString::fromLocal8Bit("左  ");
	float fLeftTextPos = nWidth - nRightWidth - metrics.width(strText) - 3;
	painter->drawText(fLeftTextPos, nLabelHeight, metrics.width(strText), nTextHeight, Qt::AlignCenter, strText);

	strText = QString::fromLocal8Bit("  ABI");
	painter->drawText(nWidth - metrics.width(strText),  nTextHeight + 2 + deltaY*nYCount, metrics.width(strText), nTextHeight, Qt::AlignCenter,
		strText);

	// 若没有测量过，则返回
	if(info.m_listLeftArm.isEmpty())
	{
		QPen penEllipse;
		penEllipse.setWidth(0.1f);
		penEllipse.setColor(Qt::black);
		painter->setPen(penEllipse);
		painter->setBrush(QColor(201,101,98,255));
		painter->drawEllipse(QPointF(fLeftTextPos + metrics.width(QString::fromLocal8Bit("左 ")), nLabelHeight+1), 3, 3);
		painter->setBrush(QColor(10,32,165, 255));
		painter->drawEllipse(QPointF(fRightTextPos + metrics.width(QString::fromLocal8Bit("右 ")), nLabelHeight+1), 3, 3);
		return;
	}

	int nLeftPWV = 800;
	int nRightPWV = 800;
	float fLeftABI = 0.0f;
	float fRightABI = 0.0f;
	if(info.m_listLeftAnkle.size()>4)
	{
		nLeftPWV = info.m_listLeftAnkle.at(4).toInt();
		fLeftABI = info.m_listLeftAnkle.at(3).toFloat();
	}

	if(info.m_listRightAnkle.size()>4)
	{
		nRightPWV = info.m_listRightAnkle.at(4).toInt();
		fRightABI = info.m_listRightAnkle.at(3).toFloat();
	}

	// 设置最大最小值
	if(nLeftPWV > nMAX_PWV)
	{
		nLeftPWV = nMAX_PWV;
	}

	if(nRightPWV > nMAX_PWV)
	{
		nRightPWV = nMAX_PWV;
	}

	if(nLeftPWV < 800)
	{
		nLeftPWV = 800;
	}

	if(nRightPWV < 800)
	{
		nRightPWV = 800;
	}

	if(fLeftABI < 0.4)
	{
		fLeftABI = 0.4;
	}

	if(fRightABI < 0.4)
	{
		fRightABI = 0.4;
	}

	if(fLeftABI < 0.4)
	{
		fLeftABI = 0.4;
	}

	if(fRightABI < 0.4)
	{
		fRightABI = 0.4;
	}

	QPen pen;
	pen.setWidth(0.1f);
	pen.setColor(Qt::black);
	painter->setPen(pen);
	painter->setBrush(QColor(201,101,98,255));

	QPointF point = _getPointByABIAndPWV(fLeftABI, nLeftPWV, nTextHeight, deltaX, deltaY, nXStartMin, nYMax);
	painter->drawEllipse(point, 3, 3);
	painter->drawEllipse(QPointF(fLeftTextPos + metrics.width(QString::fromLocal8Bit("左 ")), nLabelHeight+1), 3, 3);

	painter->setBrush(QColor(10,32,165, 255));
	point = _getPointByABIAndPWV(fRightABI, nRightPWV, nTextHeight, deltaX, deltaY, nXStartMin, nYMax);
	painter->drawEllipse(point, 3, 3);
	painter->drawEllipse(QPointF(fRightTextPos + metrics.width(QString::fromLocal8Bit("右 ")), nLabelHeight+1), 3, 3);
}

QPointF QDrawMeasure::_getPointByABIAndPWV(const float &fABI, const int &nPWV, const int &nFontHeight, const float &nDeltaX, const float &nDeltaY, const int &nStartX, const int &nHeight)
{
	float fLeftABIPos = nStartX + 
		((fABI - 0.5f) / 0.1f)*nDeltaX + nDeltaX;
	float fLeftPWV = nHeight   - ((nPWV - 800)/200.0f)*nDeltaY + nFontHeight +2;

	// 低于最小值，设定默认值
	if(fLeftABIPos < nStartX)
	{
		fLeftABIPos = nStartX;
	}

	// X轴设置最大值
	if(fLeftABIPos>nStartX + 11 *nDeltaX)
	{
		fLeftABIPos = nStartX + 11 *nDeltaX;
	}

	return QPointF(fLeftABIPos, fLeftPWV);
}

void QDrawMeasure::drawPwvAnalysis(QPainter *painter, tagPersonInfo &info)
{
	if(NULL == painter)
	{
		return;
	}

	// 设置字体
	_setFont(*painter, 3, false);

	QFontMetrics metrics = painter->fontMetrics();
	int nTextHeight = metrics.height();

	int nLeftWidth = metrics.width(QString::fromLocal8Bit("2400 "));
	int nRightWidth = metrics.width(QString::fromLocal8Bit("平均值 "));

	int nWidth = 100 + nLeftWidth + nRightWidth;
	painter->setWindow(0, 0, nWidth, nWidth);

	_setPenWidthF(*painter);
	
	painter->setRenderHint(QPainter::Antialiasing);
	painter->drawRect(0, 0, nWidth, nWidth);

	// 绘制标题
	{
		QString strText = QString::fromLocal8Bit("动脉硬化分析(baPWV)");
		painter->drawText(QPointF((nWidth-metrics.width(strText))/2.0f, 1.25*nTextHeight), strText);
	}

	// 默认为男性
	bool bIsMen = true;
	if(info.m_strSex.compare(QString::fromLocal8Bit("女")) == 0)
	{
		bIsMen = false;
	}

	// X轴刻度的个数
	int nXCount = 7;
	// Y周刻度的个数
	int nYCount = 9;

	// x坐标上每份的宽度
	float deltaX = 100.0f / nXCount;

	// y坐标上每份的宽度
	float deltaY = (nWidth - 8.0f*nTextHeight) / nYCount;

	int nStartAge = 20;
	int nMinBaPWV = 800;

	int nXStartMin = nLeftWidth;
	float fStartY = nWidth - 4.0f*nTextHeight;

	// 绘制单元格
	QPainterPath *path = new QPainterPath();
	for (int i = 0; i < nXCount; ++i) {
		for (int j = 0; j < nYCount; ++j)
		{
			path->addRect(nXStartMin + deltaX * i, fStartY - (deltaY * (j+1)), deltaX,  deltaY);
		}
	}

	// 绘制Y周刻度值
	for(int i = 0; i<=nYCount; ++i)
	{
		QString strPWVvalue = QString("%1").arg(nMinBaPWV);
		strPWVvalue.rightJustified(4, ' ');
		int nstrWidth = metrics.width(strPWVvalue);
		painter->drawText(QPointF(nXStartMin - nstrWidth - 2, 
			fStartY - (deltaY * (i) - metrics.ascent()/2)),
			strPWVvalue);
		nMinBaPWV += 200;
	}

	// 绘制X周刻度值
	for(int i = 0; i<=nXCount; ++i)
	{
		QString strAgeValue = QString::number(nStartAge);
		int nStringWidth = metrics.width(strAgeValue);
		// 年龄开始的位置
		int nAgePos = nXStartMin+deltaX * i - nStringWidth/2;
		painter->drawText(nAgePos, fStartY + nTextHeight, strAgeValue);
		nStartAge += 10;
	}

	QVector<QPointF> pointAveragePWV;
	QVector<QPointF> pointN1SDPWV;
	QVector<QPointF> pointP1SDPWV;
	QVector<QPointF> pointP2SDPWV;

	QPointF point2SDTextPos;
	QPointF point1SDTextPos;
	QPointF pointAverageTextPos;
	QPointF pointN1SDTextPos;

	painter->save();
	// 填充颜色
	{
		painter->setPen(Qt::NoPen);
		painter->setBrush(QBrush(QColor(220, 220, 220, 255)));

		QResultEvaluate evaluate;
		double dN1SDPWV = 0.0;
		double dAveragePWV = 0.0;
		double dP1SDPWV = 0.0;
		double dP2SDPWV = 0.0;

		for(int i = 20; i <=90; ++i)
		{
			evaluate.getPWVByAgeAndSex(i, bIsMen, dN1SDPWV, dAveragePWV, dP1SDPWV, dP2SDPWV);
			pointN1SDPWV.append(_getPointByPWVAndAge(dN1SDPWV, i,nXStartMin, fStartY, deltaX, deltaY));
			pointP1SDPWV.append(_getPointByPWVAndAge(dP1SDPWV, i,nXStartMin, fStartY, deltaX, deltaY));
			pointP2SDPWV.append(_getPointByPWVAndAge(dP2SDPWV, i,nXStartMin, fStartY, deltaX, deltaY));
			pointAveragePWV.append(_getPointByPWVAndAge(dAveragePWV, i,nXStartMin, fStartY, deltaX, deltaY));
		}

		point2SDTextPos = pointP2SDPWV.last();
		point1SDTextPos = pointP1SDPWV.last();
		pointAverageTextPos = pointAveragePWV.last();
		pointN1SDTextPos = pointN1SDPWV.last();

		for(int i = pointP2SDPWV.size(); i >=1; --i)
		{
			pointP1SDPWV.append(pointP2SDPWV.at(i-1));
		}
		pointN1SDPWV.append(_getPointByPWVAndAge(800, 90, nXStartMin, fStartY, deltaX, deltaY));
		pointN1SDPWV.append(_getPointByPWVAndAge(800, 20, nXStartMin, fStartY, deltaX, deltaY));

		//painter->drawPath(pathN1SD);
		painter->drawPolygon(QPolygonF(pointN1SDPWV));

		// 绘制稍硬
		painter->setBrush(QBrush(QColor(120, 120, 120, 255)));
		painter->drawPolygon(QPolygonF(pointP1SDPWV));

		// 绘制硬
		painter->setBrush(QBrush(QColor(60, 60, 60, 255)));
		pointP2SDPWV.append(_getPointByPWVAndAge(2600, 90, nXStartMin, fStartY, deltaX, deltaY));
		pointP2SDPWV.append(_getPointByPWVAndAge(2600, 20, nXStartMin, fStartY, deltaX, deltaY));
		painter->drawPolygon(QPolygonF(pointP2SDPWV));
		//QPainterPath pathNormal;
		//pathNormal.setFillRule(Qt::WindingFill);
		//QPointF point1400PWV20Age = _getPointByPWVAndAge(1400, 20, nXStartMin, fStartY, deltaX, deltaY);
		//QPointF point800PWV90Age = _getPointByPWVAndAge(800, 90, nXStartMin, fStartY, deltaX, deltaY);
		//pathNormal.addRect(QRectF(point1400PWV20Age, point800PWV90Age));
		//painter->drawPath(pathNormal);

		//QPainterPath pathLessNormal;
		//pathLessNormal.setFillRule(Qt::WindingFill);
		//painter->setBrush(QBrush(QColor(220, 220, 220, 255)));
		//QPointF point1600PWV20Age = _getPointByPWVAndAge(1600, 20, nXStartMin, fStartY, deltaX, deltaY);
		//QPointF point1400PWV90Age = _getPointByPWVAndAge(1400, 90, nXStartMin, fStartY, deltaX, deltaY);
		//pathLessNormal.addRect(QRectF(point1600PWV20Age, point1400PWV90Age));
		//painter->drawPath(pathLessNormal);

		//QPainterPath pathBad;
		//pathBad.setFillRule(Qt::WindingFill);
		//painter->setBrush(QBrush(QColor(160, 160, 160, 255)));
		//QPointF point2000PWV20Age = _getPointByPWVAndAge(2000, 20, nXStartMin, fStartY, deltaX, deltaY);
		//QPointF point1600PWV90Age = _getPointByPWVAndAge(1600, 90, nXStartMin, fStartY, deltaX, deltaY);
		//pathBad.addRect(QRectF(point2000PWV20Age, point1600PWV90Age));
		//painter->drawPath(pathBad);

		//QPainterPath pathVeryBad;
		//pathVeryBad.setFillRule(Qt::WindingFill);
		//painter->setBrush(QBrush(QColor(120, 120, 120, 255)));
		//QPointF point2600PWV20Age = _getPointByPWVAndAge(2600, 20, nXStartMin, fStartY, deltaX, deltaY);
		//QPointF point2000PWV90Age = _getPointByPWVAndAge(2000, 90, nXStartMin, fStartY, deltaX, deltaY);
		//pathVeryBad.addRect(QRectF(point2600PWV20Age, point2000PWV90Age));
		//painter->drawPath(pathVeryBad);
	}
	painter->restore();

	painter->drawPath(*path);
	delete path;
	path = NULL;

	// 绘制平均线
	{
		painter->save();
		{
			QPen pen(Qt::SolidLine);
			pen.setWidthF(painter->pen().widthF()*1.5f);
			painter->setPen(pen);
			pointAveragePWV.append(pointAveragePWV.last());
			painter->drawPolyline(QPolygonF(pointAveragePWV));
		}
		painter->restore();

		// 绘制曲线的标识信息：+1SD、平均值、-1SD、2SD
		{
			QString strText = QString::fromLocal8Bit("+2SD");
			painter->drawText(point2SDTextPos.x() , point2SDTextPos.y() + metrics.ascent()/2, strText);

			strText = QString::fromLocal8Bit("+1SD");
			painter->drawText(point1SDTextPos.x() , point1SDTextPos.y() + metrics.ascent()/2, strText);

			strText = QString::fromLocal8Bit("平均值");
			painter->drawText(pointAverageTextPos.x() , pointAverageTextPos.y() + metrics.ascent()/2, strText);

			strText = QString::fromLocal8Bit("-1SD");
			painter->drawText(pointN1SDTextPos.x() , pointN1SDTextPos.y() + metrics.ascent()/2, strText);
		}
	}

	// 绘制年龄
	{
		QString strText = QString::fromLocal8Bit("(年龄)");
		painter->drawText(QPointF(nWidth - metrics.width(strText), fStartY + nTextHeight),  strText);
	}

	// 绘制baPWV(cm/s)
	float fTextYPos = fStartY - deltaY * (nYCount) - 0.5f*nTextHeight; 
	painter->drawText(QPointF(nXStartMin, fTextYPos),
		"baPWV(cm/s)");

	// 绘制三角形标识信息
	QString strText = QString::fromLocal8Bit("右");
	float fPWVWidth = metrics.width("baPWV(cm/s)");
	float fTextWidth = metrics.width(strText);
	float fRightPos = deltaX * nXCount + nXStartMin  - fPWVWidth - fTextWidth*2 - nTextHeight*2;
	float fTextXPos = nXStartMin + fRightPos/2.0f + fPWVWidth;
	painter->drawText(QPointF(fTextXPos, fTextYPos),
		strText);

	painter->save();
	{
		QPen pen(Qt::SolidLine);
		pen.setWidthF(0.5f);
		painter->setPen(pen);
		QPointF pointRight[3] = 
		{
			QPointF(fTextXPos + fTextWidth, fTextYPos - nTextHeight/2.0f),
			QPointF(fTextXPos + fTextWidth + nTextHeight/2.0f, fTextYPos - nTextHeight),
			QPointF(fTextXPos + fTextWidth + nTextHeight/2.0f, fTextYPos)
		};
		painter->drawPolygon(pointRight, 3);


		if(info.m_listRightAnkle.size()>4)
		{
			float fPWV = info.m_listRightAnkle.at(4).toFloat();
			int nAge = info.m_strAge.toInt();
			QPointF pointMeasure = _getPointByPWVAndAge(fPWV, nAge, nXStartMin, fStartY, deltaX, deltaY);
			QPointF pointAngle[3] = 
			{
				QPointF(pointMeasure.x() - nTextHeight/2.0f, pointMeasure.y()),
				QPointF(pointMeasure.x() + nTextHeight/2.0f, pointMeasure.y()  + nTextHeight/2.0f),
				QPointF(pointMeasure.x() + nTextHeight/2.0f, pointMeasure.y()  - nTextHeight/2.0f)
			};
			painter->drawPolygon(pointAngle, 3);
		}
	}
	painter->restore();

	strText = QString::fromLocal8Bit("左");
	fTextXPos = nXStartMin + deltaX * nXCount - fTextWidth - nTextHeight;
	painter->drawText(QPointF(fTextXPos, fTextYPos),
		strText);

	painter->save();
	{
		QPen pen(Qt::SolidLine);
		pen.setWidthF(0.5f);
		painter->setPen(pen);
		painter->setBrush(Qt::black);

		QPointF pointLeft[3] = 
		{
			QPointF(fTextXPos + fTextWidth, fTextYPos - nTextHeight),
			QPointF(fTextXPos + fTextWidth, fTextYPos),
			QPointF(fTextXPos + fTextWidth + nTextHeight/2.0f, fTextYPos - nTextHeight/2.0f)
		};

		painter->drawPolygon(pointLeft, 3);
		if(info.m_listLeftAnkle.size() > 4)
		{
			float fPWV = info.m_listLeftAnkle.at(4).toFloat();
			int nAge = info.m_strAge.toInt();
			QPointF pointMeasure = _getPointByPWVAndAge(fPWV, nAge, nXStartMin, fStartY, deltaX, deltaY);
			QPointF pointAngle[3] = 
			{
				QPointF(pointMeasure.x() - nTextHeight/2.0f, pointMeasure.y()  + nTextHeight/2.0f),
				QPointF(pointMeasure.x() - nTextHeight/2.0f, pointMeasure.y() - nTextHeight/2.0f),
				QPointF(pointMeasure.x() + nTextHeight/2.0f, pointMeasure.y())
			};
			painter->drawPolygon(pointAngle, 3);
		}
	}
	painter->restore();

	QString strValue = QString::fromLocal8Bit("baPWV(左): %1 %2");
	if(info.m_listLeftAnkle.size() > 4)
	{
		double nPWV = info.m_listLeftAnkle.at(4).toDouble();
		int nAge = info.m_strAge.toInt();

		QResultEvaluate evaluate;
		double dN1SDPWV = 0.0;
		double dAveragePWV = 0.0;
		double dP1SDPWV = 0.0;
		double dP2SDPWV = 0.0;

		evaluate.getPWVByAgeAndSex(nAge, bIsMen, dN1SDPWV, dAveragePWV, dP1SDPWV, dP2SDPWV);

		QString strRet = QString::fromLocal8Bit("");
		if(nPWV != 0 && nPWV >= dAveragePWV)
		{
			strRet = QString::fromLocal8Bit("+%1%").arg(QString::number((nPWV/dAveragePWV - 1.0f)*100, 'f', 0));
		}
		else if(nPWV != 0 && nPWV < dAveragePWV)
		{
			strRet = QString::fromLocal8Bit("-%1%").arg(QString::number(( 1.0f -nPWV/dAveragePWV)*100, 'f', 0));
		}
		strValue = strValue.arg(nPWV).arg(strRet);
		painter->drawText(QPointF(5, nWidth - 1.5*nTextHeight),  strValue);
	}

	if(info.m_listRightAnkle.size() > 4)
	{
		double nPWV = info.m_listRightAnkle.at(4).toDouble();
		int nAge = info.m_strAge.toInt();

		QResultEvaluate evaluate;
		double dN1SDPWV = 0.0;
		double dAveragePWV = 0.0;
		double dP1SDPWV = 0.0;
		double dP2SDPWV = 0.0;

		evaluate.getPWVByAgeAndSex(nAge, bIsMen, dN1SDPWV, dAveragePWV, dP1SDPWV, dP2SDPWV);

		QString strRet = QString::fromLocal8Bit("");
		if(nPWV != 0 && nPWV >= dAveragePWV)
		{
			strRet = QString::fromLocal8Bit("+%1%").arg(QString::number((nPWV/dAveragePWV - 1.0f)*100, 'f', 0));
		}
		else if(nPWV != 0 && nPWV < dAveragePWV)
		{
			strRet = QString::fromLocal8Bit("-%1%").arg(QString::number(( 1.0f -nPWV/dAveragePWV)*100, 'f', 0));
		}
		strValue = QString::fromLocal8Bit("baPWV(右): %1 %2").arg(nPWV).arg(strRet);
		painter->drawText(QPointF(5, nWidth - 0.25*nTextHeight),  strValue);
	}
}

QPointF QDrawMeasure::_getPointByPWVAndAge(const float &nPWV, const int &nAge, const float &fXStartPos, const float &fYStart, const float &nDeltaX, const float &nDeltaY) const
{
	// 计算X坐标值
	float fXPos = fXStartPos + (nAge - 20)/10.0f*nDeltaX;
	if(fXPos < fXStartPos || fXPos > 100 + fXStartPos)
	{
		fXPos = fXStartPos;
	}

	// 计算Y坐标值
	float fYPos = fYStart - (nPWV-800)/200.0f*nDeltaY;
	float fYMin = fYStart - (nMAX_PWV - 800)/200.0f*nDeltaY;
	if(fYPos<fYMin || fYPos > fYStart)
	{
		fYPos = fYStart;
	}
	//qDebug()<<fXPos<<"\t"<<fYPos;
	return QPointF(fXPos, fYPos);
}

// 根据年龄计算出+1SD的PWV
// 公式由《PWV基准值.xlsx》中的原始数据拟合而来
float QDrawMeasure::_getPositive1SDPWV(const int &nAge, const bool &isMen) const
{
	float fRet = 0.0f;
	if(isMen)
	{
		fRet = 0.0003*nAge*nAge*nAge + 0.1131*nAge*nAge - 4.9623*nAge + 1343.9;
	}
	else
	{
		fRet = 0.0005*nAge*nAge*nAge + 0.1179*nAge*nAge - 3.6707*nAge + 1142.7;
	}

	return fRet;
}

// 根据年龄计算出+1SD的PWV
// 公式由《PWV基准值.xlsx》中的原始数据拟合而来
float QDrawMeasure::_getAveragePWV(const int &nAge, const bool &isMen) const
{
	float fRet = 0.0f;
	if(isMen)
	{
		fRet = -0.0003*nAge*nAge*nAge + 0.1951*nAge*nAge - 9.5127*nAge + 1268.6;
	}
	else
	{
		fRet = -0.0007*nAge*nAge*nAge + 0.2738*nAge*nAge - 11.103*nAge + 1126.7;
	}

	return fRet;
}

// 根据年龄计算出+1SD的PWV
// 公式由《PWV基准值.xlsx》中的原始数据拟合而来
float QDrawMeasure::_getNegative1SDPWV(const int &nAge, const bool &isMen) const
{
	float fRet = 0.0f;
	if(isMen)
	{
		fRet = 0.0004*nAge*nAge*nAge + 0.0393*nAge*nAge - 1.9031*nAge + 1023.5;
	}
	else
	{
		fRet = -0.0003*nAge*nAge*nAge + 0.179*nAge*nAge - 6.9367*nAge + 943.77;
	}
	return fRet;
}

void QDrawMeasure::_setPenWidthF(QPainter &painter) const
{
	QPen pen;
	pen.setColor(QColor(0,0,0));
	float fWidthF = 5*painter.window().width()/painter.viewport().width()*1.0f;
	float fHeight = 5*painter.window().height()/painter.viewport().height()*1.0f;
	pen.setWidthF(0.5f*qMin(fWidthF, fHeight));

	pen.setStyle(Qt::SolidLine);
	int nWidthF = pen.widthF();
	painter.setPen(pen);
}

void QDrawMeasure::_setFont(QPainter &painter, const int nFontSize, const bool bIsBold) const
{
	QFont fontText(QStringLiteral("宋体"), nFontSize);

	if(bIsBold)
	{
		fontText.setBold(true);
	}
	painter.setFont(fontText);
}
