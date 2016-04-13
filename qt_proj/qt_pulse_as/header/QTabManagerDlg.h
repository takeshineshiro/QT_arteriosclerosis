/******************************************************
作用：动脉硬化检测仪，各个功能Tab页的管理功能

******************************************************/


class QTabWidget;
class QMeasureDlg;
class QShowMeasureResult;
class QPersonBasicInfoDlg;
class QMedicalRecordDlg;

class QTabManagerDlg:
	public QTabWidget
{
	Q_OBJECT
public:
	QTabManagerDlg(QWidget *pParent = NULL);

	~QTabManagerDlg();

	// 设置患者信息
	void setPersonInfo(QPersonBasicInfoDlg *pPersonInfoDlg);

protected slots:

	// 切换tab页到测量结果页
	void slotResult(const QString &, const tagPersonInfo&);

protected:

	// 改变大小
	void resizeEvent(QResizeEvent *pEvent);

private:
	
	// UI初始化
	void _initUI();

private:

	// 测量显示Tab
	QMeasureDlg *m_pMeasureDlg;

	// 测量结果展示页
	QShowMeasureResult *m_pResult;

	// 病历历史查询
	QMedicalRecordDlg *m_pQueryHistory;
};

