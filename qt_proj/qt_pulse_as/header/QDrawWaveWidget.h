#pragma once

#include <QTimer>
#include <QElapsedTimer>
#include <QOpenGLWidget>
#include <QVector>

class QOpenGLShader;
class QOpenGLShaderProgram;
class QOpenGLBuffer;

class QDrawWaveWidget :public QOpenGLWidget
{
	Q_OBJECT
public:
	
	QDrawWaveWidget(QWidget *pParent = NULL);
	
	virtual ~QDrawWaveWidget(void);

	// 停止测量
	void stopMeasure();

	// 开始测量
	void startMeasure();

	// 更新测试数据
	void appendData(int *pPressureData);

public slots:
	// 更新测量数据，
	// 参数： pDrawPressure 描画点的值
	//        pShowPressure 显示的压力值
	void appendData(QVector<float> *pDrawPressure, QVector<int> arrPressure, bool bClearData = false);

signals:
	void sigStartTimer();

protected slots:
	
	// 保存图片
    void slotSavePicture();

protected:
	// 重写初始化
    virtual void initializeGL();

	// 重写重绘
    virtual void resizeGL(int nWidth, int nHeight);

	// 重写描绘操作
    virtual void paintGL();

private:

	// 初始化颜色及压力值数据
	void _initPressureAndColor(QOpenGLBuffer &buffer);

	// 原始波形数据处理函数
	// 参数信息: vectPressure 下位机传递的压力值
	//           nIndex 0、1、2、3分别表示右臂、左臂、右踝、左踝
	//           bReset 是否重置滤波参数
	inline QVector<int> _analysisPlus(QVector<int> &vectPressure, const int &nIndex = 0, const bool &bReset = false);


	inline void _updatePlusWave();
private:
	// 顶点shader
	QOpenGLShader *m_pVertexShader;

	// 片段shader
	QOpenGLShader *m_pFrameShader;

	// 链接器
	QOpenGLShaderProgram *m_pProgramShader;

	// buffer
	QOpenGLBuffer *m_pBuffer;

	// 顶点属性位置
	int m_nVertexAttr;
	// 片段属性位置 
	int m_nFragAttr;
	// 顶点操作 
	int m_nUniform;

	// 上次绘制点
	volatile int m_nLastDrawIndex;
	// 上次绘制点访问锁
	QMutex mutexLastDrawIndex;

	// 是否需要擦除
	bool m_bIsFull;

	// 计时，定时更新坐标
	QElapsedTimer timer;

	// 绘制时间计数器
	QTimer m_timerDraw;
	
	// 右臂压力
	GLint m_nRightArmPressure;

	// 左臂压力
	GLint m_nLeftArmPressure;

	// 右踝压力
	GLint m_nRightAnklePressure;

	// 左踝压力
	GLint m_nLeftAnklePressure;

	bool m_bHasMax[4];

	float m_fMaxDiff;

	// 保存脉搏波数据
	QVector<GLfloat> m_arrPlusWave[4];

	QMutex m_mutexPlusWave;

	//// Timer生存的线程环境
	//QThread *m_pTimerThread;
};
