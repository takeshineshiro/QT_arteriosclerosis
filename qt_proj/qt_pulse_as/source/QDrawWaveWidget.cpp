#include "./header/stdafx.h"
#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QPainter>
#include <QTimer>
#include <QGuiApplication>
#include <QScreen>
#include <QPushButton>
#include <QDebug>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include "./header/QDrawWaveWidget.h"

// 总点数
const int nPOINT_COUNT = 6400;
const int nMAX_PRESSURE = 150.0f;

// 人体的四肢
const int nCOUNT_LEGS = 2;

// 每个脉搏波的波形点的个数大于65
const int nEVERY_PLUS_POINT_COUNT = 70;
//const int nBLACK_WIDTH = nPOINT_COUNT/25;

QDrawWaveWidget::QDrawWaveWidget(QWidget *pParent):
	QOpenGLWidget(pParent)
{
	m_nLastDrawIndex = 0;
	m_bIsFull = false;

	m_nLeftArmPressure = 0;
	m_nLeftAnklePressure = 0;
	m_nRightArmPressure = 0;
	m_nRightAnklePressure = 0;

	memset(m_bHasMax, 0x00, sizeof(m_bHasMax));
	
	m_timerDraw.setInterval(5.0f);
	//m_pTimerThread = new QThread();
	//m_timerDraw.moveToThread(QApplication::instance()->thread());

	//
	connect(&m_timerDraw, SIGNAL(timeout()), this, SLOT(repaint()));
	connect(this, SIGNAL(sigStartTimer()), &m_timerDraw, SLOT(start()));
}

QDrawWaveWidget::~QDrawWaveWidget(void)
{
	makeCurrent();
	Q_DELETE(m_pProgramShader);
	Q_DELETE(m_pVertexShader);
	Q_DELETE(m_pFrameShader);

	m_pBuffer->destroy();
	Q_DELETE(m_pBuffer);

	doneCurrent();
}

// 重写初始化
void QDrawWaveWidget::initializeGL()
{
	// 1. 创建编译程序
	m_pProgramShader = new QOpenGLShaderProgram(this);
	qDebug()<<"initializeGL";

	// 初始化OpengL函数库
	QOpenGLFunctions * pFun = QOpenGLContext::currentContext()->functions();
	Q_ASSERT(pFun != NULL);

	// 设置背景颜色
	pFun->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// 2.创建顶点着色器
	m_pVertexShader = new QOpenGLShader(QOpenGLShader::Vertex, this);
	static const char *pVertexSrc =
		"attribute highp vec4 posAttr;\n"
		"attribute lowp vec4 colAttr;\n"
		"uniform mediump mat4 matrix;\n"
		"varying lowp  vec4 col;\n"
		"void main(){\n"
		"  col = colAttr;\n"
		"  gl_Position = matrix * posAttr;\n"
		"}\n"
		;

	// 3.编译
	bool bRet = m_pVertexShader->compileSourceCode(pVertexSrc);
	QString strErr = m_pVertexShader->log();
	Q_ASSERT(bRet);

	// 4.创建片段着色器
	m_pFrameShader = new QOpenGLShader(QOpenGLShader::Fragment, this);
	static const char *pFrameSrc =
		"varying lowp  vec4 col;\n"
		"void main()\n"
		"{\n"
		"gl_FragColor = col;\n"
		"}\n";
	bRet = m_pFrameShader->compileSourceCode(pFrameSrc);
	strErr = m_pVertexShader->log();
	Q_ASSERT(bRet);


	// 链接
	bRet = m_pProgramShader->addShader(m_pVertexShader);
	Q_ASSERT(bRet);
	bRet = m_pProgramShader->addShader(m_pFrameShader);
	Q_ASSERT(bRet);
	bRet = m_pProgramShader->link();
	Q_ASSERT(bRet);

	// 创建顶点数据
	m_pBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	m_pBuffer->create();
	m_pBuffer->bind();

	// 绑定数据
	_initPressureAndColor(*m_pBuffer);

	m_pBuffer->setUsagePattern(QOpenGLBuffer::DynamicDraw);
	m_pBuffer->release();

	m_nVertexAttr = m_pProgramShader->attributeLocation("posAttr");
	m_nFragAttr = m_pProgramShader->attributeLocation("colAttr");
	m_nUniform = m_pProgramShader->uniformLocation("matrix");

	setUpdateBehavior(QOpenGLWidget::NoPartialUpdate);
}

// 重写重绘
void QDrawWaveWidget::resizeGL(int nWidth, int nHeight)
{
	QOpenGLFunctions * pFun = QOpenGLContext::currentContext()->functions();
	Q_ASSERT(pFun != NULL);
	QFontMetrics metrics(font(), this);

	// 显示数字的字体
	QFont numFont(QString::fromLocal8Bit("宋体"), 30);
	numFont.setBold(true);
	QFontMetrics numMetrics(numFont, this);

	// 显示单位的字体
	QFont unitFont(QString::fromLocal8Bit("宋体"), 10);
	unitFont.setBold(true);
	QFontMetrics unitMetrics(unitFont, this);
	// 右上臂压力文字的宽度、高度
	float fTipsWidth = metrics.width(QString::fromLocal8Bit("右上臂压力")) + 10;
	float fTipsHeight = metrics.height();

	// 右上臂压力值的宽度、高度
	float fNumWidth = numMetrics.width(QString::fromLocal8Bit("100 "));
	float fNumHeight = numMetrics.height();

	// 右上臂压力值单位的宽度、高度
	float fUnitWidth  = unitMetrics.width(QString::fromLocal8Bit("mmHg"));
	float fUnitHeight = unitMetrics.height();

	float fLeftPos = qMax(fTipsWidth, fNumWidth);

	const qreal retinaScale = devicePixelRatio();

	Q_ASSERT(pFun != NULL);
	pFun->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float fSize = metrics.width(QString::fromLocal8Bit("右上臂 "))*1.0f;
	pFun->glViewport(fSize, 0, width() * retinaScale - fLeftPos - fSize - 10, height() * retinaScale);
	//pFun->glViewport(0, 0, width() * retinaScale, height() * retinaScale);
	pFun->glClearColor(0.0f,0.0f,0.0f,1.0f);
	//pFun->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//pFun->glEnable (GL_POLYGON_SMOOTH_HINT);
	//   pFun->glHint (GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	// 线宽
	pFun->glLineWidth(3.0f);
	// 点大小
	glPointSize(5.0f);

	// 反锯齿
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH, GL_NICEST);
}

//void QDrawWaveWidget::paintGL()
//{
//	qDebug()<<"paintGL";
//	QOpenGLFunctions * pFun = QOpenGLContext::currentContext()->functions();
//	Q_ASSERT(pFun != NULL);
//	pFun->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	static int fRed = 0;
//	const qreal retinaScale = devicePixelRatio();
//
//	pFun->glClearColor(0.0f,0.0f,0.0f,1.0f);
//	double d = width() * retinaScale;
//	d = height() * retinaScale;
//	pFun->glViewport(0, 0, width() * retinaScale, height() * retinaScale);
//	
//
//	//pFun->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	//pFun->glEnable (GL_POLYGON_SMOOTH_HINT);
// //   pFun->glHint (GL_POLYGON_SMOOTH_HINT, GL_NICEST);
//	// 线宽
//	pFun->glLineWidth(4.0f);
//	// 点大小
//	glPointSize(4.0f);
//    
//	// 反锯齿
//	glEnable (GL_LINE_SMOOTH);
//    glEnable (GL_BLEND);
//    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glHint (GL_LINE_SMOOTH, GL_NICEST);
//
//	m_pProgramShader->bind();
//
//	QMatrix4x4 project;
//	////project.frustum(0.0f, width() * retinaScale, 0.0f, height() * retinaScale, 0.1f, 10000.0f);
//	project.perspective(170.0f, 16.0f/9.0f, 0.1f, 100000.0f);
//
//	QMatrix4x4 view;
//	view.lookAt(QVector3D(0, 0, 100.0f), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
//
//	QMatrix4x4 model;
//	model.setToIdentity();
//	//model.scale(2.0f, 1.0f); 
//	model.scale(1.0/2.0f, 1.0/2.0f, 0);
//	model.translate(-100000.0f*4/100, -100000.0f*4*9.0f/100/16, 0);
//
//	fRed += 5;
//	if(fRed> 10000)
//	{
//		fRed = 0;
//	}
//
//	m_pProgramShader->setUniformValue(m_nUniform, project*view*model);
//
//	m_pProgramShader->enableAttributeArray(m_nVertexAttr);
//	m_pProgramShader->enableAttributeArray(m_nFragAttr);
//
//	m_pBuffer->bind();
//	m_pProgramShader->setAttributeBuffer(m_nVertexAttr, GL_FLOAT, 0, 2);
//	m_pProgramShader->setAttributeBuffer(m_nFragAttr, GL_FLOAT, sizeof(float)* 10000 * 2 * 4, 3);
//	m_pBuffer->release();
//
//	pFun->glDrawArrays(GL_LINE_STRIP, 0, 40000);
//
//
//
//	m_pProgramShader->disableAttributeArray(m_nVertexAttr);
//	m_pProgramShader->disableAttributeArray(m_nFragAttr);
//
//	m_pProgramShader->release();
//
//
//	//pFun->glViewport(0, 0, width() * retinaScale, height() * retinaScale);
//	//
//
//	////pFun->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	////pFun->glEnable (GL_POLYGON_SMOOTH_HINT);
// ////   pFun->glHint (GL_POLYGON_SMOOTH_HINT, GL_NICEST);
//	//// 线宽
//	//pFun->glLineWidth(4.0f);
//	//// 点大小
//	//glPointSize(4.0f);
// //   
//	//// 反锯齿
//	//glEnable (GL_LINE_SMOOTH);
// //   glEnable (GL_BLEND);
// //   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
// //   glHint (GL_LINE_SMOOTH, GL_NICEST);
//
//	//m_pProgramShader->bind();
//
//	////QMatrix4x4 project;
//	////////project.frustum(0.0f, width() * retinaScale, 0.0f, height() * retinaScale, 0.1f, 10000.0f);
//	////project.perspective(170.0f, 16.0f/9.0f, 0.1f, 100000.0f);
//
//	////QMatrix4x4 view;
//	////view.lookAt(QVector3D(0.0f, 0, 100.0f), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
//
//	////QMatrix4x4 model;
//	////model.setToIdentity();
//	//////model.scale(2.0f, 1.0f); 
//	////model.scale(1.0/4.0f, 1.0/2.0f, 0);
//	//////model.translate(-fRed, 0, 0);
//
//	//fRed += 5;
//	//if(fRed> 1000)
//	//{
//	//	fRed = 0;
//	//}
//
//	//m_pProgramShader->setUniformValue(m_nUniform, project*view*model);
//
//	//m_pProgramShader->enableAttributeArray(m_nVertexAttr);
//	//m_pProgramShader->enableAttributeArray(m_nFragAttr);
//
//	//m_pBuffer->bind();
//	//m_pProgramShader->setAttributeBuffer(m_nVertexAttr, GL_FLOAT, 0, 2);
//	//m_pProgramShader->setAttributeBuffer(m_nFragAttr, GL_FLOAT, sizeof(float)* 10000 * 2 * 4, 3);
//	//m_pBuffer->release();
//
//	//pFun->glDrawArrays(GL_LINE_STRIP, 0, 40000);
//
//
//
//	//m_pProgramShader->disableAttributeArray(m_nVertexAttr);
//	//m_pProgramShader->disableAttributeArray(m_nFragAttr);
//
//	//m_pProgramShader->release();
//
//
//	//pFun->glViewport(-width() * retinaScale/2, -height() * retinaScale/2.0f, width() * retinaScale, height() * retinaScale);
//	//
//
//	////pFun->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	////pFun->glEnable (GL_POLYGON_SMOOTH_HINT);
// ////   pFun->glHint (GL_POLYGON_SMOOTH_HINT, GL_NICEST);
//	//// 线宽
//	//pFun->glLineWidth(4.0f);
//	//// 点大小
//	//glPointSize(4.0f);
// //   
//	//// 反锯齿
//	//glEnable (GL_LINE_SMOOTH);
// //   glEnable (GL_BLEND);
// //   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
// //   glHint (GL_LINE_SMOOTH, GL_NICEST);
//
//	//m_pProgramShader->bind();
//
//	////QMatrix4x4 project;
//	////////project.frustum(0.0f, width() * retinaScale, 0.0f, height() * retinaScale, 0.1f, 10000.0f);
//	////project.perspective(170.0f, 16.0f/9.0f, 0.1f, 100000.0f);
//
//	////QMatrix4x4 view;
//	////view.lookAt(QVector3D(0.0f, 0, 100.0f), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
//
//	////QMatrix4x4 model;
//	////model.setToIdentity();
//	//////model.scale(2.0f, 1.0f); 
//	////model.scale(1.0/4.0f, 1.0/2.0f, 0);
//	//////model.translate(-fRed, 0, 0);
//
//	//fRed += 5;
//	//if(fRed> 1000)
//	//{
//	//	fRed = 0;
//	//}
//
//	//m_pProgramShader->setUniformValue(m_nUniform, project*view*model);
//
//	//m_pProgramShader->enableAttributeArray(m_nVertexAttr);
//	//m_pProgramShader->enableAttributeArray(m_nFragAttr);
//
//	//m_pBuffer->bind();
//	//m_pProgramShader->setAttributeBuffer(m_nVertexAttr, GL_FLOAT, 0, 2);
//	//m_pProgramShader->setAttributeBuffer(m_nFragAttr, GL_FLOAT, sizeof(float)* 10000 * 2 * 4, 3);
//	//m_pBuffer->release();
//
//	//pFun->glDrawArrays(GL_LINE_STRIP, 0, 40000);
//
//
//
//	//m_pProgramShader->disableAttributeArray(m_nVertexAttr);
//	//m_pProgramShader->disableAttributeArray(m_nFragAttr);
//
//	//m_pProgramShader->release();
//
//
//	//pFun->glViewport(0, -height() * retinaScale/2.0f, width() * retinaScale, height() * retinaScale);
//	//
//
//	////pFun->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	////pFun->glEnable (GL_POLYGON_SMOOTH_HINT);
// ////   pFun->glHint (GL_POLYGON_SMOOTH_HINT, GL_NICEST);
//	//// 线宽
//	//pFun->glLineWidth(4.0f);
//	//// 点大小
//	//glPointSize(4.0f);
// //   
//	//// 反锯齿
//	//glEnable (GL_LINE_SMOOTH);
// //   glEnable (GL_BLEND);
// //   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
// //   glHint (GL_LINE_SMOOTH, GL_NICEST);
//
//	//m_pProgramShader->bind();
//
//	////QMatrix4x4 project;
//	////////project.frustum(0.0f, width() * retinaScale, 0.0f, height() * retinaScale, 0.1f, 10000.0f);
//	////project.perspective(170.0f, 16.0f/9.0f, 0.1f, 100000.0f);
//
//	////QMatrix4x4 view;
//	////view.lookAt(QVector3D(0.0f, 0, 100.0f), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
//
//	////QMatrix4x4 model;
//	////model.setToIdentity();
//	//////model.scale(2.0f, 1.0f); 
//	////model.scale(1.0/4.0f, 1.0/2.0f, 0);
//	//////model.translate(-fRed, 0, 0);
//
//	//fRed += 5;
//	//if(fRed> 1000)
//	//{
//	//	fRed = 0;
//	//}
//
//	//m_pProgramShader->setUniformValue(m_nUniform, project*view*model);
//
//	//m_pProgramShader->enableAttributeArray(m_nVertexAttr);
//	//m_pProgramShader->enableAttributeArray(m_nFragAttr);
//
//	//m_pBuffer->bind();
//	//m_pProgramShader->setAttributeBuffer(m_nVertexAttr, GL_FLOAT, 0, 2);
//	//m_pProgramShader->setAttributeBuffer(m_nFragAttr, GL_FLOAT, sizeof(float)* 10000 * 2 * 4, 3);
//	//m_pBuffer->release();
//
//	//pFun->glDrawArrays(GL_LINE_STRIP, 0, 40000);
//
//
//
//	//m_pProgramShader->disableAttributeArray(m_nVertexAttr);
//	//m_pProgramShader->disableAttributeArray(m_nFragAttr);
//
//	//m_pProgramShader->release();
//}

// 重写描绘操作
void QDrawWaveWidget::paintGL()
{
	QFontMetrics metrics(font(), this);
	// 显示数字的字体
	QFont numFont(QString::fromLocal8Bit("宋体"), 30);
	numFont.setBold(true);
	QFontMetrics numMetrics(numFont, this);

	// 显示单位的字体
	QFont unitFont(QString::fromLocal8Bit("宋体"), 10);
	unitFont.setBold(true);
	QFontMetrics unitMetrics(unitFont, this);
	// 右上臂压力文字的宽度、高度
	float fTipsWidth = metrics.width(QString::fromLocal8Bit("右上臂压力")) + 10;
	float fTipsHeight = metrics.height();

	// 右上臂压力值的宽度、高度
	float fNumWidth = numMetrics.width(QString::fromLocal8Bit("100 "));
	float fNumHeight = numMetrics.height();

	// 右上臂压力值单位的宽度、高度
	float fUnitWidth  = unitMetrics.width(QString::fromLocal8Bit("mmHg"));
	float fUnitHeight = unitMetrics.height();

	float fLeftPos = qMax(fTipsWidth, fNumWidth);

	const qreal retinaScale = devicePixelRatio();

	QOpenGLFunctions * pFun = QOpenGLContext::currentContext()->functions();
	Q_ASSERT(pFun != NULL);
	pFun->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float fSize = metrics.width(QString::fromLocal8Bit("右上臂 "))*1.0f;
	pFun->glViewport(fSize, 0, width() * retinaScale - fLeftPos - fSize - 10, height() * retinaScale);

	m_pProgramShader->bind();
	int nBLACK_WIDTH = (20.0f/(width() * retinaScale - fLeftPos - fSize - 10) * retinaScale)*nPOINT_COUNT;
	//QMatrix4x4 project;
	////project.frustum(0.0f, width() * retinaScale, 0.0f, height() * retinaScale, 0.1f, 10000.0f);
	//project.perspective(170.0f, 4.0f/3.0f, 0.1f, 1000.0f);

	//QMatrix4x4 view;
	//view.lookAt(QVector3D(0, 0, 1000.0f), QVector3D(0, 0, 0.0f), QVector3D(0, 1, 0));

	//QMutexLocker lock(&mutexLastDrawIndex);
	//qDebug()<<"paintGL:  "<<timer.elapsed()<<"\t"<<m_nLastDrawIndex;
	//static int nLastIndex = m_nLastDrawIndex;
	//lock.unlock();
	_updatePlusWave();
	
	//qDebug()<<"paintGL:  "<<timer.elapsed()<<"\t"<<m_nLastDrawIndex;
	//qDebug()<<"timer<<"m_nLastDrawIndex" <<m_nLastDrawIndex;
	float fXScale = nPOINT_COUNT;
	float fYScale = 2.0f/nMAX_PRESSURE;

	QMatrix4x4 model;
	model.setToIdentity();
	model.scale(1.0f/fXScale, fYScale, 0);
	model.translate(-fXScale, nMAX_PRESSURE/4, 0);

	m_pProgramShader->setUniformValue(m_nUniform, model);
	m_pProgramShader->enableAttributeArray(m_nVertexAttr);
	m_pProgramShader->enableAttributeArray(m_nFragAttr);

	m_pBuffer->bind();
	m_pProgramShader->setAttributeBuffer(m_nVertexAttr, GL_FLOAT, 0, 2);
	m_pProgramShader->setAttributeBuffer(m_nFragAttr, GL_FLOAT, sizeof(float)* nPOINT_COUNT * 2 * 4, 3);
	m_pBuffer->release();

	pFun->glDrawArrays(GL_LINE_STRIP, 0, m_nLastDrawIndex);
	
	if(m_bIsFull && m_nLastDrawIndex + nBLACK_WIDTH < nPOINT_COUNT)
	{
		pFun->glDrawArrays(GL_LINE_STRIP, m_nLastDrawIndex+nBLACK_WIDTH, nPOINT_COUNT-nBLACK_WIDTH-m_nLastDrawIndex);
	}
	//qDebug()<<"00  Index"<<m_nLastDrawIndex <<"\t"<<m_nLastDrawIndex+nBLACK_WIDTH<<"\t"<<nPOINT_COUNT-nBLACK_WIDTH-m_nLastDrawIndex;
	//return;

	model.setToIdentity();
	model.scale(1.0f/fXScale, fYScale, 0);
	model.translate(-fXScale, 0.0f, 0);
	m_pProgramShader->setUniformValue(m_nUniform, model);

	pFun->glDrawArrays(GL_LINE_STRIP, nPOINT_COUNT, m_nLastDrawIndex);
	if(m_bIsFull && m_nLastDrawIndex + nBLACK_WIDTH < nPOINT_COUNT)
	{
		pFun->glDrawArrays(GL_LINE_STRIP, m_nLastDrawIndex+nPOINT_COUNT+nBLACK_WIDTH, nPOINT_COUNT-nBLACK_WIDTH-m_nLastDrawIndex);
	}
	//qDebug()<<"01  Index"<<m_nLastDrawIndex <<"\t"<<m_nLastDrawIndex+nPOINT_COUNT+nBLACK_WIDTH<<"\t"<<nPOINT_COUNT-nBLACK_WIDTH-m_nLastDrawIndex;

	model.setToIdentity();
	model.scale(1.0f/fXScale, fYScale, 0);
	model.translate(-fXScale, -nMAX_PRESSURE/4.0f, 0);
	m_pProgramShader->setUniformValue(m_nUniform, model);

	pFun->glDrawArrays(GL_LINE_STRIP, nPOINT_COUNT*2, m_nLastDrawIndex);
	if(m_bIsFull && m_nLastDrawIndex + nBLACK_WIDTH < nPOINT_COUNT)
	{
		pFun->glDrawArrays(GL_LINE_STRIP, m_nLastDrawIndex+nPOINT_COUNT*2+nBLACK_WIDTH, nPOINT_COUNT-nBLACK_WIDTH-m_nLastDrawIndex);
	}
	//qDebug()<<"02  Index"<<m_nLastDrawIndex <<"\t"<<m_nLastDrawIndex+nPOINT_COUNT*2+nBLACK_WIDTH<<"\t"<<nPOINT_COUNT-nBLACK_WIDTH-m_nLastDrawIndex;

	model.setToIdentity();
	model.scale(1.0f/fXScale, fYScale, 0);
	model.translate(-fXScale, -nMAX_PRESSURE/2, 0);
	m_pProgramShader->setUniformValue(m_nUniform, model);

	pFun->glDrawArrays(GL_LINE_STRIP, nPOINT_COUNT*3, m_nLastDrawIndex);
	if(m_bIsFull && m_nLastDrawIndex + nBLACK_WIDTH < nPOINT_COUNT)
	{
		pFun->glDrawArrays(GL_LINE_STRIP, m_nLastDrawIndex+nPOINT_COUNT*3+nBLACK_WIDTH, nPOINT_COUNT-nBLACK_WIDTH-m_nLastDrawIndex);
	}
	//qDebug()<<"03 Index"<<m_nLastDrawIndex <<"\t"<<m_nLastDrawIndex+nPOINT_COUNT*3+nBLACK_WIDTH<<"\t"<<nPOINT_COUNT-nBLACK_WIDTH-m_nLastDrawIndex;

	m_pProgramShader->disableAttributeArray(m_nVertexAttr);
	m_pProgramShader->disableAttributeArray(m_nFragAttr);
	m_pProgramShader->release();

	QPainter painter(this);
	painter.beginNativePainting();
	QPen pen;
	pen.setColor(Qt::red);
	painter.setPen(pen);
	painter.setFont(font());

	// 右上臂波形名称
	painter.drawText(0, height() * retinaScale/4.0f-metrics.height()/2.0f, metrics.width(QString::fromLocal8Bit("右上臂 ")),	metrics.height(), Qt::AlignJustify, QString::fromLocal8Bit("右上臂"));

	// 右上臂压力值
	{
		pen.setWidthF(1.0f);
		pen.setColor(Qt::white);
		painter.setPen(pen);
		painter.drawText(width() - fLeftPos, height() * retinaScale/4.0f - fTipsHeight - fNumHeight - fUnitHeight,
			fTipsWidth, fTipsHeight, Qt::AlignJustify, QString::fromLocal8Bit("右上臂压力"));
		painter.drawRect(width() - fLeftPos, height() * retinaScale/4.0f- fUnitHeight - fNumHeight, fLeftPos, fNumHeight);

		painter.setFont(numFont);
		painter.drawText(width()- fLeftPos, height() * retinaScale/4.0f - fUnitHeight - fNumHeight, fLeftPos, fNumHeight,
			Qt::AlignRight |Qt::AlignVCenter, QString::number(m_nRightArmPressure));

		painter.setFont(unitFont);
		painter.drawText(width()- fLeftPos, height() * retinaScale/4.0f - fUnitHeight, fLeftPos, fUnitHeight,
			Qt::AlignRight | Qt::AlignBottom, QString::fromLocal8Bit("mmHg"));
	}

	// 左上臂波形名称
	{
		pen.setColor(Qt::green);
		painter.setPen(pen);
		painter.setFont(font());
		painter.drawText(0, height() * retinaScale/2.0f-metrics.height()/2.0f, metrics.width(QString::fromLocal8Bit("左上臂 ")),	metrics.height(), Qt::AlignJustify, QString::fromLocal8Bit("左上臂 "));
	}

	// 左上臂压力值
	{
		pen.setColor(Qt::white);
		painter.setPen(pen);
		painter.drawText(width() - fLeftPos, height() * retinaScale/2.0f - fTipsHeight - fNumHeight - fUnitHeight,
			fTipsWidth, fTipsHeight, Qt::AlignJustify, QString::fromLocal8Bit("左上臂压力"));
		painter.drawRect(width() - fLeftPos, height() * retinaScale/2.0f- fUnitHeight - fNumHeight, fLeftPos, fNumHeight);

		painter.setFont(numFont);
		painter.drawText(width()- fLeftPos, height() * retinaScale/2.0f - fUnitHeight - fNumHeight, fLeftPos, fNumHeight,
			Qt::AlignRight |Qt::AlignVCenter, QString::number(m_nLeftArmPressure));

		painter.setFont(unitFont);
		painter.drawText(width()- fLeftPos, height() * retinaScale/2.0f - fUnitHeight, fLeftPos, fUnitHeight,
			Qt::AlignRight | Qt::AlignBottom, QString::fromLocal8Bit("mmHg"));
	}

	// 右踝臂波形名称
	{
		pen.setColor(Qt::blue);
		painter.setPen(pen);
		painter.setFont(font());
		painter.drawText(0, height() * retinaScale*3/4.0f-metrics.height()/2.0f, metrics.width(QString::fromLocal8Bit("右踝臂 ")),	metrics.height(), Qt::AlignJustify, QString::fromLocal8Bit("右踝臂 "));
	}

	// 右踝臂压力值
	{
		pen.setColor(Qt::white);
		painter.setPen(pen);
		painter.drawText(width() - fLeftPos, height() * 3.0f *retinaScale/4.0f - fTipsHeight - fNumHeight - fUnitHeight,
			fTipsWidth, fTipsHeight, Qt::AlignJustify, QString::fromLocal8Bit("右踝臂压力"));
		painter.drawRect(width() - fLeftPos, height()* 3.0f * retinaScale/4.0f- fUnitHeight - fNumHeight, fLeftPos, fNumHeight);

		painter.setFont(numFont);
		painter.drawText(width()- fLeftPos, height() * 3.0f * retinaScale/4.0f - fUnitHeight - fNumHeight, fLeftPos, fNumHeight,
			Qt::AlignRight |Qt::AlignVCenter, QString::number(m_nRightAnklePressure));

		painter.setFont(unitFont);
		painter.drawText(width()- fLeftPos, height() * 3.0f * retinaScale/4.0f - fUnitHeight, fLeftPos, fUnitHeight,
			Qt::AlignRight | Qt::AlignBottom, QString::fromLocal8Bit("mmHg"));
	}

	// 左踝臂波形名称
	{
		pen.setColor(Qt::yellow);
		painter.setPen(pen);
		painter.setFont(font());
		painter.drawText(0, height() * retinaScale-metrics.height(), metrics.width(QString::fromLocal8Bit("左踝臂 ")),	metrics.height(), Qt::AlignJustify, QString::fromLocal8Bit("左踝臂 "));
	}

	// 左踝臂压力值
	{
		pen.setColor(Qt::white);
		painter.setPen(pen);
		painter.drawText(width() - fLeftPos, height() * retinaScale - fTipsHeight - fNumHeight - fUnitHeight,
			fTipsWidth, fTipsHeight, Qt::AlignJustify, QString::fromLocal8Bit("左踝臂压力"));
		painter.drawRect(width() - fLeftPos, height()* retinaScale- fUnitHeight - fNumHeight, fLeftPos, fNumHeight);

		painter.setFont(numFont);
		painter.drawText(width()- fLeftPos, height() *  retinaScale- fUnitHeight - fNumHeight, fLeftPos, fNumHeight,
			Qt::AlignRight |Qt::AlignVCenter, QString::number(m_nLeftAnklePressure));

		painter.setFont(unitFont);
		painter.drawText(width()- fLeftPos, height() * retinaScale - fUnitHeight, fLeftPos, fUnitHeight,
			Qt::AlignRight | Qt::AlignBottom, QString::fromLocal8Bit("mmHg"));
	}

	painter.endNativePainting();
	//pFun->glFlush();
}

//void QDrawWaveWidget::paintGL()
//{
//	QOpenGLFunctions * pFun = QOpenGLContext::currentContext()->functions();
//	Q_ASSERT(pFun != NULL);
//	pFun->glClearColor(0.0f,0.0f,0.0f,1.0f);
//	pFun->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_POINT_BIT);
//	static int fRed = 0;
//	const qreal retinaScale = devicePixelRatio();
//
//	pFun->glViewport(0.0f, 0.0f, width() * retinaScale, height() * retinaScale);
//
//	//pFun->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_POINT_BIT);
//	//pFun->glClearColor(0.0f,0.0f,0.0f,1.0f);
//	//pFun->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	//pFun->glEnable (GL_POLYGON_SMOOTH_HINT);
//	//pFun->glHint (GL_POLYGON_SMOOTH_HINT, GL_NICEST);
//	// 线宽
//	pFun->glLineWidth(4.0f);
//	// 点大小
//	glPointSize(4.0f);
//
//	// 反锯齿
//	pFun->glEnable (GL_POLYGON_SMOOTH);
//	pFun->glEnable (GL_BLEND);
//	pFun->glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	pFun->glHint (GL_POLYGON_SMOOTH, GL_NICEST);
//
//	m_pProgramShader->bind();
//
//	QMatrix4x4 project;
//	project.ortho(0, width() * retinaScale, 0, height() * retinaScale, 0.1f, 10000.0f);
//
//	QMatrix4x4 view;
//	view.lookAt(QVector3D(0.0f, 0.0f, 10.0f), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
//
//	QMatrix4x4 model;
//	model.setToIdentity();
//	float n = width() * retinaScale/10000;
//	model.scale(width() * retinaScale/10000, height()* 2 * retinaScale/m_readFile->getMaxValue(), 0);
//	//model.rotate(fRed, 1, 0, 0);
//	n  = height() * retinaScale* 800 /m_readFile->getMaxValue();
//	model.translate(200, 200, 0);
//
//	fRed += 50;
//	if(fRed > 10000)
//	{
//		fRed = 0;
//	}
//
//	m_pProgramShader->setUniformValue(m_nUniform, project*view*model);
//
//	m_pProgramShader->enableAttributeArray(m_nVertexAttr);
//	m_pProgramShader->enableAttributeArray(m_nFragAttr);
//
//	m_pBuffer->bind();
//	m_pProgramShader->setAttributeBuffer(m_nVertexAttr, GL_FLOAT, 0, 2);
//	m_pProgramShader->setAttributeBuffer(m_nFragAttr, GL_FLOAT, sizeof(float) * 10000*2*4, 3);
//	m_pBuffer->release();
//	
//
//	if(fRed>200)
//	{
//		pFun->glDrawArrays(GL_LINE_LOOP, 0, fRed-200);
//		pFun->glDrawArrays(GL_LINE_LOOP, 10000, fRed-200);
//		pFun->glDrawArrays(GL_LINE_LOOP, 20000, fRed-200);
//		pFun->glDrawArrays(GL_LINE_LOOP, 30000, fRed-200);
//	}
//	//pFun->glDrawArrays(GL_LINE_LOOP, fRed, 40000-fRed);
//	pFun->glDrawArrays(GL_LINE_LOOP, fRed, 10000 - fRed);
//	pFun->glDrawArrays(GL_LINE_LOOP, 10000 + fRed, 10000 - fRed);
//	pFun->glDrawArrays(GL_LINE_LOOP, 20000 + fRed, 10000 - fRed);
//	pFun->glDrawArrays(GL_LINE_LOOP, 30000 + fRed, 10000 - fRed);
//	
//	m_pProgramShader->disableAttributeArray(m_nVertexAttr);
//	m_pProgramShader->disableAttributeArray(m_nFragAttr);
//
//	m_pProgramShader->release();
//
//	qDebug()<<timer.elapsed();
//	timer.start();
//}

//void QDrawWaveWidget::paintGL()
//{
//	QOpenGLFunctions * pFun = QOpenGLContext::currentContext()->functions();
//	Q_ASSERT(pFun != NULL);
//	pFun->glClearColor(0.0f,0.0f,0.0f,1.0f);
//	pFun->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	static int fRed = 0;
//	const qreal retinaScale = devicePixelRatio();
//
//	pFun->glViewport(0.0f, 0.0f, width() * retinaScale, height() * retinaScale);
//
//	//pFun->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_POINT_BIT);
//	//pFun->glClearColor(0.0f,0.0f,0.0f,1.0f);
//	//pFun->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	//pFun->glEnable (GL_POLYGON_SMOOTH_HINT);
//	//pFun->glHint (GL_POLYGON_SMOOTH_HINT, GL_NICEST);
//	// 线宽
//	pFun->glLineWidth(4.0f);
//	// 点大小
//	glPointSize(4.0f);
//
//	// 反锯齿
//	pFun->glEnable(GL_POLYGON_SMOOTH);
//	pFun->glEnable(GL_BLEND);
//	pFun->glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	pFun->glHint (GL_POLYGON_SMOOTH, GL_NICEST);
//
//	QMatrix4x4 project;
//	project.ortho(0, width() * retinaScale, 0, height() * retinaScale, 0.1f, 10000.0f);
//
//	QMatrix4x4 view;
//	view.lookAt(QVector3D(0.0f, 0.0f, 10.0f), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
//
//	QMatrix4x4 model;
//	model.setToIdentity();
//	float n = width() * retinaScale/10000;
//	model.scale(width() * retinaScale/10000, height()* 2 * retinaScale/m_readFile->getMaxValue(), 0);
//	//model.rotate(fRed, 1, 0, 0);
//	n  = height() * retinaScale* 800 /m_readFile->getMaxValue();
//	model.translate(200, 0, 0);
//
//	float *pIndex[] = {m_readFile->getLeftArm(), m_readFile->getRightArm(), m_readFile->getLeftAnkle(), m_readFile->getRightAnkle()};
//	for(int i = 0 ; i < 4; ++i)
//	{
//		m_pProgramShader->bind();
//		m_pProgramShader->setUniformValue(m_nUniform, project*view*model);
//
//		model.setToIdentity();
//		model.scale(width() * retinaScale/10000, height()* 2 * retinaScale/m_readFile->getMaxValue(), 0);
//		model.translate(200, height() * retinaScale *  (i+1)*5 / 4, 0);
//
//		m_pProgramShader->enableAttributeArray(m_nVertexAttr);
//		m_pProgramShader->enableAttributeArray(m_nFragAttr);
//
//		m_pBuffer->bind();
//		m_pProgramShader->setAttributeBuffer(m_nVertexAttr, GL_FLOAT, 0, 2);
//		m_pProgramShader->setAttributeBuffer(m_nFragAttr, GL_FLOAT, sizeof(float) * 10000*2*4, 3);
//		m_pBuffer->release();
//
//
//		//if(fRed>200)
//		//{
//		//	pFun->glDrawArrays(GL_LINE_LOOP, i*fRed, fRed-200);
//		//}
//		//pFun->glDrawArrays(GL_LINE_LOOP, fRed, 10000-fRed);
//		pFun->glDrawArrays(GL_LINE_LOOP, 10000*i, 10000);
//
//		m_pProgramShader->disableAttributeArray(m_nVertexAttr);
//		m_pProgramShader->disableAttributeArray(m_nFragAttr);
//
//
//		m_pProgramShader->release();
//		QMatrix4x4 modelIdentified;
//		modelIdentified.setToIdentity();
//		m_pProgramShader->setUniformValue(m_nUniform, modelIdentified);
//	}
//
//	fRed += 50;
//	if(fRed > 10000)
//	{
//		fRed = 0;
//	}
//
//	//QMetaObject::invokeMethod(this, "update");
//	qDebug()<<timer.elapsed();
//	timer.start();
//}

void QDrawWaveWidget::slotSavePicture()
{
	//QImage image = grabFramebuffer();
	//image.save("AA.jpeg","JPEG");
	static bool bret = true;
	if(timer.hasExpired(1000) && bret && QOpenGLContext::currentContext() != NULL)
	{
		QOpenGLFunctions * pFun = QOpenGLContext::currentContext()->functions();
		Q_ASSERT(pFun != NULL);
		QSize size = this->size();
		QImage img = QImage(size, QImage::Format_ARGB32_Premultiplied);
		pFun->glReadPixels(0, 0, this->width(), this->height(), GL_RGBA, GL_UNSIGNED_BYTE, img.bits());
		img = img.rgbSwapped();
		img = img.mirrored();
		img.save("file.jpg");
		bret = false;
	}
}

void QDrawWaveWidget::stopMeasure()
{
	timer.invalidate();
	memset(m_bHasMax, 0x00, sizeof(m_bHasMax));
}

void QDrawWaveWidget::startMeasure()
{
	m_bIsFull = false;
	m_nLastDrawIndex = 0;
	timer.restart();

	for(int i=0; i <4; ++i)
	{
		m_arrPlusWave[i].clear();
	}

	//if(m_pTimerThread != NULL && !m_pTimerThread->isRunning())
	//{
	//	m_pTimerThread->start();
	//}

	emit sigStartTimer();
}

// 获取波谷的位置
static QVector<int> getSlopeIndex(const QVector<int> &vectPressure)
{
	int nSize = vectPressure.size();

	QVector<int> slopeArr(vectPressure.size()-1, 0);
	for(int i=0; i < vectPressure.size()-1; ++i)
	{
		int nSlope = vectPressure.at(i+1) - vectPressure.at(i);
		if(nSlope>0)
		{
			slopeArr.replace(i, 1);
		}
		else if(nSlope<0)
		{
			slopeArr.replace(i, -1);
		}
	}

	for(int i = slopeArr.size() -1; i >= 0; --i)
	{
		if(slopeArr.at(i) == 0 && i == slopeArr.size() - 1)
		{
			slopeArr.replace(i, 1);
		}
		else if(slopeArr.at(i) == 0 && i != slopeArr.size() - 1)
		{
			if(slopeArr.at(i+1) >= 0)
			{
				slopeArr.replace(i, 1);
			}
			else 
			{
				slopeArr.replace(i, -1);
			}
		}
	}

	QVector<int> arrIndex;
	for(int i = 0; i < slopeArr.size() - 1; ++i)
	{
		if(slopeArr.at(i+1) - slopeArr.at(i) == 2)
		{
			if(arrIndex.isEmpty() || i - arrIndex.last()>55)
			{
				arrIndex.append(i+1);
			}

		}
	}
	return arrIndex;
}

#define NPOLES 6
static float yv[4][NPOLES+1];

static void filterloop(QVector<int> &vectPressure, const int &nIndex, const bool &bReset = false)
{   
	if(bReset)
	{
		memset(yv, 0x00, sizeof(yv));
	}

	int nMaxValue = 0;
	int nMinValue = 0;
	QVector<int>::iterator iter = vectPressure.begin();
	for (;iter != vectPressure.end(); ++iter)
	{
		yv[nIndex][6] = yv[nIndex][5];
		yv[nIndex][5] = yv[nIndex][4];
		yv[nIndex][4] = yv[nIndex][3];
		yv[nIndex][3] = yv[nIndex][2];
		yv[nIndex][2] = yv[nIndex][1];
		yv[nIndex][1] = yv[nIndex][0];
		yv[nIndex][0] = *iter;
		*iter = (yv[nIndex][0] + yv[nIndex][1] + yv[nIndex][2] + yv[nIndex][3] + yv[nIndex][4] + yv[nIndex][5] + yv[nIndex][6])/8;
		nMaxValue = (*iter) > nMaxValue ? (*iter):nMaxValue;
		nMinValue = (*iter) < nMinValue ? (*iter):nMinValue;
	}
}

static QVector<int> dealOriginalData(QVector<int> &vectPressure, QVector<int> &vectIndex, const int &nFilterIndex)
{
	QVector<int> vectPlusWave;
	if(vectIndex.at(0) != 0)
	{
		int nRange = vectPressure.at(vectIndex.at(0)) - vectPressure.at(0);
		int nIndex = 1;
		int nMin = 0;
		int nMax = 0;
		for(int i = 1; i <vectIndex.at(0); ++i)
		{
			int nValue = vectPressure.at(i) - vectPressure.at(0) - nRange * nIndex/(vectIndex.at(0));
			vectPlusWave.append(nValue);
			nIndex++;
		}
	}

	{
		int nIndexSize = vectIndex.size();
		for(int i = 0; i < vectIndex.size() - 1; ++i)
		{
			int nRange =  vectPressure.at(vectIndex.at(i+1)) - vectPressure.at(vectIndex.at(i));
			int nPlusSize = vectIndex.at(i+1) - vectIndex.at(i);
			int nIndex = 1;
			for(int j = vectIndex.at(i); j <= vectIndex.at(i+1); ++j)
			{
				int nValue = vectPressure.at(j) - vectPressure.at(vectIndex.at(i)) - nRange * nIndex/(nPlusSize);
				vectPlusWave.append(nValue);
				nIndex++;
			}
		}
		vectPressure.remove(0, vectIndex.at(vectIndex.size()-1));
	}

	filterloop(vectPlusWave, nFilterIndex, true);
	return vectPlusWave;
}

void QDrawWaveWidget::appendData(int *pPressureData)
{
	// Timer没有启动的话，直接返回
	if(!timer.isValid() || NULL == pPressureData)
	{
		return;
	}

	// 互斥访问锁
	QMutexLocker lock(&mutexLastDrawIndex);

	// 获取描绘数据存放的内存地址
	m_pBuffer->bind();
	GLfloat *pBegin = (GLfloat *)(m_pBuffer->map(QOpenGLBuffer::WriteOnly));

	// 存储原始压力值,依次存放右臂、左臂、右腿、左腿
	static QVector<int> vectOriginal[4];

	// 存储处理后的压力值,依次存放右臂、左臂、右腿、左腿
	static QVector<int> vectAnalysisPressure[4];

	// 是否处于第一阶段的下降阶段
	static bool bHasLower = false;
	if (pBegin != NULL)
	{
		// 首屏的情况下，清理数据
		if(!m_bIsFull && m_nLastDrawIndex == 0)
		{
			// 清空数据
			for(int i = 0; i<4; ++i)
			{
				vectOriginal[i].clear();
				vectAnalysisPressure[i].clear();
			}
			bHasLower = false;
		}

		if(m_nLastDrawIndex == nPOINT_COUNT)
		{
			if(!m_bIsFull)
			{
				m_bIsFull = true;
			}
			m_nLastDrawIndex = 0;
		}

		// 右臂
		if(pPressureData[0]>1990)
		{
			bHasLower = false;
			m_bHasMax[0] = true;
		}

		// 左臂
		if(pPressureData[1]>1990)
		{
			bHasLower = false;
			m_bHasMax[1] = true;
		}

		// 右踝
		if(pPressureData[2]>2190)
		{
			bHasLower = false;
			m_bHasMax[2] = true;
		}

		// 左踝
		if(pPressureData[3]>2190)
		{
			bHasLower = false;
			m_bHasMax[3] = true;
		}

		// 第一阶段结束的判断条件
		if( pPressureData[0] <=400 && 
			pPressureData[1] <=400 && 
			pPressureData[2] <=400 && 
			pPressureData[3] <=400 && 
			m_bHasMax[0] && 
			m_bHasMax[1] &&
			m_bHasMax[2] &&
			m_bHasMax[3] &&
			!bHasLower)
		{
			bHasLower = true;
			m_bHasMax[0] = false;
			m_bHasMax[1] = false;
			m_bHasMax[2] = false;
			m_bHasMax[3] = false;

			// 清空现存数据
			for(int i = 0; i<4; ++i)
			{
				vectOriginal[i].clear();
				vectAnalysisPressure[i].clear();
			}
		}

		// 维持阶段达到最高点
		if( pPressureData[0] >= 790 && 
			pPressureData[1] >= 790 && 
			pPressureData[2] >= 790 && 
			pPressureData[3] >= 790 && 
			!m_bHasMax[0] && 
			!m_bHasMax[1] &&
			!m_bHasMax[2] &&
			!m_bHasMax[3] &&
			bHasLower)
		{
			m_bHasMax[0] = true;
			m_bHasMax[1] = true;
			m_bHasMax[2] = true;
			m_bHasMax[3] = true;
		}

		if(m_bHasMax[0] && m_bHasMax[1] && m_bHasMax[2] && m_bHasMax[3])
		{
			for(int i = 0; i<4; ++i)
			{
				vectOriginal[i].append(pPressureData[i]);
			}

			bool bRetHasSlope = true;
			QVector<int> vectSlope[4];
			for(int i = 0; i<4;++i)
			{
				if(vectOriginal[i].size()<50)
				{
					bRetHasSlope = false;
					break;
				}
				QVector<int> vectOrgTemp = vectOriginal[i];

				filterloop(vectOrgTemp, i, true);
				QVector<int> vectIndex = getSlopeIndex(vectOrgTemp);
				if(vectIndex.isEmpty() || vectIndex.size() < 2)
				{
					bRetHasSlope = false;
					break;
				}
				else
				{
					vectSlope[i].append(vectIndex);
				}
			}

			if(bRetHasSlope)
			{
				//qDebug()<<"Before"<<vectAnalysisPressure[0].size()<<"\t"<<vectAnalysisPressure[1].size();
				for(int i = 0; i<4;++i)
				{
					vectAnalysisPressure[i].append(dealOriginalData(vectOriginal[i], vectSlope[i], i));
				}
				//qDebug()<<"After"<<vectAnalysisPressure[0].size()<<"\t"<<vectAnalysisPressure[1].size();
			}
		}

		for(int i = 0; i<4; ++i)
		{
			float fRightArm = 0.0f;
			if(!vectAnalysisPressure[i].isEmpty())
			{
				fRightArm = vectAnalysisPressure[i].at(0);
				vectAnalysisPressure[i].pop_front();
			}
			*(pBegin+m_nLastDrawIndex*2+1 +  nPOINT_COUNT*2*i) = fRightArm;
		}
		m_nLastDrawIndex++;
	}

	m_nRightArmPressure = pPressureData[0]/10;
	m_nLeftArmPressure = pPressureData[1]/10;
	m_nRightAnklePressure =  pPressureData[2]/10;
	m_nLeftAnklePressure =  pPressureData[3]/10;

	m_pBuffer->unmap();
	m_pBuffer->release();
}

void QDrawWaveWidget::_initPressureAndColor(QOpenGLBuffer &buffer)
{
	// 初始化颜色数据
	GLfloat *pColArr = new GLfloat[nPOINT_COUNT*3*4];
	memset(pColArr, 0x00, sizeof(GLfloat)*nPOINT_COUNT*3*4);
	GLfloat *fColor = pColArr;
	for(int i = 0; i< 3; ++i)
	{
		for(int j = 0; j < nPOINT_COUNT; ++j)
		{
			*fColor++ = i == 0 ? 1.0f : 0.0f; 
			*fColor++ = i == 1 ? 1.0f : 0.0f; 
			*fColor++ = i == 2 ? 1.0f : 0.0f;
		}
	}

	for(int j = 3*nPOINT_COUNT; j < nPOINT_COUNT*4; ++j)
	{
		*fColor++ = 1.0f; 
		*fColor++ = 1.0f; 
		*fColor++ = 0.0;
	}

	// 初始化X坐标的值
	GLfloat *pXInitValue = new GLfloat[nPOINT_COUNT*2];
	memset(pXInitValue, 0x00, sizeof(GLfloat)*nPOINT_COUNT*2);

	pXInitValue[nPOINT_COUNT*2-1] = nMAX_PRESSURE/10;
	pXInitValue[nPOINT_COUNT*2-3] = nMAX_PRESSURE/10;

	// 初始化X坐标
	for(int i = 0; i < nPOINT_COUNT*2; i += 2)
	{
		pXInitValue[i] = i;
	}

	m_pBuffer->allocate(sizeof(float)* nPOINT_COUNT * 2 * 4 + sizeof(GLfloat) * nPOINT_COUNT * 3 * 4);

	// 绑定数据
	m_pBuffer->write(0, pXInitValue, sizeof(float) * nPOINT_COUNT*2);
	m_pBuffer->write(sizeof(float) * nPOINT_COUNT*2, pXInitValue, sizeof(float) * nPOINT_COUNT*2);
	m_pBuffer->write(sizeof(float) * nPOINT_COUNT*2*2, pXInitValue, sizeof(float) * nPOINT_COUNT*2);
	m_pBuffer->write(sizeof(float) * nPOINT_COUNT*2*3, pXInitValue, sizeof(float) * nPOINT_COUNT*2);

	// 绑定颜色值
	m_pBuffer->write(sizeof(float) * nPOINT_COUNT*2*4, pColArr, sizeof(GLfloat) * nPOINT_COUNT*3*4);

	delete []pColArr;
	pColArr = NULL;

	delete pXInitValue;
	pXInitValue = NULL;
}

// 原始波形数据
inline QVector<int> QDrawWaveWidget::_analysisPlus(QVector<int> &vectPressure, const int &nValueIndex,  const bool &bReset)
{
	QVector<int> vecPlusPressure;

	if(vectPressure.isEmpty())
	{
		return vecPlusPressure;
	}

	int nSize = vectPressure.size();
	bool bHasLower = false;
	QVector<int> slopeArr;
	QVector<int> vecTemp = vectPressure;
	filterloop(vecTemp, nValueIndex, true);
	for(int i = 0; i<nSize-1; ++i)
	{
		slopeArr.append(vecTemp.at(i+1) - vecTemp.at(i));
	}

	// 寻找另一个波谷出现的位置
	QVector<int> arrIndex;
	// 记录当前上升点的个数
	int nCountSlopePositive = 0;
	// 上升点的压力
	int nStartPressure = 0;
	for(int i = 0; i<slopeArr.size(); ++i)
	{
		if(slopeArr.at(i) >= 1)
		{
			if(nCountSlopePositive == 0)
			{
				nStartPressure = vecTemp.at(i);
			}
			nCountSlopePositive++;
		}
		else if(slopeArr.at(i) >= 0 && vecTemp.at(i) - nStartPressure > 0)
		{
			nCountSlopePositive++;
		}
		else if(nCountSlopePositive != 0)
		{
			nCountSlopePositive = 0;
		}

		if(nCountSlopePositive>=4)
		{
			if(arrIndex.isEmpty() || (i - nCountSlopePositive - arrIndex.last() > 55))
			{
				arrIndex.append(i + 1 - nCountSlopePositive);
			}
			nCountSlopePositive = 0;
		}
	}

	if(arrIndex.size() >= 2)
	{
		if(arrIndex.at(0) != 0)
		{
			int nRange = vectPressure.at(arrIndex.at(0)) - vectPressure.at(0);
			int nIndex = 1;
			int nMin = 0;
			int nMax = 0;
			for(int i = 1; i <arrIndex.at(0); ++i)
			{
				int nValue = vectPressure.at(i) - vectPressure.at(0) - nRange * nIndex/(arrIndex.at(0));
				vecPlusPressure.append(nValue);
				if(nValue<nMin)
				{
					nMin = nValue;
				}
				nIndex++;
			}
		}

		{
			int nIndexSize = arrIndex.size();
			for(int i = 0; i < arrIndex.size() - 1; ++i)
			{
				int nRange =  vectPressure.at(arrIndex.at(i+1)) - vectPressure.at(arrIndex.at(i));
				int nPlusSize = arrIndex.at(i+1) - arrIndex.at(i);
				int nIndex = 1;
				int nMin = 0;
				for(int j = arrIndex.at(i); j <= arrIndex.at(i+1); ++j)
				{
					int nValue = vectPressure.at(j) - vectPressure.at(arrIndex.at(i)) - nRange * nIndex/(nPlusSize);
					vecPlusPressure.append(nValue);
					if(nValue<nMin)
					{
						nMin = nValue;
					}
					nIndex++;
				}
			}
			vectPressure.remove(0, arrIndex.at(arrIndex.size()-1));
		}
	}

	filterloop(vecPlusPressure, nValueIndex, true);
	return vecPlusPressure;
}

void QDrawWaveWidget::appendData(QVector<float> *pArrDrawPressure, QVector<int> arrShowPressure, bool bClearData)
{
	// Timer没有启动的话，直接返回
	if(!timer.isValid()/* || arrDrawPressure == NULL */||arrShowPressure.isEmpty() || arrShowPressure.size()<4)
	{
		return;
	}
	
	QMutexLocker lock(&mutexLastDrawIndex);
	//if(m_nLastDrawIndex == nPOINT_COUNT)
	//{
	//	if(!m_bIsFull)
	//	{
	//		m_bIsFull = true;
	//	}
	//	m_nLastDrawIndex = 0;
	//}

	//// 仅仅更新显示压力值
	//if(!bOnlyUpdatePressure)
	//{
	//	m_nLastDrawIndex++;
	//		m_nRightArmPressure = pShowPressure[0]/10;
	//m_nLeftArmPressure = pShowPressure[1]/10;
	//m_nRightAnklePressure =  pShowPressure[2]/10;
	//m_nLeftAnklePressure =  pShowPressure[3]/10;
	//	return;
	//}

	// 获取描绘数据存放的内存地址
	//m_pBuffer->bind();

	//GLfloat *pBegin = (GLfloat *)(m_pBuffer->map(QOpenGLBuffer::WriteOnly));
	if(bClearData)
	{
		//qDebug()<<"Clear Data"<<arrShowPressure.at(0)/10<<" "<<arrShowPressure.at(1)/10<<" "<<arrShowPressure.at(2)/10<<" "<<arrShowPressure.at(3)/10;
		for(int i = 0; i<4; ++i)
		{
			m_arrPlusWave[i].clear();
		}
	}

	if(pArrDrawPressure != NULL)
	{
		//qDebug()<<pArrDrawPressure->size();
		for(int j = 0; j<pArrDrawPressure->size(); ++j)
		{
			//for(int i = 0; i<4; ++i)
			//{
				m_arrPlusWave[j%4].append(pArrDrawPressure->at(j));
			//}
		}
	}

	//m_nLastDrawIndex++;

	//m_pBuffer->unmap();
	//m_pBuffer->release();
	//for(int i = 0; i<4 && NULL != pDrawPressure; ++i)
	//{
	//	m_arrPlusWave[i].append(*(pDrawPressure+i));
	//	//qDebug()<<"Size:"<<i<<"\t"<<m_arrPlusWave[i].size();
	//}
	// 获取描绘数据存放的内存地址
	//m_pBuffer->bind();
	//GLfloat *pBegin = (GLfloat *)(m_pBuffer->map(QOpenGLBuffer::WriteOnly));
	//for(int i = 0; i<4; ++i)
	//{
	//	*(pBegin+m_nLastDrawIndex*2+1 +  nPOINT_COUNT*2*i) = *(pDrawPressure+i);	
	//}

	//m_nLastDrawIndex++;
	m_nRightArmPressure = arrShowPressure.at(0)/10;
	m_nLeftArmPressure = arrShowPressure.at(1)/10;
	m_nRightAnklePressure =  arrShowPressure.at(2)/10;
	m_nLeftAnklePressure = arrShowPressure.at(3)/10;

	//qDebug()<<"appendData:  "<<timer.elapsed()<<"\t"<<m_nLastDrawIndex;
	
	//if(m_nLastDrawIndex%10 == 0)
	//{
	//	update();
	//	//repaint();
	//}
	//_updatePlusWave();
	Q_DELETE(pArrDrawPressure);
}

void QDrawWaveWidget::_updatePlusWave()
{
	// Timer没有启动的话，直接返回
	if(!timer.isValid())
	{
		return;
	}
 
	QMutexLocker lock(&mutexLastDrawIndex);
	if(m_nLastDrawIndex == nPOINT_COUNT)
	{
		if(!m_bIsFull)
		{
			m_bIsFull = true;
		}
		m_nLastDrawIndex = 0;
	}

	// 获取描绘数据存放的内存地址
	m_pBuffer->bind();

	int nDrawSize = 16;
	for(int i = 0; i<4; ++i)
	{
		if(nDrawSize > m_arrPlusWave[i].size())
		{
			nDrawSize = m_arrPlusWave[i].size();
		}
	}

	if(nDrawSize>16 || nDrawSize == 0)
	{
		nDrawSize = 16;
	}

	qDebug()<<nDrawSize;

	GLfloat *pBegin = (GLfloat *)(m_pBuffer->map(QOpenGLBuffer::WriteOnly));
	for(int j = 0; j<16; ++j)
	{
		for(int i = 0; i<4; ++i)
		{
			float fValue = 0.0f;
			if(!m_arrPlusWave[i].isEmpty())
			{
				fValue = m_arrPlusWave[i].at(0);
				m_arrPlusWave[i].pop_front();
			}
			*(pBegin+m_nLastDrawIndex*2+ 1 +  nPOINT_COUNT*2*i) = fValue;
		}
		++m_nLastDrawIndex;
	}
	//m_nLastDrawIndex += 16;

	m_pBuffer->unmap();
	m_pBuffer->release();

	//qDebug()<<"end:"<<timer.elapsed();
	//repaint();
}
