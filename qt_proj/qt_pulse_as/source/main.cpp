#include "./header/stdafx.h"
#include "./header/measurearteriosclerosis.h"
#include "./header/QDrawWaveWidget.h"
#include <QtWidgets/QApplication>
#include <QDialogButtonBox>
//#include "./header/QPushButtonDemo.h"
#include "./header/QReadAndWriteXml.h"
#include "./header/QPainterResult.h"
#include "./header/QShowMeasureResult.h"
#include "./header/win_qextserialport.h"

// 引入DLL
#ifdef __cplusplus
extern "C"
{
#include "fenlishuju_and_lvbo_2.h"
}
#else
#include "fenlishuju_and_lvbo_2.h"
#endif

#include "PackageSendCmd.h"

int main(int argc, char *argv[])
{
	//suanfa3(3,4);
	//int n = 10;
	//
	//QRegExp rx(QString("2015+"));
	//bool bRet = rx.isValid();
	//int nPos = rx.indexIn(QString::fromLocal8Bit("20151111162026(马富军).xml"));
	//QRegExp rx1(QString(".xml$"));
	////rx1.setPatternSyntax(QRegExp::Wildcard);
	//bRet = rx1.isValid();
	//nPos = rx1.indexIn(QString::fromLocal8Bit("20151111162026(马富军).xml"));
	////match integers 0 to 99
	////QRegExp rx("^\\d\\d?$");
	////qDebug()<<rx.indexIn("123");
	////qDebug()<<rx.indexIn("-12");
	////qDebug()<<rx.indexIn("12");
	////qDebug()<<rx.indexIn("1");
	//double dAnkle_left_bloodPress = 0.0;
	//double dAnkle_left_Diastolic = 0.0;
	//double dAnkle_left_dingdian = 0.0;
	//double dAnkle_right_bloodPress = 0.0;
	//double dAnkle_right_Diastolic = 0.0;
	//double dAnkle_right_dingdian = 0.0;
	//double dBrachial_left_bloodPress = 0.0;
	//double dBrachial_left_Diastolic = 0.0;
	//double dBrachial_left_dingdian = 0.0;
	//double dBrachial_right_bloodPress = 0.0;
	//double dBrachial_right_Diastolic = 0.0;
	//double dBrachial_right_dingdian = 0.0;
	//double dleft_ABI = 0.0;
	//double dright_ABI = 0.0;
	//double dleft_PWV = 0.0;
	//double dright_PWV = 0.0;

	//// 改为自己的路径
	//char * const arrDir = "E:\\Lepu_20150810\\LepuMedical\\SvnFile\\MeasureArteriosclerosis(VS)\\MeasureArteriosclerosis\\MeasureArteriosclerosis\\Data\\";
	//
	//// 路径下的文件名
	//char *arrFileName[100] = {"20151126160708(马富军).txt"};
	//
	//qDebug()<<("Part\t\tbloodPress\tDiastolic\tdingdian\tABI\tPWV\n");
	//for(int i = 0; i <100 && arrFileName[i] != NULL; ++i)
	//{
	//	qDebug(("%s\n", arrFileName[i]));
	//	char fileAbs[256] = {};
	//	strcat(fileAbs, arrDir);
	//	strcat(fileAbs, arrFileName[i]);

	//	suanfa(fileAbs, arrDir,
	//		&dAnkle_left_bloodPress, &dAnkle_left_Diastolic, &dAnkle_left_dingdian,
	//		&dAnkle_right_bloodPress, &dAnkle_right_Diastolic, &dAnkle_right_dingdian,
	//		&dBrachial_left_bloodPress, &dBrachial_left_Diastolic, &dBrachial_left_dingdian,
	//		&dBrachial_right_bloodPress, &dBrachial_right_Diastolic, &dBrachial_right_dingdian,
	//		&dleft_ABI, &dright_ABI, &dleft_PWV, &dright_PWV);

	//	qDebug("Left Ankle:\t\t%f\t%f\t%f\t%f\t%f\n", dAnkle_left_bloodPress,
	//		dAnkle_left_Diastolic,dAnkle_left_dingdian,dleft_ABI,dleft_PWV);
	//	qDebug("Right Ankle:\t\t%f\t%f\t%f\t%f\t%f\n", dAnkle_right_bloodPress,
	//		dAnkle_right_Diastolic,dAnkle_right_dingdian,dright_ABI,dright_PWV);
	//	qDebug("Left Brachial:\t\t%f\t%f\t%f\t%f\t%f\n", dBrachial_left_bloodPress,
	//		dBrachial_left_Diastolic,dBrachial_left_dingdian,0,0);
	//	qDebug("Right Brachial:\t\t%f\t%f\t%f\t%f\t%f\n", dBrachial_right_bloodPress,
	//		dBrachial_right_Diastolic,dBrachial_right_dingdian,0,0);
	//}
	//int nInput = 0;
	//scanf("%d", &nInput);
	//char * const arrFileName = "E:\\Lepu_20150810\\LepuMedical\\SvnFile\\MeasureArteriosclerosis(VS)\\MeasureArteriosclerosis\\MeasureArteriosclerosis\\Data\\20150929093139.txt";
	//QDir::setCurrent("E:\\Lepu_20150810\\LepuMedical\\SvnFile\\MeasureArteriosclerosis(VS)\\MeasureArteriosclerosis\\MeasureArteriosclerosis\\Data\\1200Hz");
	//QFileInfoList listInfo = QDir::current().entryInfoList();

	//QMap<QString, QString> mapFile1ToFile2;

	//foreach (QFileInfo fileName, listInfo)
	//{
	//	QString strFileName = fileName.fileName();
	//	if(!strFileName.contains(".txt"))
	//	{
	//		continue;
	//	}

	//	QString strLeft =  strFileName.left(strFileName.indexOf("_"));
	//	QString strFile1 = strLeft +"_1.txt";
	//	QString strFile2 = strLeft +"_2.txt";
	//	mapFile1ToFile2[strFile1] = strFile2;
	//}

	//QMap<QString, QString>::const_iterator iter = mapFile1ToFile2.begin();
	//while(iter != mapFile1ToFile2.end())
	//{
	//	int nReceviedCount = 0;
	//	QMap<int, int> mapIndexToFirst;

	//	QFile file1(iter.key());
	//	if(file1.open(QFile::ReadOnly))
	//	{
	//		QTextStream in(&file1);
	//		while(!in.atEnd())
	//		{
	//			QStringList strValue = in.readLine().remove("\r\n").split("; ");
	//			int nLeftAnkle = strValue.at(0).toInt();
	//			int nRightAnkle = strValue.at(1).toInt();
	//			int nLeftArm = strValue.at(2).toInt();
	//			int nRightArm = strValue.at(3).toInt();

	//			if(nRightArm == 65535 && nLeftArm == 65535)
	//			{
	//				mapIndexToFirst[nRightAnkle] = nLeftAnkle;
	//			}

	//			if(!mapIndexToFirst.count(1))
	//			{
	//				++nReceviedCount;
	//			}
	//		}
	//	}
	//	file1.close();

	//	QFile file2(iter.value());
	//	if(file2.open(QFile::ReadOnly))
	//	{
	//		QTextStream in(&file2);
	//		while(!in.atEnd())
	//		{
	//			QStringList strValue = in.readLine().remove("\r\n").split("; ");
	//			int nLeftAnkle = strValue.at(0).toInt();
	//			int nRightAnkle = strValue.at(1).toInt();
	//			int nLeftArm = strValue.at(2).toInt();
	//			int nRightArm = strValue.at(3).toInt();

	//			if(nRightArm == 65535 && nLeftArm == 65535)
	//			{
	//				mapIndexToFirst[nRightAnkle] = nLeftAnkle;
	//			}

	//			if(!mapIndexToFirst.count(1))
	//			{
	//				++nReceviedCount;
	//			}
	//		}
	//	}
	//	file2.close();
	//	int nTransferCount = 0;

	//	if(mapIndexToFirst.count(0))
	//	{
	//		nTransferCount = mapIndexToFirst[0]*5000;
	//	}

	//	if(mapIndexToFirst.count(1))
	//	{
	//		nTransferCount += mapIndexToFirst[1];
	//	}
	//	QString str = QString::fromLocal8Bit("File1Name:%1 File2Name:%2 ReceviedCount:%3 TransferCount:%4 Diff:%5 Per:%6").arg(iter.value())
	//		.arg(iter.value()).arg(nReceviedCount).arg(nTransferCount).arg(nTransferCount-nReceviedCount).arg(QString::number((nTransferCount-nReceviedCount)*100.0f/nTransferCount,'f', 6));
	//	qDebug()<<str;	
	//	//qDebug()<<"File1Name"<<"\t"<<iter.key()<<"File2Name"<<"\t"<<iter.value()<<"ReceviedCount"<<"\t"<<nReceviedCount<<"\t"<<"TransferCount:"<<"\t"<<nTransferCount<<"\t"<<"Diff"<<"\t"<<(nTransferCount - nReceviedCount);
	//	++iter;
	//}

	//	qDebug()<<fileName.fileName();
	//	char fileAbs[256] = {};
	//	strcat(fileAbs, arrDir);
	//	strcat(fileAbs, QString(fileName.fileName()).toStdString().c_str());

	//	suanfa(fileAbs, arrDir,
	//		&dAnkle_left_bloodPress, &dAnkle_left_Diastolic, &dAnkle_left_dingdian,
	//		&dAnkle_right_bloodPress, &dAnkle_right_Diastolic, &dAnkle_right_dingdian,
	//		&dBrachial_left_bloodPress, &dBrachial_left_Diastolic, &dBrachial_left_dingdian,
	//		&dBrachial_right_bloodPress, &dBrachial_right_Diastolic, &dBrachial_right_dingdian,
	//		&dleft_ABI, &dright_ABI, &dleft_PWV, &dright_PWV);
	//	printf("Part\t\tbloodPress\tDiastolic\tdingdian\tABI\tPWV\n");
	//	printf("Left Ankle:\t\t%f\t%f\t%f\t%f\t%f\n", dAnkle_left_bloodPress,
	//		dAnkle_left_Diastolic,dAnkle_left_dingdian,dleft_ABI,dleft_PWV);
	//	printf("Right Ankle:\t\t%f\t%f\t%f\t%f\t%f\n", dAnkle_right_bloodPress,
	//		dAnkle_right_Diastolic,dAnkle_right_dingdian,dright_ABI,dright_PWV);
	//	printf("Left Brachial:\t\t%f\t%f\t%f\t%f\t%f\n", dBrachial_left_bloodPress,
	//		dBrachial_left_Diastolic,dBrachial_left_dingdian,0,0);
	//	printf("Right Brachial:\t\t%f\t%f\t%f\t%f\t%f\n", dBrachial_right_bloodPress,
	//		dBrachial_right_Diastolic,dBrachial_right_dingdian,0,0);
	//}
	//int nInput = 0;
	//scanf("%d", &nInput);
	//match strings without whitespace
	//QRegExp rx("^\\S+$");
	//qDebug()<<rx.indexIn("Hello World!");
	//qDebug()<<rx.indexIn("Hello_World!");

	// match strings containing 'mail' or 'letter' or 'correspondence' but only match whole words i.e. not 'email'
	//QRegExp rx("\\b(mail|letter|correspondence)\\b");
	//qDebug()<<rx.indexIn("email");
	//qDebug()<<rx.indexIn("This is a mail letter.");
	//qDebug()<<rx.cap(1);


	// match ampersands but not &amp;
	//QRegExp rx("&(?!amp;)");
	//QString strLine1("This is a & ");
	//strLine1.replace(rx, "&amp");
	//qDebug()<<strLine1;
	//QString strLine2("This is a &amp; And & .");
	//strLine2.replace(rx, "&amp;");
	//qDebug()<<strLine2;


	// match Eric or Eirik
	//QString str = "One Eric another Eirik, and an Ericsson. "
	//                 "How many Eiriks, Eric?";
	//QRegExp rx("\\b(Eric|Eirik)\\b");
	//QRegExp rx("\\b(Ei?ri[ck])\\b");
	//int pos = 0;
	//int nCount = 0;
	//while(pos >= 0)
	//{
	//	 pos = rx.indexIn(str, pos);
	//	 qDebug()<<rx.cap();
	//	 if(pos >= 0)
	//	 {
	//		 pos += rx.matchedLength();
	//		 ++nCount;
	//	 }
	//}
	//qDebug()<<nCount;

	// QString strReg("(1030");
	// strReg += "|";
	// strReg += "ma)";
	// QRegExp rx(strReg);
	// bool bValiad = rx.isValid();
	// int pos = rx.indexIn("20121030(ma)");
	// pos = rx.indexIn("20121130(fu)");

	//QString strTime = QDateTime::currentDateTime().toString(QString::fromLocal8Bit("yyyy年MM月dd日 HH时mm分ss秒"));

//QSettings set("AAA", QSettings::IniFormat);
//set.setValue("BAUD", 128000);
	Q_INIT_RESOURCE(Icon);

	QApplication a(argc, argv);
	MeasureArteriosclerosis w;
	//Win_QextSerialPort *myCom = new Win_QextSerialPort("COM9",QextSerialBase::EventDriven);

	//bool bRet = myCom ->open(QIODevice::ReadWrite);

	////以读写方式打开串口
	//myCom->setBaudRate(BAUD128000);

	////波特率设置，我们设置为9600

	//myCom->setDataBits(DATA_8);

	////数据位设置，我们设置为8位数据位

	//myCom->setParity(PAR_NONE);

	////奇偶校验设置，我们设置为无校验

	//myCom->setStopBits(STOP_1);

	////停止位设置，我们设置为1位停止位

	//myCom->setFlowControl(FLOW_OFF);

	////数据流控制设置，我们设置为无数据流控制

	////myCom->setTimeout(500);

	//PackageSendCmd sendCmd;
	//QString strStart = sendCmd.getNoramlStart();
	//strStart = "AAA";
	//int nSize = myCom->write(QByteArray::fromHex(sendCmd.getNoramlStart().toLocal8Bit()));
	//QString str = myCom->errorString();
	//str = "AAA";
	//
	//// 等待5秒钟
	//while(!myCom->waitForReadyRead(5*1000))
	//{
	//	//file.write(info.manufacturer().toLocal8Bit() + "DDDD" + QString::number(timer.elapsed()).toLocal8Bit() + "\r\n");
	//	QByteArray byteRecv = myCom->readAll().toHex();
	//	byteRecv = "AAAA";
	//}

	//延时设置，我们设置为延时500ms,这个在Windows下好像不起作用

	//connect(myCom,SIGNAL(readyRead()),this,SLOT(readMyCom()));


	w.show();
	//tagPersonInfo tagInfo;
	//tagInfo.m_strNo = "20151105104930";
	//tagInfo.m_strName = QString::fromLocal8Bit("马富军");
	//tagInfo.m_strAge = "25";
	//tagInfo.m_strSex = QString::fromLocal8Bit("男");
	//tagInfo.m_strDoctor = QString::fromLocal8Bit("张三");
	//tagInfo.m_listHistory<<QString::fromLocal8Bit("高血压")<<QString::fromLocal8Bit("高血糖")<<QString::fromLocal8Bit("高血脂");
	//tagInfo.m_strDate = "2015-10-31 11:50:40";
	//tagInfo.m_strHeight = "168cm";
	//tagInfo.m_strWeight = "70Kg";
	//tagInfo.m_strNote = QString::fromLocal8Bit("中国");
	//tagInfo.m_strSection = QString::fromLocal8Bit("内科");
	//tagInfo.m_listLeftArm<<"129"<<"85"<<"119";
	//tagInfo.m_listLeftAnkle<<"129"<<"123"<<"118"<<"1.00"<<"1223";
	//tagInfo.m_listRightArm<<"127"<<"119"<<"115";
	//tagInfo.m_listRightAnkle<<"130"<<"116"<<"117"<<"1.02"<<"1501";
	//tagInfo.m_strDiagnose = QString::fromLocal8Bit("您本次左侧baPWV测量结果: 左侧外周动脉柔软。\n"
	//                                                  "您本次右侧baPWV测量结果: 右侧外周动脉正常。\n"
	//                                                  "您本次左侧ABI测量结果: 左侧外周动脉正常。\n"
	//                                                  "您本次右侧ABI测量结果: 右侧外周动脉正常。");

	//QPainterResult dlg(tagInfo);
	//dlg.show();

	//QPushButtonDemo pushButton;
	//pushButton.setMinimumSize(QSize(80,80));
	////pushButton.setText(QStringLiteral("开始"));
	//pushButton.show();

	//QDir  dir(QString::fromLocal8Bit("E:/Lepu_20150810/LepuMedical/SvnFile/MeasureArteriosclerosis(VS)/MeasureArteriosclerosis/MeasureArteriosclerosis/Data"), QString::fromLocal8Bit("*.txt"));
	//QDir::setCurrent(QString::fromLocal8Bit("E:/Lepu_20150810/LepuMedical/SvnFile/MeasureArteriosclerosis(VS)/MeasureArteriosclerosis/MeasureArteriosclerosis/Data"));
	//foreach(const QString &info, dir.entryList())
	//{
	//	QFile file(info);
	//	int nMaxLeftArm = 0;
	//	int nMaxLeftAnkle = 0;
	//	int nMaxRightArm = 0;
	//	int nMAxRightAnkle =0;
	//	if(file.open(QFile::ReadOnly))
	//	{
	//		QTextStream in(&file);
	//		while(!in.atEnd())
	//		{
	//			QStringList strValue = in.readLine().remove("\r\n").split("; ");
	//			int nLeftAnkle = strValue.at(0).toInt();
	//			int nRightAnkle = strValue.at(1).toInt();
	//			int nLeftArm = strValue.at(2).toInt();
	//			int nRightArm = strValue.at(3).toInt();

	//			nMaxLeftArm = nLeftArm>nMaxLeftArm?nLeftArm:nMaxLeftArm;
	//			nMaxLeftAnkle = nLeftAnkle>nMaxLeftAnkle?nLeftAnkle:nMaxLeftAnkle;
	//			nMaxRightArm = nRightArm>nMaxRightArm?nRightArm:nMaxRightArm;
	//			nMAxRightAnkle = nRightAnkle>nMAxRightAnkle?nRightAnkle:nMAxRightAnkle;
	//		}
	//	}
	//	
	//	qDebug()<<info<<"\t"<<nMaxLeftArm<<"\t"
	//		<<nMaxRightArm<<"\t"<<nMaxLeftAnkle<<"\t"<<nMAxRightAnkle;
	//}

	return a.exec();
}


//#include <QtGui>

//int main(int argc, char **argv)
//{
//    QApplication app(argc, argv);
//    QMainWindow win;
//    QColumnView *cview = new QColumnView;
//	QList<int> listWidth;
//	listWidth.push_back(cview->width()/2);
//	listWidth.push_back(cview->width()/2);
//	cview->setColumnWidths(listWidth);
//    win.setCentralWidget(cview);
//
//    /* Create the data model */
//    QStandardItemModel model;
//
//    for (int groupnum = 0; groupnum < 3 ; ++groupnum)
//    {
//        /* Create the phone groups as QStandardItems */
//        QStandardItem *group = new QStandardItem(QString("Group %1").arg(groupnum));
//
//        /* Append to each group 5 person as children */
//        for (int personnum = 0; personnum < 5 ; ++personnum)
//        {
//            QStandardItem *child = new QStandardItem(QString("Person %1 (group %2)").arg(personnum).arg(groupnum));
//            /* the appendRow function appends the child as new row */
//            group->appendRow(child);
//        }
//        /* append group as new row to the model. model takes the ownership of the item */
//        model.appendRow(group);
//    }
//
//    cview->setModel(&model);
//
//    win.show();
//    return app.exec();
//}
