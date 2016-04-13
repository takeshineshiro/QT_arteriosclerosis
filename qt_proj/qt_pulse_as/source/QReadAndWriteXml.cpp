#include "./header/stdafx.h"
#include <QDomDocument>
#include "./header/QReadAndWriteXml.h"


QReadAndWriteXml::QReadAndWriteXml(void)
{
	_createDir();
}


QReadAndWriteXml::~QReadAndWriteXml(void)
{
}

void QReadAndWriteXml::_createDir()
{
	QString strDir(".//Data");

	if (QDir::current().dirName() == "Data")
	{
		return;
	}

	QDir dirCurr(strDir);
	bool bRet = false;
	if(!dirCurr.exists())
	{
		QDir dir;
		bRet = dir.mkdir(strDir);
		QDir::setCurrent(strDir);
	}
	else
	{
		QDir::setCurrent(strDir);
	}
}

void QReadAndWriteXml::saveInfoAndMeasureValue(const tagPersonInfo & tagInfo)
{
	QString strFileName = tagInfo.m_strNo + "(" + tagInfo.m_strName + ").xml";
	QFile file(strFileName);

	// 文件存在，则更新
	if(file.exists())
	{
		// 打开文件失败
		if (!file.open(QFile::ReadWrite))
		{
			file.close();
			return;
		}

		QDomDocument doc;
		QString strError;
		int nErrorLine;
		int nErrorCode;
		if(!doc.setContent(&file, &strError, &nErrorLine,&nErrorCode))
		{
			file.close();
			return; 
		}

		//_updateInfo(tagInfo, doc);


		// 添加本次测量结果
		QDomElement root = doc.documentElement();
		QDomElement nodeMeasureValue = doc.createElement("MeasureValue");
		{
			QDomElement nodeDate = doc.createElement("Date");
			nodeDate.appendChild(doc.createTextNode(tagInfo.m_strDate));

			QDomElement nodeAge = doc.createElement("Age");
			nodeAge.appendChild(doc.createTextNode(tagInfo.m_strAge));

			QDomElement nodeHeight = doc.createElement("Height");
			nodeHeight.appendChild(doc.createTextNode(tagInfo.m_strHeight));

			QDomElement nodeWeight = doc.createElement("Weight");
			nodeWeight.appendChild(doc.createTextNode(tagInfo.m_strWeight));

			QDomElement nodeSection = doc.createElement("Section");
			nodeSection.appendChild(doc.createTextNode(tagInfo.m_strSection));

			QDomElement nodeDoctor = doc.createElement("Doctor");
			nodeDoctor.appendChild(doc.createTextNode(tagInfo.m_strDoctor));


			QDomElement nodeLeftArm = doc.createElement("LeftArm");
			nodeLeftArm.appendChild(doc.createTextNode(tagInfo.m_listLeftArm.join(",")));

			QDomElement nodeRightArm = doc.createElement("RightArm");
			nodeRightArm.appendChild(doc.createTextNode(tagInfo.m_listRightArm.join(",")));

			QDomElement nodeLeftAnkle = doc.createElement("LeftAnkle");
			nodeLeftAnkle.appendChild(doc.createTextNode(tagInfo.m_listLeftAnkle.join(",")));

			QDomElement nodeRightAnkle = doc.createElement("RightAnkle");
			nodeRightAnkle.appendChild(doc.createTextNode(tagInfo.m_listRightAnkle.join(",")));

			QDomElement nodeDiagnose = doc.createElement("Diagnose");
			nodeDiagnose.appendChild(doc.createTextNode(tagInfo.m_strDiagnose));

			QDomElement nodeHistory = doc.createElement("History");
			nodeHistory.appendChild(doc.createTextNode(tagInfo.m_listHistory.join(",")));

			QDomElement nodeNote = doc.createElement("Note");
			nodeNote.appendChild(doc.createTextNode(tagInfo.m_strNote));

			nodeMeasureValue.appendChild(nodeDate);
			nodeMeasureValue.appendChild(nodeAge);
			nodeMeasureValue.appendChild(nodeHeight);
			nodeMeasureValue.appendChild(nodeWeight);
			nodeMeasureValue.appendChild(nodeSection);
			nodeMeasureValue.appendChild(nodeDoctor);

			nodeMeasureValue.appendChild(nodeLeftArm);
			nodeMeasureValue.appendChild(nodeRightArm);
			nodeMeasureValue.appendChild(nodeLeftAnkle);
			nodeMeasureValue.appendChild(nodeRightAnkle);
			nodeMeasureValue.appendChild(nodeDiagnose);
			nodeMeasureValue.appendChild(nodeHistory);
			nodeMeasureValue.appendChild(nodeNote);
		}
		root.appendChild(nodeMeasureValue);

		file.seek(0);

		QTextStream out(&file);
		doc.save(out, 4);
	}
	else
	{
		// 打开文件失败
		if (!file.open(QFile::ReadWrite))
		{
			file.close();
			return;
		}

		QXmlStreamWriter xmlWriter;
		xmlWriter.setDevice(&file);

		xmlWriter.writeStartDocument();
		xmlWriter.setAutoFormatting(true);

		// 写入患者基本信息
		xmlWriter.writeStartElement("BasicInfo");
		xmlWriter.writeTextElement("NO", tagInfo.m_strNo);
		xmlWriter.writeTextElement("Name", tagInfo.m_strName);
		xmlWriter.writeTextElement("Sex", tagInfo.m_strSex);

		// 保存测量信息
		{
			xmlWriter.writeStartElement("MeasureValue");
			xmlWriter.writeTextElement("Date", tagInfo.m_strDate);
			xmlWriter.writeTextElement("Age", tagInfo.m_strAge);
			xmlWriter.writeTextElement("Height", tagInfo.m_strHeight);
			xmlWriter.writeTextElement("Weight", tagInfo.m_strWeight);

			xmlWriter.writeTextElement("Section", tagInfo.m_strSection);
			xmlWriter.writeTextElement("Doctor", tagInfo.m_strDoctor);

			xmlWriter.writeComment(QString::fromLocal8Bit("左右臂测量值的顺序为：收缩压、平均压、舒张压"));
			xmlWriter.writeTextElement("LeftArm", tagInfo.m_listLeftArm.join(","));
			xmlWriter.writeTextElement("RightArm", tagInfo.m_listRightArm.join(","));

			xmlWriter.writeComment(QString::fromLocal8Bit("左右踝测量值的顺序为：收缩压、平均压、舒张压、ABI、PWV"));
			xmlWriter.writeTextElement("LeftAnkle", tagInfo.m_listLeftAnkle.join(","));
			xmlWriter.writeTextElement("RightAnkle", tagInfo.m_listRightAnkle.join(","));

			xmlWriter.writeTextElement("Diagnose", tagInfo.m_strDiagnose);

			xmlWriter.writeTextElement("History", tagInfo.m_listHistory.join(","));
			xmlWriter.writeTextElement("Note", tagInfo.m_strNote);
			xmlWriter.writeEndElement();
		}
		xmlWriter.writeEndElement();
		xmlWriter.writeEndDocument();
	}
	file.close();
}

void QReadAndWriteXml::_updateInfo(const tagPersonInfo& tagInfo, QDomDocument &docInfo)
{
	if(docInfo.isNull())
	{
		return;
	}

	QDomNodeList listInfo = docInfo.elementsByTagName("BasicInfo");
	if(listInfo.length() == 1)
	{
		QDomNodeList nodeList = listInfo.at(0).childNodes();
		for(int i = 0 ; i < nodeList.length(); ++i)
		{
			QDomNode node = nodeList.at(i);
			if(!node.isElement())
			{
				continue;
			}

			if(node.nodeName() == "Age")
			{
				_updateNodeValue(node, tagInfo.m_strAge);
			}

			if(node.nodeName() == "Height")
			{
				_updateNodeValue(node, tagInfo.m_strHeight);
			}

			if(node.nodeName() == "Weight")
			{
				_updateNodeValue(node, tagInfo.m_strWeight);
			}
		}
	}
}

// 更新节点值
void QReadAndWriteXml::_updateNodeValue(QDomNode &node, const QString &strValue)
{
	// 1. 先获取旧的节点
	QDomNode oldNode = node.firstChild();

	// 2.设置新的值
	node.firstChild().setNodeValue(strValue);

	// 3.获取新的节点
	QDomNode newNode = node.firstChild();

	// 4.新旧节点更新
	node.replaceChild(newNode, oldNode);
}

QList<QStandardItem *> QReadAndWriteXml::queryHistoryRecord(const QString &strNo, const QString &strName, QMap<int, tagPersonInfo*> &mapIndexToInfo, 
															const QString &strStartTime, const QString &strEndTime)
{
	QList<QStandardItem *> listItem;
	QFileInfoList listFile = QDir::current().entryInfoList(QDir::Files);
	// 没有文件，返回空值
	if(listFile.isEmpty())
	{
		return listItem;
	}

	// 查询符合文件名中含有strNo及姓名的文件名
	QStringList listValidFile;

	QRegExp rxEnd(QString(".xml$"));
	// 找到符合条件的文件
	for(QFileInfoList::const_iterator iter = listFile.begin(); iter != listFile.end(); ++iter)
	{
		if(rxEnd.indexIn(iter->fileName()) != -1 &&
			iter->fileName().contains(strNo) && 
			iter->fileName().contains(strName)
			)
		{
			listValidFile.append(iter->fileName());
		}
	}

	if(listValidFile.isEmpty())
	{
		return listItem;
	}

	int nCountIndex = 0;

	for(QStringList::const_iterator iter = listValidFile.begin();  iter != listValidFile.end(); ++iter)
	{
		QFile file(*iter);
		// 打开文件失败
		if(!file.open(QFile::ReadOnly))
		{
			file.close();
			continue;
		}

		QDomDocument doc;
		if(!doc.setContent(&file))
		{
			file.close();
			continue;
		}
		QDomElement root = doc.documentElement();

		// 获取公用的编号、姓名、性别信息
		QString strReadNo;
		QString strReadName;
		QString strSex;
		QDomNodeList listNo = root.elementsByTagName("NO");
		if(!listNo.isEmpty())
		{
			strReadNo = listNo.at(0).toElement().text();
		}

		QDomNodeList listName = root.elementsByTagName("Name");
		if(!listName.isEmpty())
		{
			strReadName = listName.at(0).toElement().text();
		}

		// 没有编号、姓名，或者姓名及编号不匹配，重新读取文件
		if(strReadNo.isEmpty()  ||
		   strReadName.isEmpty() ||
		   !strNo.isEmpty() && !strReadNo.contains(strNo) ||
		   !strName.isEmpty() && !strReadName.contains(strName))
		{
			file.close();
			continue;
		}

		QDomNodeList listSex = root.elementsByTagName("Sex");
		if(!listSex.isEmpty())
		{
			strSex = listSex.at(0).toElement().text();
		}

		tagPersonInfo *pCurrInfo = NULL;

		// 患者编号
		QStandardItem *pNoItem = NULL;
		// 从后往前，按测量时间倒序
		QDomNode nextNode = root.lastChild();
		while(!nextNode.isNull())
		{
			QDomElement element = nextNode.toElement();
			// 如果为元素
			if(!element.isNull() && element.isElement())
			{
				bool bValidDate = false;
				QString strTag = element.tagName();
				// 标签为"MeasureValue"
				if(strTag == "MeasureValue")
				{
					QDomNode valueNode =   nextNode.firstChild();
					// 判断是否已经存在
					tagPersonInfo *pInfo = NULL;

					while(!valueNode.isNull())
					{
						QDomElement valueElement = valueNode.toElement();
						if(!valueElement.isNull() && valueElement.isElement())
						{
							QString strTag = valueElement.tagName();
							QString strNodeValue = valueElement.text();
							// 测量时间
							if(strTag == "Date")
							{
								QDate measureDate = QDateTime::fromString(strNodeValue, "yyyy-MM-dd HH:mm:ss").date();
								bool bLessThanStart = QDate::fromString(strStartTime, "yyyy/MM/dd") > measureDate;
								bool bMoreThanEnd = measureDate > QDate::fromString(strEndTime, "yyyy/MM/dd");
								if(bLessThanStart || bMoreThanEnd)
								{
									break;
								}

								// 序号
								listItem.append(new QStandardItem(QString::number(++nCountIndex)));
								// 编号
								listItem.append(new QStandardItem(strReadNo));
								// 姓名
								listItem.append(new QStandardItem(strReadName));
								// 性别
								listItem.append(new QStandardItem(strSex));

								if(mapIndexToInfo.contains(nCountIndex))
								{
									pInfo = mapIndexToInfo.value(nCountIndex);
								}
								else 
								{
									pInfo = new tagPersonInfo();
								}

								pInfo->m_strNo = strReadNo;
								pInfo->m_strName = strReadName;
								pInfo->m_strSex = strSex;

								pInfo->m_strDate = strNodeValue;
								bValidDate = true;
								listItem.append(new QStandardItem(strNodeValue));
							}
							// 年龄
							else if(strTag == "Age" && bValidDate)
							{
								pInfo->m_strAge = strNodeValue;
								listItem.insert(listItem.end() - 1, new QStandardItem(strNodeValue));
								//listItem.append();
							}
							// 身高
							else if(bValidDate && strTag == "Height")
							{
								pInfo->m_strHeight = strNodeValue;
							}
							// 体重
							else if(bValidDate && strTag == "Weight")
							{
								pInfo->m_strWeight = strNodeValue;
							}
							// 科室
							else if(bValidDate && strTag == "Section")
							{
								pInfo->m_strSection = strNodeValue;
							}
							// 医生或者执行测量的操作者
							else if (bValidDate && strTag == "Doctor")
							{
								pInfo->m_strDoctor = strNodeValue;
							}
							// 左臂测量数据
							else if (bValidDate && strTag == "LeftArm")
							{
								pInfo->m_listLeftArm = strNodeValue.split(",");
							}
							// 右臂测量数据
							else if (bValidDate && strTag == "RightArm")
							{
								pInfo->m_listRightArm = strNodeValue.split(",");
							}
							// 左踝测量数据
							else if(bValidDate && strTag == "LeftAnkle")
							{
								pInfo->m_listLeftAnkle = strNodeValue.split(",");
							}
							else if(bValidDate && strTag == "RightAnkle")
							{
								pInfo->m_listRightAnkle = strNodeValue.split(",");
							}
							else if (bValidDate && strTag == "Diagnose")
							{
								pInfo->m_strDiagnose = strNodeValue;
							}
							else if (bValidDate && strTag == "History")
							{
								pInfo->m_listHistory = strNodeValue.split(",");
							}
							else if (bValidDate && strTag == "Note")
							{
								pInfo->m_strNote = strNodeValue;
							} 
						}
						valueNode = valueNode.nextSibling();
					}

					if(bValidDate)
					{
						mapIndexToInfo.insert(nCountIndex, pInfo);
					}
				}
			}
			nextNode = nextNode.previousSibling();
			if(nextNode == root.firstChild())
			{
				break;
			}
		}
		file.close();
	}
	return listItem;
}
