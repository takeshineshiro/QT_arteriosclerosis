#include "./header/stdafx.h"
#include <QDesktopWidget>
#include "./header/QPainterResult.h"


QPainterResult::QPainterResult(tagPersonInfo info, QWidget *pParent)
	:QDialog(pParent),m_infoAndValue(info)
{
	setWindowTitle(QString::fromLocal8Bit("历史测量数据"));
	
	QFontMetrics drawMetrics(font());
	int nHeight = drawMetrics.height();

	setMinimumWidth(640);
	setMinimumHeight(40*nHeight);
}

QPainterResult::~QPainterResult(void)
{
}

void QPainterResult::paintEvent(QPaintEvent *pEvent)
{
	QPainter painter(this);
	painter.setFont(font());
	QFontMetrics drawMetrics(painter.font());
	int nHeight = drawMetrics.height();

	int nLeftWidth = drawMetrics.width(QString::fromLocal8Bit("编号:") +  m_infoAndValue.m_strNo);
	int nRightWidth =  drawMetrics.width(QString::fromLocal8Bit("姓名:姓名姓"));
	int nDateWidth =  drawMetrics.width(QString::fromLocal8Bit("测量时间: ") + m_infoAndValue.m_strDate);
	int nWidth = nLeftWidth + nRightWidth + nDateWidth;

	if(nWidth < width())
	{
		nWidth = width();
	}

	painter.setWindow(0, 0, nWidth, 39*nHeight);

	float fYPos = 0.5f*nHeight;
	float fXPos = 5;
	painter.drawRect(fXPos, fYPos, nWidth - 10, nHeight*7 + fYPos);

	// 外边框
	painter.drawRect(fXPos + 5, fYPos + 0.5*nHeight, nWidth- 40 - nDateWidth, nHeight*6 + fYPos);

	QString strDrawText = QString::fromLocal8Bit("编号:") +  m_infoAndValue.m_strNo;
	// 编号
	painter.drawText(fXPos + 5, fYPos + 1.0*nHeight, drawMetrics.width(strDrawText), nHeight, Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

	// 姓名
	strDrawText = QString::fromLocal8Bit("姓名:") +  m_infoAndValue.m_strName;
	painter.drawText(nWidth - 40 - nDateWidth - nRightWidth, fYPos + 1.0*nHeight, drawMetrics.width(strDrawText), nHeight, Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

	// 性别
	strDrawText = QString::fromLocal8Bit("性别:") +  m_infoAndValue.m_strSex;
	painter.drawText(fXPos + 5, fYPos + 2.5*nHeight,drawMetrics.width(strDrawText),nHeight, Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

	// 年龄
	strDrawText = QString::fromLocal8Bit("年龄:") +  m_infoAndValue.m_strAge + QString::fromLocal8Bit("岁");
	painter.drawText(nWidth - 40 - nDateWidth - nRightWidth, fYPos + 2.5*nHeight,drawMetrics.width(strDrawText), nHeight, Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

	// 身高
	strDrawText = QString::fromLocal8Bit("身高:") +  m_infoAndValue.m_strHeight + "cm";
	painter.drawText(fXPos + 5, fYPos + nHeight*4.0,drawMetrics.width(strDrawText), nHeight, Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

	// 体重
	strDrawText = QString::fromLocal8Bit("体重:") +  m_infoAndValue.m_strWeight + "Kg";
	painter.drawText(nWidth - 40 - nDateWidth - nRightWidth, fYPos + nHeight*4.0, drawMetrics.width(strDrawText), nHeight, Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

	// 病史
	strDrawText = QString::fromLocal8Bit("病史:") +  m_infoAndValue.m_listHistory.join(",");
	painter.drawText(fXPos + 5, fYPos + nHeight*5.5, drawMetrics.width(strDrawText), nHeight, Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

	// 绘制右侧的数据
	painter.drawRect(nWidth - 15 - nDateWidth, fYPos + 0.5*nHeight, nDateWidth, nHeight*6 + fYPos);
	strDrawText = QString::fromLocal8Bit("测量时间:") + m_infoAndValue.m_strDate;
	painter.drawText(nWidth - 15 - nDateWidth + 5, fYPos + 1.0*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

	strDrawText = QString::fromLocal8Bit("科    室:") + m_infoAndValue.m_strSection;
	painter.drawText(nWidth - 15 - nDateWidth + 5, fYPos + 3.25*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

	strDrawText = QString::fromLocal8Bit("医    生:") + m_infoAndValue.m_strDoctor;
	painter.drawText(nWidth - 15 - nDateWidth + 5, fYPos + 5.5*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

	painter.drawRect(fXPos, 8.75*nHeight, nWidth - 10, 22*nHeight);
	// 绘制单位
	{
		strDrawText = QString::fromLocal8Bit("测量值");
		painter.drawText(QRectF(fXPos + 5, 9*nHeight, drawMetrics.width(strDrawText), nHeight), strDrawText);

		strDrawText = QString::fromLocal8Bit("血压单位:(mmHg)  PWV单位:(cm/s)");
		painter.drawText(QRectF(nWidth - 10 - drawMetrics.width(strDrawText), 9*nHeight, drawMetrics.width(strDrawText), nHeight), strDrawText);
	}

	// 绘制左臂测量结果
	{
		int nRectWidth  = drawMetrics.width(QString::fromLocal8Bit("  收缩压:100  "));
		painter.drawRoundedRect(10, 10.5*nHeight, nRectWidth, 6.5*nHeight, 20.0, 15.0);

		strDrawText = QString::fromLocal8Bit("左臂");
		painter.drawText(10 + (nRectWidth - drawMetrics.width(strDrawText))/2, 11*nHeight,drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		strDrawText = QString::fromLocal8Bit("收缩压:  ");
		if(m_infoAndValue.m_listLeftArm.at(0).length() <3)
		{
			strDrawText += " ";

		}
		strDrawText += m_infoAndValue.m_listLeftArm.at(0);
		painter.drawText(fXPos + 10, 12.5*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		strDrawText = QString::fromLocal8Bit("平均压:  ");
		if(m_infoAndValue.m_listLeftArm.at(1).length() < 3)
		{
			strDrawText += " ";
		}
		strDrawText += m_infoAndValue.m_listLeftArm.at(1);

		painter.drawText(fXPos + 10 , 14*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		strDrawText = QString::fromLocal8Bit("舒张压:  ");
		if(m_infoAndValue.m_listLeftArm.at(2).length() <3)
		{
			strDrawText += " ";
		}
		strDrawText += m_infoAndValue.m_listLeftArm.at(2);
		painter.drawText(fXPos + 10 , 15.5*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
	}

	// 绘制右臂测量结果
	{
		int nRectWidth  = drawMetrics.width(QString::fromLocal8Bit("  收缩压:100  "));
		painter.drawRoundedRect(width() - 10 - nRectWidth, 10.5*nHeight, nRectWidth, 6.5*nHeight, 20.0, 15.0);

		strDrawText = QString::fromLocal8Bit("右臂");
		painter.drawText(nWidth - 10 - nRectWidth + (nRectWidth - drawMetrics.width(strDrawText))/2, 11*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		strDrawText = QString::fromLocal8Bit("收缩压:  ");
		if(m_infoAndValue.m_listRightArm.at(0).length() <3)
		{
			strDrawText += " ";

		}
		strDrawText += m_infoAndValue.m_listRightArm.at(0);
		painter.drawText(nWidth - 5 - nRectWidth, 12.5*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		strDrawText = QString::fromLocal8Bit("平均压:  ");
		if(m_infoAndValue.m_listRightArm.at(1).length() < 3)
		{
			strDrawText += " ";
		}
		strDrawText += m_infoAndValue.m_listRightArm.at(1);

		painter.drawText(nWidth - 5  - nRectWidth , 14*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		strDrawText = QString::fromLocal8Bit("舒张压:  ");
		if(m_infoAndValue.m_listRightArm.at(2).length() <3)
		{
			strDrawText += " ";
		}
		strDrawText += m_infoAndValue.m_listRightArm.at(2);
		painter.drawText(nWidth - 5  - nRectWidth , 15.5*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
	}

	// 绘制左踝测量结果
	{
		int nRectWidth  = drawMetrics.width(QString::fromLocal8Bit("  收缩压:100  "));
		painter.drawRoundedRect(10, 18*nHeight, nRectWidth, 9.5*nHeight, 20.0, 15.0);

		strDrawText = QString::fromLocal8Bit("左踝");
		painter.drawText(10 + (nRectWidth - drawMetrics.width(strDrawText))/2, 18.5*nHeight,drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		strDrawText = QString::fromLocal8Bit("收缩压:  ");
		if(m_infoAndValue.m_listLeftAnkle.at(0).length() <3)
		{
			strDrawText += " ";
		}
		strDrawText += m_infoAndValue.m_listLeftAnkle.at(0);
		painter.drawText(fXPos + 10, 20*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		strDrawText = QString::fromLocal8Bit("平均压:  ");
		if(m_infoAndValue.m_listLeftAnkle.at(1).length() < 3)
		{
			strDrawText += " ";
		}
		strDrawText += m_infoAndValue.m_listLeftAnkle.at(1);

		painter.drawText(fXPos + 10 , 21.5*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		strDrawText = QString::fromLocal8Bit("舒张压:  ");
		if(m_infoAndValue.m_listLeftAnkle.at(2).length() <3)
		{
			strDrawText += " ";
		}
		strDrawText += m_infoAndValue.m_listLeftAnkle.at(2);
		painter.drawText(fXPos + 10 , 23*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		strDrawText = QString::fromLocal8Bit("   ABI: ");
		if(m_infoAndValue.m_listLeftAnkle.at(3).length() <3)
		{
			strDrawText += " ";
		}
		strDrawText += m_infoAndValue.m_listLeftAnkle.at(3);
		painter.drawText(fXPos + 10 , 24.5*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		strDrawText = QString::fromLocal8Bit("   PWV: ");
		if(m_infoAndValue.m_listLeftAnkle.at(4).length() <3)
		{
			strDrawText += " ";
		}
		strDrawText += m_infoAndValue.m_listLeftAnkle.at(4);
		painter.drawText(fXPos + 10 , 26*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
	}

	// 绘制右踝测量结果
	{
		int nRectWidth  = drawMetrics.width(QString::fromLocal8Bit("  收缩压:100  "));
		painter.drawRoundedRect(nWidth - 10 - nRectWidth, 18*nHeight, nRectWidth, 9.5*nHeight, 20.0, 15.0);

		strDrawText = QString::fromLocal8Bit("右踝");
		painter.drawText(nWidth - 10 - nRectWidth + (nRectWidth - drawMetrics.width(strDrawText))/2, 18.5*nHeight,drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		strDrawText = QString::fromLocal8Bit("收缩压:  ");
		if(m_infoAndValue.m_listRightAnkle.at(0).length() <3)
		{
			strDrawText += " ";
		}
		strDrawText += m_infoAndValue.m_listRightAnkle.at(0);
		painter.drawText(nWidth - 5 - nRectWidth, 20*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		strDrawText = QString::fromLocal8Bit("平均压:  ");
		if(m_infoAndValue.m_listRightAnkle.at(1).length() < 3)
		{
			strDrawText += " ";
		}
		strDrawText += m_infoAndValue.m_listRightAnkle.at(1);
		painter.drawText(nWidth - 5 - nRectWidth , 21.5*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		strDrawText = QString::fromLocal8Bit("舒张压:  ");
		if(m_infoAndValue.m_listRightAnkle.at(2).length() <3)
		{
			strDrawText += " ";
		}
		strDrawText += m_infoAndValue.m_listRightAnkle.at(2);
		painter.drawText(nWidth - 5 - nRectWidth , 23*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		strDrawText = QString::fromLocal8Bit("   ABI: ");
		if(m_infoAndValue.m_listRightAnkle.at(3).length() <3)
		{
			strDrawText += " ";
		}
		strDrawText += m_infoAndValue.m_listRightAnkle.at(3);
		painter.drawText(nWidth - 5 - nRectWidth , 24.5*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		strDrawText = QString::fromLocal8Bit("   PWV: ");
		if(m_infoAndValue.m_listRightAnkle.at(4).length() <3)
		{
			strDrawText += " ";
		}
		strDrawText += m_infoAndValue.m_listRightAnkle.at(4);
		painter.drawText(nWidth - 5 - nRectWidth , 26*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
	}

	// 绘制参考值
	{
		strDrawText = QString::fromLocal8Bit("参考值:");
		painter.drawText(fXPos + 10 , 28*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);

		strDrawText = QString::fromLocal8Bit("ABI: 0.9-1.4  PWV:1000-1200");
		painter.drawText(fXPos + 10 , 29.5*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
	}
	
	painter.drawRect(5, 31*nHeight, nWidth - 10, 7*nHeight);
	// 绘制结论
	{
		strDrawText = m_infoAndValue.m_strDiagnose;
		int nRows = m_infoAndValue.m_strDiagnose.split("\n").count() + 1;
		painter.drawText(fXPos + 10 , 31.5*nHeight, drawMetrics.width(strDrawText), nHeight *nRows,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
	}
}
