// 作用：用来时时保存测量的压力值

#ifndef QSAVE_PRESSURE_H
#define QSAVE_PRESSURE_H


class QFile;
class QString;


class QSavePressure
{
public:
    explicit QSavePressure();

    ~ QSavePressure();

    // 往文件中写数据
    void writeData(const QString &strValue);

    // 保存文件
    void save();

    // 获取文件名称
    const QString fileName() const;

    // 设置文件名称
    void setFileName(const QString &strFileName);

	// 返回绝对路径
	QString getAbsoluteFileName();
private:

    // 创建路径
    void _createDir();

private:

    // 文件对象
    QFile *m_pFile;

    // 当前目录
    QString m_strDir;

    Q_DISABLE_COPY(QSavePressure);
};

#endif // QSAVE_PRESSURE_H
