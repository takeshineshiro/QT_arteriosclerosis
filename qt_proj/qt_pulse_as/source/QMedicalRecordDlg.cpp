#include "./header/stdafx.h"
#include <QPrinter>
#include <QPrintDialog>
#include "./header/QDrawMeasure.h"
#include "./header/ui_MedicalRecordDlg.h"
#include "./header/QReadAndWriteXml.h"
#include "./header/QMedicalRecordDlg.h"
#include "./header/QPainterResult.h"


QMedicalRecordDlg::QMedicalRecordDlg(QWidget *pParent)
	:QWidget(pParent)
{
	_initUI();
}


QMedicalRecordDlg::~QMedicalRecordDlg(void)
{
	Q_DELETE(m_pRecordDlg);

	//QMap<int, tagPersonInfo *> m_pRowToInfo
	QMap<int, tagPersonInfo *>::iterator iter = m_mapRowToInfo.begin();
	while( iter != m_mapRowToInfo.end())
	{
		tagPersonInfo * pPreInfo = *iter; 
		++iter;
		Q_DELETE(pPreInfo);
	}
}

void QMedicalRecordDlg::_initUI()
{
	m_pRecordDlg = new Ui::MedicalRecordDlg;
	m_pRecordDlg->setupUi(this);

	QStandardItemModel *pModel = new QStandardItemModel();
	Q_CHECK_PTR(pModel);
	QStringList listHeader;
	listHeader<<QString::fromLocal8Bit("序号")<<QString::fromLocal8Bit("编号")
		<<QString::fromLocal8Bit("姓名")<<QString::fromLocal8Bit("性别")
		<<QString::fromLocal8Bit("年龄")<<QString::fromLocal8Bit("测量时间");
	pModel->setHorizontalHeaderLabels(listHeader);
	m_pRecordDlg->m_pQueryResultView->setModel(pModel);

	m_pRecordDlg->m_pQueryResultView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

	// 设置查询区间
	m_pRecordDlg->m_pStartTime->setDate(QDate::currentDate().addDays(-30));
	m_pRecordDlg->m_pEndTime->setDate(QDate::currentDate());

	// 编号是年月日时分秒组成,如20151010162930(14位)
	m_pRecordDlg->m_pedPersonNo->setValidator(new QRegExpValidator(QRegExp("[1-9][0-9]{13,13}"), this));

	// 姓名的最大长度为30
	m_pRecordDlg->m_pedName->setValidator(new QRegExpValidator(QRegExp(".{1,30}"), this));

	connect(m_pRecordDlg->m_pbtnShowDetail, SIGNAL(clicked()),
		this , SLOT(slotScanDetail()));
	connect(m_pRecordDlg->m_pQueryResultView, SIGNAL(doubleClicked(const QModelIndex & index)), this,
		SLOT(slotScanDetail()));
	connect(m_pRecordDlg->m_pbtnQuery, SIGNAL(clicked()), this ,SLOT(slotQuery()));
}

void QMedicalRecordDlg::slotQuery()
{
	QStandardItemModel *pModel = dynamic_cast<QStandardItemModel *>(m_pRecordDlg->m_pQueryResultView->model());
	if(pModel != NULL)
	{
		pModel->removeRows(0, pModel->rowCount());
		QMap<int, tagPersonInfo *>::iterator iter = m_mapRowToInfo.begin();
		while( iter != m_mapRowToInfo.end())
		{
			tagPersonInfo * pPreInfo = *iter; 
			++iter;
			Q_DELETE(pPreInfo);
		}
		m_mapRowToInfo.clear();
	}

	// 获取查询条件
	QString strNo = m_pRecordDlg->m_pedPersonNo->text();
	QString strName = m_pRecordDlg->m_pedName->text();
	QString strStartTime = m_pRecordDlg->m_pStartTime->text();
	QString strEndTime = m_pRecordDlg->m_pEndTime->text();

	bool bValidTime = QDateTime::fromString(strStartTime, "yyyy/MM/dd HH:mm:ss") > QDateTime::fromString(strEndTime, "yyyy/MM/dd HH:mm:ss");
	bool bNoNameEmpty = strNo.isEmpty() && strName.isEmpty();
	// 如果编号和姓名都错误，则返回
	if(bValidTime && bNoNameEmpty)
	{
		if(bNoNameEmpty)
		{
			QMessageBox::information(this, QString::fromLocal8Bit("查询条件"), QString::fromLocal8Bit("请输入编号或者姓名！"));
			m_pRecordDlg->m_plblTotalRecord->setText(QString::fromLocal8Bit("满足条件共：<font color=""red"">%1 </font>条记录").arg(0));
			return;
		}

		if(bValidTime)
		{
			QMessageBox::information(this, QString::fromLocal8Bit("查询条件"), QString::fromLocal8Bit("输入的开始时间大于结束时间！"));
			m_pRecordDlg->m_plblTotalRecord->setText(QString::fromLocal8Bit("满足条件共：<font color=""red"">%1 </font>条记录").arg(0));
		}
		return;
	}


	QReadAndWriteXml xmlReader;
	QList<QStandardItem *> listItem = xmlReader.queryHistoryRecord(strNo, strName, m_mapRowToInfo, strStartTime, strEndTime);
	if(!listItem.isEmpty())
	{
		QList<QStandardItem *> listRow;
		for(int i = 0; i < listItem.count(); ++i)
		{
			listRow.append(listItem.at(i));
			if((i + 1) % m_pRecordDlg->m_pQueryResultView->header()->count()  == 0 )
			{
				pModel->appendRow(listRow);
				listRow.clear();
			}
		}
		m_pRecordDlg->m_plblTotalRecord->setText(QString::fromLocal8Bit("满足条件共：<font color=""red"">%1 </font>条记录").arg(m_mapRowToInfo.count()));
	}
	else
	{
		m_pRecordDlg->m_plblTotalRecord->setText(QString::fromLocal8Bit("满足条件共：<font color=""red"">%1 </font>条记录").arg(0));
	}
}

void QMedicalRecordDlg::slotScanDetail()
{
	int nRowIndex = m_pRecordDlg->m_pQueryResultView->currentIndex().row() + 1;

	tagPersonInfo *pInfo = m_mapRowToInfo.value(nRowIndex);
	if(pInfo != NULL)
	{
		QPainterResult dlg(*pInfo, this);
		dlg.exec();
	}
	//QPrinter printer;

	////// 默认选择自带打印机
	////QList<QPrinterInfo> listPrinter = QPrinterInfo::availablePrinters();
	////foreach(QPrinterInfo info, listPrinter)
	////{
	////	if(info.printerName().contains("HP LaserJet 1020"))
	////	{
	////		printer.setPrinterName(info.printerName());
	////		break;
	////	}
	////}

	////printer.setOutputFormat(QPrinter::PdfFormat);
	////printer.setOutputFileName("PrintDemo.pdf");
	////printer.setOutputFormat(QPrinter::NativeFormat);
	//QPrintDialog printDlg(&printer);
	//int nRet = printDlg.exec();
	////QMessageBox::information(this, QString("AAA"), QString::number(nRet) + "\n" + QString::number(printer.outputFormat()));
	//if(nRet == QDialog::Accepted)
	//{
	//	QPainter painter(&printer);
	//	QRect pageRect = printer.pageRect();
	//	
	//	int nViewWidth = pageRect.width();
	//	int nViewHeight = pageRect.height();

	//	// 绘制标题
	//	painter.save();
	//	{
	//		QFont fontTitle(QString::fromLocal8Bit("宋体"), 11);
	//		fontTitle.setBold(true);
	//		painter.setFont(fontTitle);
	//		QFontMetrics drawMetrics(painter.font());
	//		painter.setViewport(0, 0, nViewWidth, nViewHeight*0.05f);
	//		painter.setWindow(0, 0, nViewWidth, drawMetrics.height() * 1.5);
	//		painter.drawText(QRect(0, drawMetrics.height()*0.25, nViewWidth - 10, drawMetrics.height()),  Qt::AlignCenter, QString::fromLocal8Bit("乐普医疗动脉硬化检测报告"));
	//	}
	//	painter.restore();

	//	// 绘制基本信息的设置  
	//	painter.save();
	//	{
	//		QPen pen;
	//		pen.setWidthF(1.0f);
	//		pen.setColor(Qt::black);
	//		pen.setStyle(Qt::SolidLine);
	//		painter.setPen(pen);
	//		painter.setViewport(5, 5 + nViewHeight*0.05f, nViewWidth - 10, nViewHeight*0.1f);
	//		QDrawMeasure draw;
	//		draw.drawPersonInfo(&painter, *pInfo);
	//	}
	//	painter.restore();

	//	// 绘制脉搏波数据
	//	painter.save();
	//	{
	//		//painter.setViewport(5 + (pageRect.width() - 10 - nMinSide)/2, 5+pageRect.height()*0.15f + ((int)(pageRect.height()*0.35) - nMinSide)/2, nMinSide, nMinSide);
	//		painter.setViewport(5, 5 + nViewHeight*0.15f, nViewWidth - 10, nViewHeight*0.30f);
	//		QDrawMeasure draw;
	//		draw.drawWaveInfo(&painter, *pInfo);
	//	}
	//	painter.restore();


	//	// 绘制血压值数据
	//	painter.save();
	//	{
	//		painter.setViewport(5, 5+nViewHeight*0.45f, (nViewWidth-10)/2, nViewHeight*0.25f);
	//		QDrawMeasure draw;
	//		draw.drawPressure(&painter, *pInfo);
	//	}
	//	painter.restore();

	//	// 绘制PWV分析
	//	painter.save();
	//	{
	//		//float nSide = qMin((pageRect.width()-10)/2.0f, pageRect.height()*0.25f);
	//		painter.setViewport(5+(nViewWidth-10)/2, 5+nViewHeight*0.45f, (nViewWidth-10)/2, nViewHeight*0.25f);
	//		QDrawMeasure draw;
	//		draw.drawPwvAnalysis(&painter, *pInfo);
	//	}
	//	painter.restore();


	//	// 绘制ABI及PWV综合分析信息
	//	painter.save();
	//	{
	//		painter.setViewport(5, 5+nViewHeight*0.70f, 
	//			(nViewWidth - 10)/2, nViewHeight*0.25f);
	//		QDrawMeasure draw;
	//		draw.drawAnalysis(&painter, *pInfo, true);
	//	}
	//	painter.restore();

	//	// 绘制结论信息
	//	painter.save();
	//	{
	//		painter.setViewport(5+(nViewWidth - 10)/2, 5+nViewHeight*0.70f, 
	//			(nViewWidth -10)/2, nViewHeight*0.25f);
	//		QDrawMeasure draw;
	//		draw.drawResult(&painter, *pInfo);
	//	}
	//	painter.restore();

	//	// 绘制大的边框
	//	painter.save();
	//	{
	//		painter.setViewport(0, 0, nViewWidth, nViewHeight);
	//		painter.setWindow(0, 0, nViewWidth, nViewHeight);
	//		QPen pen;
	//		pen.setColor(QColor(0,0,0));
	//		pen.setWidthF(4.0f);
	//		pen.setStyle(Qt::SolidLine);
	//		painter.setPen(pen);
	//		painter.drawRect(5, 5 + nViewHeight*0.05f, nViewWidth - 10, nViewHeight*0.90);
	//	}
	//	painter.restore();
	//	//return;
 //  }
}
