#include "./header/stdafx.h"
#include <QPrinter>
#include <QPrintDialog>
#include <QPrinterInfo>
#include <exception>
#include "./header/QShowMeasureResult.h"
#include "./header/QReadAndWriteXml.h"
#include "./header/QDrawMeasure.h"
#include "./header/QResultEvaluate.h"

// 引入DLL
#ifdef __cplusplus
extern "C"
{
#include "./header/fenlishuju_and_lvbo_2.h"
}
#else
#include "./header/fenlishuju_and_lvbo_2.h"
#endif

//// 最大的PWV
//const int nMAX_PWV = 2600;
//// 最大的ABI值
//const float nMAX_ABI = 1.5f;


QShowMeasureResult::QShowMeasureResult(QWidget *pParent)
	:QWidget(pParent)
{
	_initUI();
}


QShowMeasureResult::~QShowMeasureResult(void)
{
	Q_DELETE(m_pbtnPrint);
	Q_DELETE(m_pMainLayout);
}

void QShowMeasureResult::_initUI()
{
	// 打印按钮
	m_pbtnPrint = new QPushButton(QString::fromLocal8Bit("打印"));
	m_pbtnPrint->setMinimumSize(80, 60);
	m_pbtnPrint->setIcon(QIcon(":/Print/Images/print.png"));

	// 布局
	m_pMainLayout = new QVBoxLayout(this);
	//m_pMainLayout->addSpacerItem(new QSpacerItem(width(), height(), 
	//	QSizePolicy::Expanding, QSizePolicy::Minimum));
	//m_pMainLayout->addWidget(m_pTableShow, 2);
	//m_pMainLayout->addSpacing(9);
	//m_pMainLayout->addWidget(m_pGraphShow, 2);
	//m_pMainLayout->addSpacing(height() - m_pbtnPrint->height());
	//m_pMainLayout->addWidget(m_pedShowResult, 1);
	//m_pMainLayout->addSpacing(9);
	m_pMainLayout->addWidget(m_pbtnPrint, 0, Qt::AlignBottom | Qt::AlignRight);

	setLayout(m_pMainLayout);

	connect(m_pbtnPrint,SIGNAL(clicked()), this, SLOT(slotPrint()));

#if _DEBUG
	m_infoAndValue.m_strNo = QString::fromLocal8Bit("20151223091930");
	m_infoAndValue.m_listHistory<<QString::fromLocal8Bit("高血压");
	m_infoAndValue.m_listHistory<<QString::fromLocal8Bit("高血糖");
	m_infoAndValue.m_strName = QString::fromLocal8Bit("张三");
	m_infoAndValue.m_strSex = QString::fromLocal8Bit("女");
	m_infoAndValue.m_strAge = "56";
	m_infoAndValue.m_strHeight = "165";
	m_infoAndValue.m_strWeight = "60";
	m_infoAndValue.m_strSection = QString::fromLocal8Bit("内科");
	m_infoAndValue.m_strDoctor = QString::fromLocal8Bit("李四");

	m_infoAndValue.m_strNote = "";
	m_infoAndValue.m_strDate = QString::fromLocal8Bit("2015年12月23日 10:19:30");

	m_infoAndValue.m_listLeftArm<<"106"<<"62"<<"76";
	m_infoAndValue.m_listRightArm<<"105"<<"68"<<"80";
	m_infoAndValue.m_listLeftAnkle<<"111"<<"51"<<"70"<<"0.0"<<"0";
	m_infoAndValue.m_listRightAnkle<<"107"<<"55"<<"72"<<"0.0"<<"0";
	m_infoAndValue.m_listHistory<<QString::fromLocal8Bit("高血压")<<QString::fromLocal8Bit("冠心病");

	m_nLeftPWV = 0;
	m_nRightPWV = 0;
	m_fLeftABI = 0;
	m_fRightABI = 0;
	//m_listLeftArmPressure = m_infoAndValue.m_listLeftArm;
	//m_listLeftAnklePressure = m_infoAndValue.m_listLeftAnkle;
	//m_listRightArmPressure =  m_infoAndValue.m_listRightArm;
	//m_listRightAnklePressure = m_infoAndValue.m_listRightAnkle;
	_updateResult();
#endif
	//m_pbtnPrint->move(width()- m_pbtnPrint->width() - 10, height());
}

void QShowMeasureResult::slotMeasureResult(const QString &fileName,const tagPersonInfo&tagInfo)
{
	// 临时调试用
	static QElapsedTimer timer;
	if(!timer.hasExpired(20*1000))
	{
		return;
	}

	timer.start();

	m_infoAndValue = tagInfo;

	_getABIAndPWV(fileName);

	_updateResult();

	//m_infoAndValue.m_strDiagnose = m_pedShowResult->toPlainText();

	m_infoAndValue.m_listLeftAnkle.clear();
	m_infoAndValue.m_listLeftAnkle << m_listLeftAnklePressure;
	m_infoAndValue.m_listRightAnkle.clear();
	m_infoAndValue.m_listRightAnkle << m_listRightAnklePressure;
	m_infoAndValue.m_listLeftArm.clear();
	m_infoAndValue.m_listLeftArm << m_listLeftArmPressure;
	m_infoAndValue.m_listRightArm.clear();
	m_infoAndValue.m_listRightArm << m_listRightArmPressure;

	QString strLeftABI = QString::number(m_fLeftABI, 'f', 2);
	QString strRightABI = QString::number(m_fRightABI, 'f', 2);
	m_infoAndValue.m_listLeftAnkle<<strLeftABI;
	m_infoAndValue.m_listLeftAnkle<<QString::number(m_nLeftPWV);
	m_infoAndValue.m_listRightAnkle<<strRightABI;
	m_infoAndValue.m_listRightAnkle<<QString::number(m_nRightPWV);

	m_infoAndValue.m_strDate = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
	// 保存测量结果
	{        
		QReadAndWriteXml xmlSaveInfo;
		xmlSaveInfo.saveInfoAndMeasureValue(m_infoAndValue);
	}

	update();
}

void QShowMeasureResult::_updateResult()
{
	//m_pedShowResult->clear();

#if !_DEBUG
	if(m_listLeftArmPressure.isEmpty())
	{
		return;
	}
#endif

	////m_pedShowResult->append(QString::fromLocal8Bit("结论:"));
	QString strDiagnose;
	//strDiagnose.append(QString::fromLocal8Bit("结论:"));
	//strDiagnose.append("\n");
	//if(m_nLeftPWV < 1200)
	//{
	//	strDiagnose.append(QString::fromLocal8Bit("您本次左侧baPWV测量结果: 左侧外周动脉柔软。"));
	//	//m_pedShowResult->append(QString::fromLocal8Bit("您本次左侧baPWV测量结果: 左侧外周动脉柔软。"));
	//}
	//else if(m_nLeftPWV > 1200 && m_nLeftPWV <= 1400)
	//{
	//	strDiagnose.append(QString::fromLocal8Bit("您本次左侧baPWV测量结果: 左侧外周动脉正常。"));
	//	//m_pedShowResult->append(QString::fromLocal8Bit("您本次左侧baPWV测量结果: 左侧外周动脉正常。"));
	//}
	//else if (m_nLeftPWV > 1400 && m_nLeftPWV <= 1600)
	//{
	//	strDiagnose.append(QString::fromLocal8Bit("您本次左侧baPWV测量结果: 怀疑左侧外周动脉稍微硬化。"));
	//	//m_pedShowResult->append(QString::fromLocal8Bit("您本次左侧baPWV测量结果: 怀疑左侧外周动脉稍微硬化。"));
	//}
	//else 
	//{
	//	strDiagnose.append(QString::fromLocal8Bit("您本次左侧baPWV测量结果: 怀疑左侧外周动脉硬化。"));
	//	//m_pedShowResult->append(QString::fromLocal8Bit("您本次左侧baPWV测量结果: 怀疑左侧外周动脉硬化。"));
	//}

	//strDiagnose.append("\n");

	//if(m_nRightPWV < 1200)
	//{
	//	strDiagnose.append(QString::fromLocal8Bit("您本次右侧baPWV测量结果: 右侧外周动脉柔软。"));
	//	//m_pedShowResult->append(QString::fromLocal8Bit("您本次右侧baPWV测量结果: 右侧外周动脉柔软。"));
	//}
	//else if(m_nRightPWV > 1200 && m_nRightPWV <= 1400)
	//{
	//	strDiagnose.append(QString::fromLocal8Bit("您本次右侧baPWV测量结果: 右侧外周动脉正常。"));
	//	//m_pedShowResult->append(QString::fromLocal8Bit("您本次右侧baPWV测量结果: 右侧外周动脉正常。"));
	//}
	//else if (m_nRightPWV > 1400 && m_nRightPWV <= 1600)
	//{
	//	strDiagnose.append(QString::fromLocal8Bit("您本次右侧baPWV测量结果: 怀疑右侧外周动脉稍微硬化。"));
	//	//m_pedShowResult->append(QString::fromLocal8Bit("您本次右侧baPWV测量结果: 怀疑右侧外周动脉稍微硬化。"));
	//}
	//else 
	//{
	//	strDiagnose.append(QString::fromLocal8Bit("您本次右侧baPWV测量结果: 怀疑右侧外周动脉硬化。"));
	//	//m_pedShowResult->append(QString::fromLocal8Bit("您本次右侧baPWV测量结果: 怀疑右侧外周动脉硬化。"));
	//}

	//strDiagnose.append("\n");

	//float fRightABI = m_fRightABI;
	//float fLeftABI = m_fLeftABI;
	//if(fLeftABI < 0.9f && fLeftABI >=0.8f)
	//{
	//	strDiagnose.append(QString::fromLocal8Bit("您本次左侧ABI测量结果: 左侧外周动脉可能阻塞。"));
	//	//m_pedShowResult->append(QString::fromLocal8Bit("您本次左侧ABI测量结果: 怀疑左侧外周动脉阻塞。"));
	//}
	//else if(fLeftABI >= 0.9f && fLeftABI<1.4f)
	//{
	//	strDiagnose.append(QString::fromLocal8Bit("您本次左侧ABI测量结果: 正常范围。"));
	//	//m_pedShowResult->append(QString::fromLocal8Bit("您本次左侧ABI测量结果: 左侧外周动脉正常。"));
	//}
	//else if(fLeftABI >= 1.4f)
	//{
	//	strDiagnose.append(QString::fromLocal8Bit("您本次左侧ABI测量结果: 左侧外周动脉钙化。"));
	//	//m_pedShowResult->append(QString::fromLocal8Bit("您本次左侧ABI测量结果: 怀疑左侧外周动脉钙化。"));
	//}
	//else
	//{
	//	strDiagnose.append(QString::fromLocal8Bit("您本次左侧ABI测量结果: 左侧外周动脉阻塞。"));
	//}

	//strDiagnose.append("\n");
	//if(fRightABI < 0.9f && fLeftABI >= 0.8f)
	//{
	//	strDiagnose.append(QString::fromLocal8Bit("您本次右侧ABI测量结果: 右侧外周动脉可能阻塞。"));
	//	//m_pedShowResult->append(QString::fromLocal8Bit("您本次右侧ABI测量结果: 怀疑右侧外周动脉阻塞。"));
	//}
	//else if(fRightABI >= 0.9f && fRightABI<=1.4f)
	//{
	//	strDiagnose.append(QString::fromLocal8Bit("您本次右侧ABI测量结果: 正常范围。"));
	//	//m_pedShowResult->append(QString::fromLocal8Bit("您本次右侧ABI测量结果: 右侧外周动脉正常。"));
	//}
	//else if(fRightABI > 1.4f)
	//{
	//	strDiagnose.append(QString::fromLocal8Bit("您本次右侧ABI测量结果: 右侧外周动脉钙化。"));
	//	//m_pedShowResult->append(QString::fromLocal8Bit("您本次右侧ABI测量结果: 怀疑右侧外周动脉钙化。"));
	//}
	//else
	//{
	//	strDiagnose.append(QString::fromLocal8Bit("您本次右侧ABI测量结果: 右侧外周动脉阻塞。"));
	//}
	QResultEvaluate evaluate;
	strDiagnose = evaluate.getEvaluate(56, m_fLeftABI,m_fRightABI, m_nLeftPWV, m_nRightPWV
		, m_infoAndValue.m_strSex == QString::fromLocal8Bit("男"));

	m_infoAndValue.m_strDiagnose = strDiagnose;
}

void QShowMeasureResult::paintEvent(QPaintEvent *pEvent)
{
	QPainter painter(this);
	QStringList listHeader;
	listHeader<<QString::fromLocal8Bit("(血压单位:mmHg)")<<QString::fromLocal8Bit("收缩压")
		<<QString::fromLocal8Bit("平均血压")<<QString::fromLocal8Bit("舒张压")
		<<QString::fromLocal8Bit("  ABI")<<QString::fromLocal8Bit("baPWV(cm/s)");

	QFont font(QStringLiteral("宋体"), 8);
	font.setBold(true);
	painter.setFont(font);
	QFontMetrics metrics = painter.fontMetrics();
	QList<int> listHeaderWidth;
	int nWindowWidth = 0;
	int nHeight = metrics.height();
	for(int i = 0; i <  listHeader.size(); ++i)
	{   
		int nWidth = metrics.width(listHeader.at(i)) + 15;
		nWindowWidth += nWidth;
		listHeaderWidth.append(nWidth);
	}
	int nViewHeight = height() - m_pbtnPrint->height() - 20;
	int nWindowHeight = 38*nHeight;
	painter.setViewport(0, 0, width(), nViewHeight);
	painter.setWindow(0, 0,  nWindowWidth, nWindowHeight);
	//painter.drawRect(0, 0,  nWindowWidth, 40*nHeight);

	Qt::Alignment flags = Qt::AlignRight | Qt::AlignBottom;
	// 绘制单位
	painter.save();
	{
		font.setBold(false);
		painter.setFont(font);
		painter.drawText(5, nHeight, listHeaderWidth.at(0), nHeight, flags, listHeader.at(0));
	}
	painter.restore();


	QList<int> listColPos;
	// 描绘标题
	int nLastDrawPos = listHeaderWidth.at(0);
	for(int i = 1; i<listHeader.size(); ++i)
	{
		painter.drawText(nLastDrawPos, nHeight, listHeaderWidth.at(i), nHeight, flags, listHeader.at(i));
		listColPos.append(nLastDrawPos);
		nLastDrawPos += listHeaderWidth.at(i);
	}
	listColPos.append(nLastDrawPos);

	QStringList listCol;
	listCol<<QString::fromLocal8Bit("右上臂")<<QString::fromLocal8Bit("左上臂")
		<<QString::fromLocal8Bit("右脚踝")<<QString::fromLocal8Bit("左脚踝");

	nLastDrawPos = nHeight*2.5;
	QList<int> listRowHeight;
	for(int i = 0; i < listCol.size(); ++i)
	{
		painter.drawText(listHeaderWidth.at(0) - metrics.width(listCol.at(i)), nLastDrawPos, 
			metrics.width(listCol.at(i)), nHeight*1.5, flags, listCol.at(i));
		listRowHeight.append(nLastDrawPos);
		nLastDrawPos += nHeight*1.5;
	}

	if(!m_listRightArmPressure.isEmpty())
	{
		// 右臂数据
		for(int i = 1; i <= m_listRightArmPressure.size(); ++i)
		{
			QString strValue = m_listRightArmPressure.at(i-1);
			painter.drawText(listColPos.at(i-1), listRowHeight.at(0), 
				listHeaderWidth.at(i), nHeight*1.5, Qt::AlignRight | Qt::AlignBottom, strValue);
		}

		// 左臂数据
		for(int i = 1; i <= m_listLeftArmPressure.size(); ++i)
		{
			QString strValue = m_listLeftArmPressure.at(i-1);
			painter.drawText(listColPos.at(i-1), listRowHeight.at(1), 
				listHeaderWidth.at(i), nHeight*1.5, Qt::AlignRight | Qt::AlignBottom, strValue);
		}

		int i = 1;
		// 右踝数据
		for(; i <= m_listRightAnklePressure.size(); ++i)
		{
			QString strValue = m_listRightAnklePressure.at(i-1);
			painter.drawText(listColPos.at(i-1), listRowHeight.at(2), 
				listHeaderWidth.at(i), nHeight*1.5, Qt::AlignRight | Qt::AlignBottom, strValue);
		}

		QString strValue = QString::number(m_fRightABI, 'f', 2);
		listColPos.at(i-1);
		listRowHeight.at(2);
		listHeaderWidth.at(i);
		painter.drawText(listColPos.at(i-1), listRowHeight.at(2), 
			listHeaderWidth.at(i), nHeight*1.5, Qt::AlignRight | Qt::AlignBottom, strValue);

		strValue = QString::number(m_nRightPWV);
		listColPos.at(i);
		listRowHeight.at(2);
		listHeaderWidth.at(i+1);
		painter.drawText(listColPos.at(i), listRowHeight.at(2), 
			listHeaderWidth.at(i+1), nHeight*1.5, Qt::AlignRight | Qt::AlignBottom, strValue);

		i = 1;
		// 左踝数据
		for(; i <= m_listLeftAnklePressure.size(); ++i)
		{
			QString strValue = m_listLeftAnklePressure.at(i-1);
			painter.drawText(listColPos.at(i-1), listRowHeight.at(3), 
				listHeaderWidth.at(i), nHeight*1.5, Qt::AlignRight | Qt::AlignBottom, strValue);
		}

		strValue = QString::number(m_fLeftABI, 'f', 2);
		painter.drawText(listColPos.at(i-1), listRowHeight.at(3), 
			listHeaderWidth.at(i), nHeight*1.5, Qt::AlignRight | Qt::AlignBottom, strValue);

		strValue = QString::number(m_nLeftPWV);
		painter.drawText(listColPos.at(i), listRowHeight.at(3), 
			listHeaderWidth.at(i+1), nHeight*1.5, Qt::AlignRight | Qt::AlignBottom, strValue);

	}

	painter.save();
	// 绘制图表显示
	{
		QPainter painterGraph(this);
		int nGraphHeight = height()*0.5;
		int nSide = qMin(width(), nGraphHeight);
		painterGraph.setViewport((width() - 1.44*nSide)/2, 9*nHeight*nViewHeight/nWindowHeight + (nGraphHeight - nSide)/2, 1.44*nSide, 1.0*nSide);
		QDrawMeasure draw;
		draw.drawAnalysis(&painterGraph, m_infoAndValue);
	}
	painter.restore();

	int nRows = m_infoAndValue.m_strDiagnose.split("\n").count() + 1;
	if( m_infoAndValue.m_strDiagnose.isEmpty())
	{
		painter.drawText(5, 31.5*nHeight, nWindowWidth, nHeight, Qt::AlignLeft |  Qt::AlignVCenter,  QString::fromLocal8Bit("结论:"));
		nRows = 3;
	}
	else
	{
		painter.drawText(5, 31.5*nHeight, nWindowWidth, nHeight,  Qt::AlignLeft |  Qt::AlignVCenter,  QString::fromLocal8Bit("结论:"));
		painter.drawText(5, 31.5*nHeight+nHeight, nWindowWidth, nHeight*nRows, Qt::AlignLeft |  Qt::AlignVCenter,  m_infoAndValue.m_strDiagnose);
	}

	QResultEvaluate evaluate;
	QStringList listDescription = evaluate.getDescription().split("\n");
	painter.save();
	{
		QFont font(painter.font());
		font.setPointSizeF(font.pointSizeF()*0.8);
		painter.setFont(font);
		QFontMetrics drawMetrics(painter.font());
		int nTempHeight = drawMetrics.height();
		for(int i = 0; i <listDescription.size(); ++i)
		{
			painter.drawText(5, 31.5*nHeight+(nRows+2)*nHeight+i*nTempHeight, metrics.width(listDescription.at(i)), nTempHeight,
				Qt::AlignLeft |  Qt::AlignVCenter,  listDescription.at(i));
		}
		//painter->drawText(5, fYPos + listRet.size()*nFontHeight + nFontHeight, evaluate.getDescription());
	}
	painter.restore();
	//return QWidget::paintEvent(pEvent);
}

void QShowMeasureResult::resizeEvent(QResizeEvent *pEvent)
{
	//m_pTableShow->setMinimumHeight(height()*0.3);
	//m_pGraphShow->setMinimumHeight(height()*0.4);
	//return QWidget::resizeEvent(pEvent);
}


//void QShowMeasureResult::slotPrint()
//{
//	//if(m_infoAndValue.m_strNo.isEmpty() || m_infoAndValue.m_listLeftAnkle.size() < 3)
//	//{
//	//	QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("没有测量结果可打印!"));
//	//	return;
//	//}
//
//	QPrinter printer;
//
//	//// 默认选择自带打印机
//	//QList<QPrinterInfo> listPrinter = QPrinterInfo::availablePrinters();
//	//foreach(QPrinterInfo info, listPrinter)
//	//{
//	//	if(info.printerName().contains("HP LaserJet 1020"))
//	//	{
//	//		printer.setPrinterName(info.printerName());
//	//		break;
//	//	}
//	//}
//
//	//if(listPrinter.isEmpty())
//	//{
//
//	//}
//
//	printer.setOutputFormat(QPrinter::PdfFormat);
//	printer.setOutputFileName("PrintE.pdf");
//	//printer.setOutputFormat(QPrinter::NativeFormat);
//	QPrintDialog printDlg(&printer, this);
//	//int nRet = printDlg.exec();
//	//QMessageBox::information(this, QString("AAA"), QString::number(nRet));
//	//if(printDlg.exec() == QDialog::Accepted)
//	//{
//		QPainter painter(&printer);
//		painter.setViewport(printer.pageRect());
//
// //       QPixmap pixmap(":/AppIcon/Images/Report.png");
//	//	painter.setWindow(0,0,pixmap.width()+20, pixmap.height()+20);
//	//	painter.drawPixmap(0,0,pixmap);
//	//	return;
//	//}
//	//QPainter painter(&printer);
//		painter.setFont(font());
//		QFontMetrics drawMetrics(painter.font());
//		int nHeight = drawMetrics.height();
//
//		int nLeftWidth = drawMetrics.width(QString::fromLocal8Bit("编号:") +  m_infoAndValue.m_strNo);
//		int nRightWidth =  drawMetrics.width(QString::fromLocal8Bit("姓名:姓名姓"));
//		int nDateWidth =  drawMetrics.width(QString::fromLocal8Bit("测量时间: ") + m_infoAndValue.m_strDate);
//		int nWidth = nLeftWidth + nRightWidth + nDateWidth;
//		nWidth *= 1.4;
//		painter.setWindow(0, 0, nWidth, 40*nHeight);
//
//		painter.setPen(Qt::blue);
//		painter.drawRect(0, 0, nWidth - 15, 39*nHeight);
//		float fYPos = 0.5f*nHeight;
//		float fXPos = 5;
//		painter.drawRect(fXPos, fYPos, nWidth - 25, nHeight*7 + fYPos);
//
//		QString strDrawText;
//		// 患者信息
//		{
//			painter.drawRect(fXPos + 5, fYPos + 0.5*nHeight, nWidth - 50 - nDateWidth, nHeight*6 + fYPos);
//
//			// 编号所在列的开始位置
//			float fFirstColLeft = fXPos + 10;
//			// 姓名所在列的开始位置
//			float fSecondColLeft = nWidth - 50 - nDateWidth - 5 - nRightWidth;
//
//			strDrawText = QString::fromLocal8Bit("编号:") +  m_infoAndValue.m_strNo;
//			// 编号
//			painter.drawText(fFirstColLeft, fYPos + 1.0*nHeight, drawMetrics.width(strDrawText), nHeight, Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
//
//			// 姓名
//			strDrawText = QString::fromLocal8Bit("姓名:") +  m_infoAndValue.m_strName;
//			painter.drawText(fSecondColLeft, fYPos + 1.0*nHeight, drawMetrics.width(strDrawText), nHeight, Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
//
//			// 性别
//			strDrawText = QString::fromLocal8Bit("性别:") +  m_infoAndValue.m_strSex;
//			painter.drawText(fFirstColLeft, fYPos + 2.5*nHeight,drawMetrics.width(strDrawText),nHeight, Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
//
//			// 年龄
//			strDrawText = QString::fromLocal8Bit("年龄:") +  m_infoAndValue.m_strAge + QString::fromLocal8Bit("岁");
//			painter.drawText(fSecondColLeft, fYPos + 2.5*nHeight,drawMetrics.width(strDrawText), nHeight, Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
//
//			// 身高
//			strDrawText = QString::fromLocal8Bit("身高:") +  m_infoAndValue.m_strHeight + QString::fromLocal8Bit("cm");
//			painter.drawText(fFirstColLeft, fYPos + nHeight*4.0,drawMetrics.width(strDrawText), nHeight, Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
//
//			// 体重
//			strDrawText = QString::fromLocal8Bit("体重:") +  m_infoAndValue.m_strWeight + QString::fromLocal8Bit("Kg");
//			painter.drawText(fSecondColLeft, fYPos + nHeight*4.0, drawMetrics.width(strDrawText), nHeight, Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
//
//			// 病史
//			strDrawText = QString::fromLocal8Bit("病史:") +  m_infoAndValue.m_listHistory.join(",");
//			painter.drawText(fFirstColLeft, fYPos + nHeight*5.5, drawMetrics.width(strDrawText), nHeight, Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
//		}
//
//
//		// 绘制测量时间信息 
//		{
//			float fLeft = nWidth - 35 - nDateWidth;
//			painter.drawRect(fLeft, fYPos + 0.5*nHeight, nDateWidth + 10, nHeight*6 + fYPos);
//			strDrawText = QString::fromLocal8Bit("测量时间:") + m_infoAndValue.m_strDate;
//			painter.drawText(fLeft + 5 , fYPos + 1.0*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
//
//			strDrawText = QString::fromLocal8Bit("科    室:") + m_infoAndValue.m_strSection;
//			painter.drawText(fLeft + 5, fYPos + 3.25*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
//
//			strDrawText = QString::fromLocal8Bit("医    生:") + m_infoAndValue.m_strDoctor;
//			painter.drawText(fLeft + 5, fYPos + 5.5*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
//		}
//
//		painter.drawRect(fXPos, 8.75*nHeight, nWidth - 25, 30*nHeight);
//
//		// 绘制单位
//		painter.save();
//		{	
//			strDrawText = QString::fromLocal8Bit("测量值");
//			painter.drawText(QRectF(fXPos + 10, 9*nHeight, drawMetrics.width(strDrawText), nHeight), strDrawText);
//
//			QFont currfont(QString::fromLocal8Bit("宋体"), 4);
//			currfont.setBold(true);
//			painter.setFont(currfont);
//			QFontMetrics currMetrics = QFontMetrics(currfont);
//			strDrawText = QString::fromLocal8Bit("血压单位:(mmHg)  PWV单位:(cm/s)");
//			painter.drawText(QRectF(nWidth - 25 - currMetrics.width(strDrawText) - 5 , 9*nHeight, currMetrics.width(strDrawText), nHeight), strDrawText);
//		}
//		painter.restore();
//
//
//		// 绘制左臂测量结果
//		int nRectWidth = 0;
//		{
//			float fLeft = fXPos + 5;
//			nRectWidth = drawMetrics.width(QString::fromLocal8Bit("  收缩压:100  "));
//			painter.drawRoundedRect(fLeft, 10.5*nHeight, nRectWidth, 6.5*nHeight, 20.0, 15.0);
//
//			strDrawText = QString::fromLocal8Bit("左臂");
//			painter.drawText(fLeft + (nRectWidth - drawMetrics.width(strDrawText))/2, 11*nHeight,drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
//
//			fLeft += 5;
//			strDrawText = QString::fromLocal8Bit("收缩压:  ");
//			if(m_infoAndValue.m_listLeftArm.at(0).length() <3)
//			{
//				strDrawText += " ";
//			}
//
//			strDrawText += m_infoAndValue.m_listLeftArm.at(0);
//			painter.drawText(fLeft, 12.5*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
//
//			strDrawText = QString::fromLocal8Bit("平均压:  ");
//			if(m_infoAndValue.m_listLeftArm.at(1).length() < 3)
//			{
//				strDrawText += " ";
//			}
//			strDrawText += m_infoAndValue.m_listLeftArm.at(1);
//
//			painter.drawText(fLeft , 14*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
//
//			strDrawText = QString::fromLocal8Bit("舒张压:  ");
//			if(m_infoAndValue.m_listLeftArm.at(2).length() <3)
//			{
//				strDrawText += " ";
//			}
//			strDrawText += m_infoAndValue.m_listLeftArm.at(2);
//			painter.drawText(fLeft , 15.5*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
//		}
//
//		// TODO:baPWV与年龄的关系
//		//painter.save();
//		//{
//		//	int nMinSide = qMin(nWidth - 25 -  nRectWidth, 18*nHeight);
//		//	int nXViewPos = (nRectWidth + 30) *  painter.viewport().width()/ nWidth;
//		//	int nYViewPos = 11*painter.viewport().height() / 40;
//		//	//painter.drawRect(nRectWidth + 30, 10*nHeight, nWidth - 25 - nRectWidth*2 - 30, 18*nHeight);
//		//	painter.setViewport(nXViewPos, nYViewPos, nMinSide, nMinSide);
//		//	painter.setWindow(0, 0, nMinSide, nMinSide);
//		//	painter.drawRect(0, 0, nMinSide*2, nMinSide*2);
//		//	for(int i = 0; i < 12; ++i)
//		//	{
//		//		painter.drawLine(nMinSide*2*i/12, 0, nMinSide*2*i/12, nMinSide*2);
//		//	}
//
//		//	for(int i = 0; i < 12; ++i)
//		//	{
//		//		painter.drawLine(0, nMinSide*2*i/12, nMinSide*2, nMinSide*2*i/12);
//		//	}
//		//}
//		//painter.restore();
//
//		// 绘制右臂测量结果
//		{
//			int nRectWidth  = drawMetrics.width(QString::fromLocal8Bit("  收缩压:100  "));
//			painter.drawRoundedRect(nWidth - 40 - nRectWidth -5, 10.5*nHeight, nRectWidth, 6.5*nHeight, 20.0, 15.0);
//
//			float fLeft = nWidth - 40 - nRectWidth + 5;
//			strDrawText = QString::fromLocal8Bit("右臂");
//			painter.drawText(nWidth - 40  - nRectWidth + (nRectWidth - drawMetrics.width(strDrawText))/2, 11*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
//
//			strDrawText = QString::fromLocal8Bit("收缩压:  ");
//			if(m_infoAndValue.m_listRightArm.at(0).length() <3)
//			{
//				strDrawText += " ";
//
//			}
//			strDrawText += m_infoAndValue.m_listRightArm.at(0);
//			painter.drawText(fLeft, 12.5*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
//
//			strDrawText = QString::fromLocal8Bit("平均压:  ");
//			if(m_infoAndValue.m_listRightArm.at(1).length() < 3)
//			{
//				strDrawText += " ";
//			}
//			strDrawText += m_infoAndValue.m_listRightArm.at(1);
//
//			painter.drawText(fLeft , 14*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
//
//			strDrawText = QString::fromLocal8Bit("舒张压:  ");
//			if(m_infoAndValue.m_listRightArm.at(2).length() <3)
//			{
//				strDrawText += " ";
//			}
//			strDrawText += m_infoAndValue.m_listRightArm.at(2);
//			painter.drawText(fLeft , 15.5*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
//		}
//
//
//		// 绘制左踝测量结果
//		{
//			float fLeft = fXPos + 5;
//			int nRectWidth  = drawMetrics.width(QString::fromLocal8Bit("  收缩压:100  "));
//			painter.drawRoundedRect(fLeft, 18*nHeight, nRectWidth, 9.5*nHeight, 20.0, 15.0);
//
//			strDrawText = QString::fromLocal8Bit("左踝");
//			painter.drawText(fLeft + (nRectWidth - drawMetrics.width(strDrawText))/2, 18.5*nHeight,drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
//
//			fLeft += 5;
//			strDrawText = QString::fromLocal8Bit("收缩压:  ");
//			if(m_infoAndValue.m_listLeftAnkle.at(0).length() <3)
//			{
//				strDrawText += " ";
//			}
//			strDrawText += m_infoAndValue.m_listLeftAnkle.at(0);
//			painter.drawText(fLeft, 20*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
//
//			strDrawText = QString::fromLocal8Bit("平均压:  ");
//			if(m_infoAndValue.m_listLeftAnkle.at(1).length() < 3)
//			{
//				strDrawText += " ";
//			}
//			strDrawText += m_infoAndValue.m_listLeftAnkle.at(1);
//
//			painter.drawText(fLeft , 21.5*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
//
//			strDrawText = QString::fromLocal8Bit("舒张压:  ");
//			if(m_infoAndValue.m_listLeftAnkle.at(2).length() <3)
//			{
//				strDrawText += " ";
//			}
//			strDrawText += m_infoAndValue.m_listLeftAnkle.at(2);
//			painter.drawText(fLeft , 23*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
//
//			strDrawText = QString::fromLocal8Bit("   ABI: ");
//			if(m_infoAndValue.m_listLeftAnkle.at(3).length() <3)
//			{
//				strDrawText += " ";
//			}
//			strDrawText += m_infoAndValue.m_listLeftAnkle.at(3);
//			painter.drawText(fLeft , 24.5*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
//
//			strDrawText = QString::fromLocal8Bit("   PWV: ");
//			if(m_infoAndValue.m_listLeftAnkle.at(4).length() <3)
//			{
//				strDrawText += " ";
//			}
//			strDrawText += m_infoAndValue.m_listLeftAnkle.at(4);
//			painter.drawText(fLeft , 26*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
//		}
//
//		// 绘制右踝测量结果
//		{
//			int nRectWidth  = drawMetrics.width(QString::fromLocal8Bit("  收缩压:100  "));
//			painter.drawRoundedRect(nWidth - 40  - nRectWidth - 5, 18*nHeight, nRectWidth, 9.5*nHeight, 20.0, 15.0);
//
//			float fLeft = nWidth - 40 - nRectWidth + 5;
//			strDrawText = QString::fromLocal8Bit("右踝");
//			painter.drawText(nWidth - 40  - nRectWidth + (nRectWidth - drawMetrics.width(strDrawText))/2, 18.5*nHeight,drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
//
//			strDrawText = QString::fromLocal8Bit("收缩压:  ");
//			if(m_infoAndValue.m_listRightAnkle.at(0).length() <3)
//			{
//				strDrawText += " ";
//			}
//			strDrawText += m_infoAndValue.m_listRightAnkle.at(0);
//			painter.drawText(fLeft, 20*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
//
//			strDrawText = QString::fromLocal8Bit("平均压:  ");
//			if(m_infoAndValue.m_listRightAnkle.at(1).length() < 3)
//			{
//				strDrawText += " ";
//			}
//			strDrawText += m_infoAndValue.m_listRightAnkle.at(1);
//			painter.drawText(fLeft , 21.5*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
//
//			strDrawText = QString::fromLocal8Bit("舒张压:  ");
//			if(m_infoAndValue.m_listRightAnkle.at(2).length() <3)
//			{
//				strDrawText += " ";
//			}
//			strDrawText += m_infoAndValue.m_listRightAnkle.at(2);
//			painter.drawText(fLeft , 23*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
//
//			strDrawText = QString::fromLocal8Bit("   ABI: ");
//			if(m_infoAndValue.m_listRightAnkle.at(3).length() <3)
//			{
//				strDrawText += " ";
//			}
//			strDrawText += m_infoAndValue.m_listRightAnkle.at(3);
//			painter.drawText(fLeft , 24.5*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
//
//			strDrawText = QString::fromLocal8Bit("   PWV: ");
//			if(m_infoAndValue.m_listRightAnkle.at(4).length() <3)
//			{
//				strDrawText += " ";
//			}
//			strDrawText += m_infoAndValue.m_listRightAnkle.at(4);
//			painter.drawText(fLeft , 26*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
//		}
//
//
//		// 绘制参考值
//		{
//			float fLeft = fXPos + 10;
//			strDrawText = QString::fromLocal8Bit("参考值:");
//			painter.drawText(fLeft , 28*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
//
//			strDrawText = QString::fromLocal8Bit("ABI: 0.9-1.4  PWV:1000-1200");
//			painter.drawText(fLeft , 29*nHeight, drawMetrics.width(strDrawText), nHeight,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
//		}
//
//		// 绘制结论
//		{
//			float fLeft = fXPos + 10;
//			strDrawText = m_infoAndValue.m_strDiagnose;
//
//			int nRows = m_infoAndValue.m_strDiagnose.split("\n").count() + 1;
//			painter.drawText(15 , 30.5*nHeight, drawMetrics.width(strDrawText), nHeight *nRows,  Qt::AlignLeft | Qt::AlignVCenter, strDrawText);
//		}
//	//}
//}

void QShowMeasureResult::slotPrint()
{
	if(m_infoAndValue.m_strNo.isEmpty() || m_infoAndValue.m_listLeftAnkle.size() < 3)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("没有测量结果可打印!"));
		return;
	}

	QPrinter printer;
	QPrintDialog printDlg(&printer);
	int nRet = printDlg.exec();
    if(nRet == QDialog::Accepted)
	{
		QPainter painter(&printer);
		QRect pageRect = printer.pageRect();
		
		int nViewWidth = pageRect.width();
		int nViewHeight = pageRect.height();

		// 绘制标题
		painter.save();
		{
			QFont fontTitle(QString::fromLocal8Bit("宋体"), 11);
			fontTitle.setBold(true);
			painter.setFont(fontTitle);
			QFontMetrics drawMetrics(painter.font());
			painter.setViewport(0, 0, nViewWidth, nViewHeight*0.05f);
			painter.setWindow(0, 0, nViewWidth, drawMetrics.height() * 1.5);
			painter.drawText(QRect(0, drawMetrics.height()*0.25, nViewWidth - 10, drawMetrics.height()),  Qt::AlignCenter, QString::fromLocal8Bit("乐普医疗动脉硬化检测报告"));
		}
		painter.restore();

		// 绘制基本信息的设置  
		painter.save();
		{
			QPen pen;
			pen.setWidthF(1.0f);
			pen.setColor(Qt::black);
			pen.setStyle(Qt::SolidLine);
			painter.setPen(pen);
			painter.setViewport(5, 5 + nViewHeight*0.05f, nViewWidth - 10, nViewHeight*0.1f);
			QDrawMeasure draw;
			draw.drawPersonInfo(&painter, m_infoAndValue);
		}
		painter.restore();

		// 绘制脉搏波数据
		painter.save();
		{
			//painter.setViewport(5 + (pageRect.width() - 10 - nMinSide)/2, 5+pageRect.height()*0.15f + ((int)(pageRect.height()*0.35) - nMinSide)/2, nMinSide, nMinSide);
			painter.setViewport(5, 5 + nViewHeight*0.15f, nViewWidth - 10, nViewHeight*0.30f);
			QDrawMeasure draw;
			draw.drawWaveInfo(&painter, m_infoAndValue);
		}
		painter.restore();


		// 绘制血压值数据
		painter.save();
		{
			painter.setViewport(5, 5+nViewHeight*0.45f, (nViewWidth-10)/2, nViewHeight*0.25f);
			QDrawMeasure draw;
			draw.drawPressure(&painter, m_infoAndValue);
		}
		painter.restore();

		// 绘制PWV分析
		painter.save();
		{
			//float nSide = qMin((pageRect.width()-10)/2.0f, pageRect.height()*0.25f);
			painter.setViewport(5+(nViewWidth-10)/2, 5+nViewHeight*0.45f, (nViewWidth-10)/2, nViewHeight*0.25f);
			QDrawMeasure draw;
			draw.drawPwvAnalysis(&painter, m_infoAndValue);
		}
		painter.restore();

		// 绘制ABI及PWV综合分析信息
		painter.save();
		{
			painter.setViewport(5, 5+nViewHeight*0.70f, 
				(nViewWidth - 10)/2, nViewHeight*0.25f);
			QDrawMeasure draw;
			draw.drawAnalysis(&painter, m_infoAndValue, true);
		}
		painter.restore();

		// 绘制结论信息
		painter.save();
		{
			painter.setViewport(5+(nViewWidth - 10)/2, 5+nViewHeight*0.70f, 
				(nViewWidth -10)/2, nViewHeight*0.25f);
			QDrawMeasure draw;
			draw.drawResult(&painter, m_infoAndValue);
		}
		painter.restore();

		// 绘制大的边框
		painter.save();
		{
			painter.setViewport(0, 0, nViewWidth, nViewHeight);
			painter.setWindow(0, 0, nViewWidth, nViewHeight);
			QPen pen;
			pen.setColor(QColor(0,0,0));
			pen.setWidthF(4.0f);
			pen.setStyle(Qt::SolidLine);
			painter.setPen(pen);
			painter.drawRect(5, 5 + nViewHeight*0.05f, nViewWidth - 10, nViewHeight*0.90);
		}
		painter.restore();
		//return;
   }
}

// 调用dll中的方法，获取测量结果
void QShowMeasureResult::_getABIAndPWV(const QString & strFileName)
{
	// 顺序：左上臂收缩压、平均压、舒张压
	m_listLeftArmPressure.clear();

	// 顺序：右上臂收缩压、平均压、舒张压
	m_listRightArmPressure.clear();

	// 顺序：左脚踝收缩压、平均压、舒张压
	m_listLeftAnklePressure.clear();

	// 顺序：右脚踝收缩压、平均压、舒张压
	m_listRightAnklePressure.clear();

	double dAnkle_left_bloodPress = 0.0;
	double dAnkle_left_Diastolic = 0.0;
	double dAnkle_left_dingdian = 0.0;
	double dAnkle_right_bloodPress = 0.0;
	double dAnkle_right_Diastolic = 0.0;
	double dAnkle_right_dingdian = 0.0;
	double dBrachial_left_bloodPress = 0.0;
	double dBrachial_left_Diastolic = 0.0;
	double dBrachial_left_dingdian = 0.0;
	double dBrachial_right_bloodPress = 0.0;
	double dBrachial_right_Diastolic = 0.0;
	double dBrachial_right_dingdian = 0.0;
	double dleft_ABI = 0.0;
	double dright_ABI = 0.0;
	double dleft_PWV = 0.0;
	double dright_PWV = 0.0;

	try
	{
		// TODO：这一段很恶心，有时间修改
		QString strTemp = strFileName;
		strTemp.replace(QString("/"),QString("\\\\"));

		QString strDir = strTemp.left(strTemp.lastIndexOf("\\\\") + 2);

		QByteArray baFileName = strTemp.toLocal8Bit();
		char *pFileName = baFileName.data();

		QByteArray baFileDir = strDir.toLocal8Bit();
		char *pFileDir = baFileDir.data();

		//suanfa2(pFileName, pFileDir,
		//	&dAnkle_left_bloodPress, &dAnkle_left_Diastolic, &dAnkle_left_dingdian,
		//	&dAnkle_right_bloodPress, &dAnkle_right_Diastolic, &dAnkle_right_dingdian,
		//	&dBrachial_left_bloodPress, &dBrachial_left_Diastolic, &dBrachial_left_dingdian,
		//	&dBrachial_right_bloodPress, &dBrachial_right_Diastolic, &dBrachial_right_dingdian,
		//	&dleft_ABI, &dright_ABI, &dleft_PWV, &dright_PWV);
	}
	catch (const std::bad_exception &e)
	{
		//qDebug()<<"There Is a Exception."<<e.what();
	}

	// 左踝PWV
	m_nLeftPWV = dleft_PWV;

	// 右踝PWV
	m_nRightPWV = dright_PWV;

	m_listLeftArmPressure.append(QString::number((int)dBrachial_left_bloodPress));
	m_listLeftArmPressure.append(QString::number((int)dBrachial_left_dingdian));
	m_listLeftArmPressure.append(QString::number((int)dBrachial_left_Diastolic));

	m_listLeftAnklePressure.append(QString::number((int)dAnkle_left_bloodPress));
	m_listLeftAnklePressure.append(QString::number((int)dAnkle_left_dingdian));
	m_listLeftAnklePressure.append(QString::number((int)dAnkle_left_Diastolic));

	m_listRightArmPressure.append(QString::number((int)dBrachial_right_bloodPress));
	m_listRightArmPressure.append(QString::number((int)dBrachial_right_dingdian));
	m_listRightArmPressure.append(QString::number((int)dBrachial_right_Diastolic));

	m_listRightAnklePressure.append(QString::number((int)dAnkle_right_bloodPress));
	m_listRightAnklePressure.append(QString::number((int)dAnkle_right_dingdian));
	m_listRightAnklePressure.append(QString::number((int)dAnkle_right_Diastolic));

	m_fLeftABI = dleft_ABI;
	m_fRightABI = dright_ABI;
}
