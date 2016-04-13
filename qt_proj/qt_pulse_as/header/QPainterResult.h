#pragma once
#include "qwidget.h"
class QPainterResult :
	public QDialog
{
public:
	
	QPainterResult(tagPersonInfo info,QWidget *pParent = NULL);
	
	~QPainterResult();

protected:

	void paintEvent(QPaintEvent *pEvent);

private:
	// 用户信息及测量结果
	tagPersonInfo m_infoAndValue;
};

