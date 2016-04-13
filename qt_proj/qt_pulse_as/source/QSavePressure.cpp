#include "./header/stdafx.h"
#include <QFile>
#include <QDir>
#include "./header/QSavePressure.h"

QSavePressure::QSavePressure():
    m_strDir()
{
    _createDir();
    m_pFile = NULL;
}

QSavePressure::~QSavePressure()
{
	if(NULL != m_pFile)
	{
		m_pFile->flush();
		delete m_pFile;
		m_pFile = NULL;
	}
}

void QSavePressure::writeData(const QString &strValue)
{
	if(NULL == m_pFile)
	{
		return;
	}

    if(m_pFile->isOpen() && m_pFile->isWritable())
    {
        m_pFile->write(strValue.toLocal8Bit());
        m_pFile->flush();
    }
}

// 保存文件
void QSavePressure::save()
{
	if(NULL == m_pFile || !m_pFile->isOpen())
	{
		//qDebug()<<"Non SaveFile";
		return;
	}

	if(!m_pFile->exists() || !m_pFile->isOpen())
	{
		m_pFile->setFileName(m_pFile->fileName());
		m_pFile->open(QFile::WriteOnly);
	}
    m_pFile->flush();
    m_pFile->close();
}

// 获取文件名
const QString QSavePressure::fileName() const
{
	if(NULL == m_pFile)
	{
		return "";
	}
    return m_pFile->fileName();
}

// 设置文件名
void QSavePressure::setFileName(const QString &strFileName)
{
	if(NULL == m_pFile)
	{
		m_pFile = new QFile(strFileName);
		m_pFile->open(QFile::WriteOnly);
	}
	else
	{
		_createDir();
		m_pFile->setFileName(strFileName);
		m_pFile->open(QFile::WriteOnly);
	}
}

// 创建路径
void QSavePressure::_createDir()
{
	if(m_strDir.isEmpty())
    {
        m_strDir = ".//Data";
    }

	if(QDir::current().dirName() == "Data")
	{
		return;
	}

    QDir dirCurr(m_strDir);
    // 判断当前目录是否存在
    if(!m_strDir.isEmpty() && !dirCurr.exists())
    {
        QDir dir;
        bool ret = dir.mkdir(m_strDir);
        QDir::setCurrent(m_strDir);
    }

    QDir::setCurrent(m_strDir);
}

QString QSavePressure::getAbsoluteFileName()
{
	_createDir();
	return QDir::current().absoluteFilePath(fileName());
}
