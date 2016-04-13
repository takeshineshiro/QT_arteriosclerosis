#include "./header/stdafx.h"
#include "./header/ui_PersonBasicInfo.h"
#include <QMessageBox>
#include "./header/QPersonBasicInfoDlg.h"


QPersonBasicInfoDlg::QPersonBasicInfoDlg(QWidget *pParent)
	:QWidget(pParent)
{
	m_pPersonInfo = new Ui::PersonBasicInfo();
	m_pPersonInfo->setupUi(this);
	setFixedWidth(320);

	// 编号是年月日时分秒组成,如20151010162930(14位)
	m_pPersonInfo->m_pPersonNo->setValidator(new QRegExpValidator(QRegExp("[1-9][0-9]{13,13}"), this));

	// 姓名的最大长度为30
	m_pPersonInfo->m_pedName->setValidator(new QRegExpValidator(QRegExp(".{1,30}"), this));

	// 年龄：两位数或者三位数
	m_pPersonInfo->m_pedAge->setValidator(new QRegExpValidator(QRegExp("[1-9][0-9]{0,2}"), this));

	// 身高：两位数或者三位数
	m_pPersonInfo->m_pedHeight->setValidator(new QRegExpValidator(QRegExp("[1-9][0-9]{0,2}"), this));

	connect(m_pPersonInfo->m_pedNote, SIGNAL(textChanged()), this, SLOT(slotNoteTextChange()));

	//设置初始值
	m_pPersonInfo->m_pPersonNo->setCurrentText(QDateTime::currentDateTime().toString("yyyyMMddHHmmss"));
}

QPersonBasicInfoDlg::~QPersonBasicInfoDlg()
{
	delete m_pPersonInfo;
	m_pPersonInfo = NULL;
}

void QPersonBasicInfoDlg::slotNoteTextChange()
{
	QString strTextContent = m_pPersonInfo->m_pedNote->toPlainText(); 
	int length = strTextContent.count();

	int maxLength = 300; // 最大字符数

	if(length > maxLength)
	{
		int position = m_pPersonInfo->m_pedNote->textCursor().position();
		QTextCursor textCursor = m_pPersonInfo->m_pedNote->textCursor();
		strTextContent.remove(position - (length - maxLength), length - maxLength);

		m_pPersonInfo->m_pedNote->setText(strTextContent);
		textCursor.setPosition(position - (length - maxLength));
		m_pPersonInfo->m_pedNote->setTextCursor(textCursor);
		m_pPersonInfo->m_plblNote->setText(QString::fromLocal8Bit("备注(您还可以输入%1字)").arg(0));
	}
	else
	{
		m_pPersonInfo->m_plblNote->setText(QString::fromLocal8Bit("备注(您还可以输入%1字)").arg(maxLength - length ));
	}
}

QString QPersonBasicInfoDlg::getPersonName() const
{
	return m_pPersonInfo->m_pedName->text();
}

void QPersonBasicInfoDlg::resetInfo()
{
	m_pPersonInfo->m_pPersonNo->setCurrentText(QDateTime::currentDateTime().toString("yyyyMMddHHmmss"));
	m_pPersonInfo->m_pedName->clear();
	m_pPersonInfo->m_pedAge->clear();
	m_pPersonInfo->m_pedHeight->clear();
	m_pPersonInfo->m_pcboxSection->clear();
}

tagPersonInfo  QPersonBasicInfoDlg::getPersonInfo()
{
	tagPersonInfo tagInfo;
	tagInfo.m_strNo =  m_pPersonInfo->m_pPersonNo->currentText();
	tagInfo.m_strName = m_pPersonInfo->m_pedName->text();
	tagInfo.m_strSex = m_pPersonInfo->m_pcboxSex->currentText();
	tagInfo.m_strAge = m_pPersonInfo->m_pedAge->text();
	tagInfo.m_strHeight = m_pPersonInfo->m_pedHeight->text();
	tagInfo.m_strWeight = m_pPersonInfo->m_pedWeight->text();
	tagInfo.m_strSection = m_pPersonInfo->m_pcboxSection->currentText();
	tagInfo.m_strDoctor = m_pPersonInfo->m_pedDoctor->text();

	tagInfo.m_strNote = m_pPersonInfo->m_pedNote->toPlainText();
	if(m_pPersonInfo->m_pbtnNonHistory->checkState() == Qt::Checked)
	{
		tagInfo.m_listHistory<<QString::fromLocal8Bit("无");
	}
	else
	{
		if(m_pPersonInfo->m_pbtnHighBlood->checkState() == Qt::Checked)
		{
			tagInfo.m_listHistory<<QString::fromLocal8Bit("高血压");
		}

		if(m_pPersonInfo->m_pbtnHyperlipidemia->checkState() == Qt::Checked)
		{
			tagInfo.m_listHistory<<QString::fromLocal8Bit("高血脂");
		}

		if(m_pPersonInfo->m_pbtnCHD->checkState() == Qt::Checked)
		{
			tagInfo.m_listHistory<<QString::fromLocal8Bit("冠心病");
		}

		if(m_pPersonInfo->m_pbtnDHF->checkState() == Qt::Checked)
		{
			tagInfo.m_listHistory<<QString::fromLocal8Bit("心衰");
		}

		if(m_pPersonInfo->m_pbtnHyperglycemia->checkState() == Qt::Checked)
		{
			tagInfo.m_listHistory<<QString::fromLocal8Bit("高血糖");
		}
	}

	return tagInfo;
}

// 检查用户输入的数据是否正确
bool QPersonBasicInfoDlg::isValidInput()
{
	bool bRet = false;
	// 判断编号是否为空
	if (m_pPersonInfo->m_pPersonNo->currentText().isEmpty())
	{
		QMessageBox::information(this, QString::fromLocal8Bit("编号输入错误"),
			QString::fromLocal8Bit("输入的编号为空或者错误！"));
		return bRet;
	}

	// 判断姓名
	if (m_pPersonInfo->m_pedName->text().isEmpty())
	{
		QMessageBox::information(this, QString::fromLocal8Bit("姓名为空"),
			QString::fromLocal8Bit("请输入患者姓名！"));
		return bRet;
	}

	// 判断年龄
	if (m_pPersonInfo->m_pedAge->text().isEmpty())
	{
		QMessageBox::information(this, QString::fromLocal8Bit("年龄错误"),
			QString::fromLocal8Bit("请输入患者的年龄！"));
		return bRet;
	}
	
	bRet = true;

	return bRet;
}
