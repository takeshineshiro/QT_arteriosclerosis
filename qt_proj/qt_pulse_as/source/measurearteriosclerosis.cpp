#include "./header/stdafx.h"
#include <QTabWidget>
#include <QHBoxLayout>
#include "./header/QPersonBasicInfoDlg.h"
#include "./header/QTabManagerDlg.h"
#include "./header/measurearteriosclerosis.h"


MeasureArteriosclerosis::MeasureArteriosclerosis(QWidget *parent)
	: QDialog(parent)
{
	_initUI();
}

MeasureArteriosclerosis::~MeasureArteriosclerosis()
{
	delete m_pBasicInfo;
	m_pBasicInfo = NULL;

	delete m_pMainTab;
	m_pMainTab = NULL;

	delete m_pVersionInfo;
	m_pVersionInfo = NULL;

	delete m_pMainLayout;
	m_pMainLayout = NULL;

	delete m_pLayout;
	m_pLayout = NULL;
}

void MeasureArteriosclerosis::_initUI()
{
	m_pBasicInfo = new QPersonBasicInfoDlg(this);

	// 测量相关Tab
	m_pMainTab = new QTabManagerDlg(this);
	m_pMainTab->setMinimumWidth(m_pBasicInfo->width()*2);
	m_pMainTab->setPersonInfo(m_pBasicInfo);

	m_pMainLayout = new QHBoxLayout();
	m_pMainLayout->addWidget(m_pBasicInfo);
	m_pMainLayout->addSpacing(10);
	m_pMainLayout->addWidget(m_pMainTab);
	m_pMainLayout->setStretch(0, 0);
	m_pMainLayout->setStretch(1, 4);
	
	m_pLayout = new QVBoxLayout();
	m_pLayout->addLayout(m_pMainLayout, 4);
	m_pVersionInfo = new QLabel(QStringLiteral("VpsView(动脉硬化检测仪上位机),当前版本:1.0.0,Copyright © 乐普(北京)医疗器械股份有限公司 2015"));
	//m_pVersionInfo->setFont(QFont(QStringLiteral("宋体"), 6));
	m_pVersionInfo->setAlignment(Qt::AlignCenter);
	m_pLayout->addWidget(m_pVersionInfo);
	setLayout(m_pLayout);
	
	setWindowFlags(Qt::Dialog | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
	setWindowTitle(QStringLiteral("动脉硬化检测仪(上位机)"));
	setWindowIcon(QIcon(":/AppIcon/Images/AppLogo.png"));
	//setWindowIconText(QStringLiteral("动脉硬化检测仪"));
}
