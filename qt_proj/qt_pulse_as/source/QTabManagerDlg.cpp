#include "./header/stdafx.h"
#include <QTabWidget>
#include "./header/QPersonBasicInfoDlg.h"
#include "./header/QMeasureDlg.h"
#include "./header/QTabManagerDlg.h"
#include "./header/QShowMeasureResult.h"
#include "./header/QMedicalRecordDlg.h"

QTabManagerDlg::QTabManagerDlg(QWidget *pParent)
	:QTabWidget(pParent)
{
	m_pResult = NULL;
	m_pQueryHistory = NULL;
	_initUI();
}


QTabManagerDlg::~QTabManagerDlg(void)
{
	Q_DELETE(m_pMeasureDlg);
	Q_DELETE(m_pResult);
	Q_DELETE(m_pQueryHistory);
}

// UI初始化
void QTabManagerDlg::_initUI()
{
	setLayoutDirection(Qt::LeftToRight);
	setTabPosition(QTabWidget::South);
	setTabShape(QTabWidget::Triangular);
	setIconSize(QSize((width()-100)/4, 40));

	//setElideMode(Qt::ElideNone);
	//setUsesScrollButtons(true);
	//setDocumentMode(false);
	//setTabsClosable(false);
	//setMovable(false);
	//setTabBarAutoHide(false);

	// 新建测量页面
	m_pMeasureDlg = new QMeasureDlg(this);
	addTab(m_pMeasureDlg,QStringLiteral("测量显示"));

	m_pResult = new QShowMeasureResult(this);
	m_pResult->setMinimumSize(this->size());
	addTab(m_pResult,QStringLiteral("测量结果"));

	m_pQueryHistory = new QMedicalRecordDlg(this);
	addTab(m_pQueryHistory,QStringLiteral("病历查询"));

	// 测量结束后，切换到测量结果显示页
	connect(m_pMeasureDlg, SIGNAL(sigRecvTestEndCmd(const QString &, const tagPersonInfo&)), this, SLOT(slotResult(const QString &,const tagPersonInfo&)));
	connect(m_pMeasureDlg, SIGNAL(sigRecvTestEndCmd(const QString &, const tagPersonInfo&)), m_pResult, SLOT(slotMeasureResult(const QString &,const tagPersonInfo&)));
}

void QTabManagerDlg::resizeEvent(QResizeEvent *pEvent)
{
	setIconSize(QSize((width()-100)/4, 60));
	update();
}

void QTabManagerDlg::slotResult(const QString &,const tagPersonInfo&)
{
	setCurrentWidget(m_pResult);
}

void QTabManagerDlg::setPersonInfo(QPersonBasicInfoDlg *pPersonInfoDlg)
{
	m_pMeasureDlg->setPersonInfo(pPersonInfoDlg);
}
