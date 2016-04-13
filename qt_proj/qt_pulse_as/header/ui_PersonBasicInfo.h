/********************************************************************************
** Form generated from reading UI file 'PersonBasicInfo.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PERSONBASICINFO_H
#define UI_PERSONBASICINFO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PersonBasicInfo
{
public:
    QHBoxLayout *horizontalLayout_3;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QLabel *label_14;
    QComboBox *m_pPersonNo;
    QLabel *label;
    QLineEdit *m_pedName;
    QLabel *label_2;
    QComboBox *m_pcboxSex;
    QLabel *label_3;
    QLineEdit *m_pedAge;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout;
    QLineEdit *m_pedHeight;
    QLabel *label_9;
    QLabel *label_5;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *m_pedWeight;
    QLabel *label_10;
    QLabel *label_6;
    QLabel *label_7;
    QLineEdit *m_pedDoctor;
    QLabel *label_8;
    QGridLayout *gridLayout;
    QCheckBox *m_pbtnNonHistory;
    QCheckBox *m_pbtnHighBlood;
    QCheckBox *m_pbtnHyperlipidemia;
    QCheckBox *m_pbtnCHD;
    QCheckBox *m_pbtnDHF;
    QCheckBox *m_pbtnHyperglycemia;
    QLabel *m_plblNote;
    QTextEdit *m_pedNote;
    QComboBox *m_pcboxSection;

    void setupUi(QWidget *PersonBasicInfo)
    {
        if (PersonBasicInfo->objectName().isEmpty())
            PersonBasicInfo->setObjectName(QStringLiteral("PersonBasicInfo"));
        PersonBasicInfo->resize(314, 658);
        horizontalLayout_3 = new QHBoxLayout(PersonBasicInfo);
        horizontalLayout_3->setSpacing(9);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setSizeConstraint(QLayout::SetDefaultConstraint);
        horizontalLayout_3->setContentsMargins(0, 10, 0, 0);
        groupBox = new QGroupBox(PersonBasicInfo);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setVerticalSpacing(15);
        label_14 = new QLabel(groupBox);
        label_14->setObjectName(QStringLiteral("label_14"));

        gridLayout_2->addWidget(label_14, 0, 0, 1, 1);

        m_pPersonNo = new QComboBox(groupBox);
        m_pPersonNo->setObjectName(QStringLiteral("m_pPersonNo"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(m_pPersonNo->sizePolicy().hasHeightForWidth());
        m_pPersonNo->setSizePolicy(sizePolicy);
        m_pPersonNo->setEditable(true);

        gridLayout_2->addWidget(m_pPersonNo, 0, 1, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_2->addWidget(label, 1, 0, 1, 1);

        m_pedName = new QLineEdit(groupBox);
        m_pedName->setObjectName(QStringLiteral("m_pedName"));

        gridLayout_2->addWidget(m_pedName, 1, 1, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_2->addWidget(label_2, 2, 0, 1, 1);

        m_pcboxSex = new QComboBox(groupBox);
        m_pcboxSex->setObjectName(QStringLiteral("m_pcboxSex"));

        gridLayout_2->addWidget(m_pcboxSex, 2, 1, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout_2->addWidget(label_3, 3, 0, 1, 1);

        m_pedAge = new QLineEdit(groupBox);
        m_pedAge->setObjectName(QStringLiteral("m_pedAge"));

        gridLayout_2->addWidget(m_pedAge, 3, 1, 1, 1);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_2->addWidget(label_4, 4, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        m_pedHeight = new QLineEdit(groupBox);
        m_pedHeight->setObjectName(QStringLiteral("m_pedHeight"));

        horizontalLayout->addWidget(m_pedHeight);

        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QStringLiteral("label_9"));

        horizontalLayout->addWidget(label_9);


        gridLayout_2->addLayout(horizontalLayout, 4, 1, 1, 1);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_2->addWidget(label_5, 5, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        m_pedWeight = new QLineEdit(groupBox);
        m_pedWeight->setObjectName(QStringLiteral("m_pedWeight"));

        horizontalLayout_2->addWidget(m_pedWeight);

        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QStringLiteral("label_10"));

        horizontalLayout_2->addWidget(label_10);


        gridLayout_2->addLayout(horizontalLayout_2, 5, 1, 1, 1);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout_2->addWidget(label_6, 6, 0, 1, 1);

        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout_2->addWidget(label_7, 7, 0, 1, 1);

        m_pedDoctor = new QLineEdit(groupBox);
        m_pedDoctor->setObjectName(QStringLiteral("m_pedDoctor"));

        gridLayout_2->addWidget(m_pedDoctor, 7, 1, 1, 1);

        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout_2->addWidget(label_8, 8, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setVerticalSpacing(12);
        m_pbtnNonHistory = new QCheckBox(groupBox);
        m_pbtnNonHistory->setObjectName(QStringLiteral("m_pbtnNonHistory"));

        gridLayout->addWidget(m_pbtnNonHistory, 0, 0, 1, 1);

        m_pbtnHighBlood = new QCheckBox(groupBox);
        m_pbtnHighBlood->setObjectName(QStringLiteral("m_pbtnHighBlood"));
        m_pbtnHighBlood->setCursor(QCursor(Qt::ArrowCursor));

        gridLayout->addWidget(m_pbtnHighBlood, 1, 0, 1, 1);

        m_pbtnHyperlipidemia = new QCheckBox(groupBox);
        m_pbtnHyperlipidemia->setObjectName(QStringLiteral("m_pbtnHyperlipidemia"));

        gridLayout->addWidget(m_pbtnHyperlipidemia, 1, 1, 1, 1);

        m_pbtnCHD = new QCheckBox(groupBox);
        m_pbtnCHD->setObjectName(QStringLiteral("m_pbtnCHD"));

        gridLayout->addWidget(m_pbtnCHD, 2, 0, 1, 1);

        m_pbtnDHF = new QCheckBox(groupBox);
        m_pbtnDHF->setObjectName(QStringLiteral("m_pbtnDHF"));

        gridLayout->addWidget(m_pbtnDHF, 2, 1, 1, 1);

        m_pbtnHyperglycemia = new QCheckBox(groupBox);
        m_pbtnHyperglycemia->setObjectName(QStringLiteral("m_pbtnHyperglycemia"));

        gridLayout->addWidget(m_pbtnHyperglycemia, 3, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 8, 1, 1, 1);

        m_plblNote = new QLabel(groupBox);
        m_plblNote->setObjectName(QStringLiteral("m_plblNote"));

        gridLayout_2->addWidget(m_plblNote, 9, 0, 1, 2);

        m_pedNote = new QTextEdit(groupBox);
        m_pedNote->setObjectName(QStringLiteral("m_pedNote"));

        gridLayout_2->addWidget(m_pedNote, 10, 0, 1, 2);

        m_pcboxSection = new QComboBox(groupBox);
        m_pcboxSection->setObjectName(QStringLiteral("m_pcboxSection"));
        m_pcboxSection->setEditable(true);

        gridLayout_2->addWidget(m_pcboxSection, 6, 1, 1, 1);

        gridLayout_2->setRowMinimumHeight(10, 1);

        horizontalLayout_3->addWidget(groupBox);


        retranslateUi(PersonBasicInfo);

        QMetaObject::connectSlotsByName(PersonBasicInfo);
    } // setupUi

    void retranslateUi(QWidget *PersonBasicInfo)
    {
        PersonBasicInfo->setWindowTitle(QApplication::translate("PersonBasicInfo", "Form", 0));
        groupBox->setTitle(QApplication::translate("PersonBasicInfo", "\345\237\272\346\234\254\344\277\241\346\201\257", 0));
        label_14->setText(QApplication::translate("PersonBasicInfo", "\347\274\226\345\217\267", 0));
#ifndef QT_NO_TOOLTIP
        m_pPersonNo->setToolTip(QApplication::translate("PersonBasicInfo", "\346\202\243\350\200\205\347\274\226\345\217\267(\347\224\261\345\271\264\346\234\210\346\227\245\346\227\266\345\210\206\347\247\222\347\273\204\346\210\220,\345\246\20220151010162930(14\344\275\215)", 0));
#endif // QT_NO_TOOLTIP
        label->setText(QApplication::translate("PersonBasicInfo", "\345\247\223\345\220\215", 0));
#ifndef QT_NO_TOOLTIP
        m_pedName->setToolTip(QApplication::translate("PersonBasicInfo", "\346\202\243\350\200\205\345\247\223\345\220\215(\346\234\200\345\244\23230\344\270\252\345\255\227\347\254\246)", 0));
#endif // QT_NO_TOOLTIP
        m_pedName->setText(QApplication::translate("PersonBasicInfo", "ma", 0));
        label_2->setText(QApplication::translate("PersonBasicInfo", "\346\200\247\345\210\253", 0));
        m_pcboxSex->clear();
        m_pcboxSex->insertItems(0, QStringList()
         << QApplication::translate("PersonBasicInfo", "\347\224\267", 0)
         << QApplication::translate("PersonBasicInfo", "\345\245\263", 0)
        );
        label_3->setText(QApplication::translate("PersonBasicInfo", "\345\271\264\351\276\204", 0));
#ifndef QT_NO_TOOLTIP
        m_pedAge->setToolTip(QApplication::translate("PersonBasicInfo", "\346\202\243\350\200\205\345\271\264\351\276\204", 0));
#endif // QT_NO_TOOLTIP
        m_pedAge->setText(QApplication::translate("PersonBasicInfo", "18", 0));
        label_4->setText(QApplication::translate("PersonBasicInfo", "\350\272\253\351\253\230", 0));
#ifndef QT_NO_TOOLTIP
        m_pedHeight->setToolTip(QApplication::translate("PersonBasicInfo", "\346\202\243\350\200\205\350\272\253\351\253\230", 0));
#endif // QT_NO_TOOLTIP
        m_pedHeight->setText(QString());
        label_9->setText(QApplication::translate("PersonBasicInfo", "cm", 0));
        label_5->setText(QApplication::translate("PersonBasicInfo", "\344\275\223\351\207\215", 0));
#ifndef QT_NO_TOOLTIP
        m_pedWeight->setToolTip(QApplication::translate("PersonBasicInfo", "\344\275\223\351\207\215", 0));
#endif // QT_NO_TOOLTIP
        m_pedWeight->setText(QString());
        label_10->setText(QApplication::translate("PersonBasicInfo", "Kg", 0));
        label_6->setText(QApplication::translate("PersonBasicInfo", "\347\247\221\345\256\244", 0));
        label_7->setText(QApplication::translate("PersonBasicInfo", "\345\214\273\347\224\237", 0));
#ifndef QT_NO_TOOLTIP
        m_pedDoctor->setToolTip(QApplication::translate("PersonBasicInfo", "\345\201\232\346\243\200\346\237\245\347\232\204\346\211\247\350\241\214\350\200\205", 0));
#endif // QT_NO_TOOLTIP
        m_pedDoctor->setText(QString());
        label_8->setText(QApplication::translate("PersonBasicInfo", "\347\227\205\345\217\262", 0));
#ifndef QT_NO_TOOLTIP
        m_pbtnNonHistory->setToolTip(QApplication::translate("PersonBasicInfo", "\346\262\241\346\234\211\346\202\243\347\227\205\345\216\206\345\217\262", 0));
#endif // QT_NO_TOOLTIP
        m_pbtnNonHistory->setText(QApplication::translate("PersonBasicInfo", "\346\227\240", 0));
#ifndef QT_NO_TOOLTIP
        m_pbtnHighBlood->setToolTip(QApplication::translate("PersonBasicInfo", "\346\202\243\346\234\211\351\253\230\350\241\200\345\216\213", 0));
#endif // QT_NO_TOOLTIP
        m_pbtnHighBlood->setText(QApplication::translate("PersonBasicInfo", "\351\253\230\350\241\200\345\216\213", 0));
        m_pbtnHyperlipidemia->setText(QApplication::translate("PersonBasicInfo", "\351\253\230\350\241\200\350\204\202", 0));
        m_pbtnCHD->setText(QApplication::translate("PersonBasicInfo", "\345\206\240\345\277\203\347\227\205", 0));
        m_pbtnDHF->setText(QApplication::translate("PersonBasicInfo", "\345\277\203\350\241\260", 0));
        m_pbtnHyperglycemia->setText(QApplication::translate("PersonBasicInfo", "\351\253\230\350\241\200\347\263\226", 0));
        m_plblNote->setText(QApplication::translate("PersonBasicInfo", "\345\244\207\346\263\250(\346\234\200\345\244\232300\345\255\227)", 0));
#ifndef QT_NO_TOOLTIP
        m_pedNote->setToolTip(QApplication::translate("PersonBasicInfo", "\345\244\207\346\263\250\344\277\241\346\201\257(\346\234\200\345\244\232300\345\255\227)", 0));
#endif // QT_NO_TOOLTIP
        m_pedNote->setHtml(QApplication::translate("PersonBasicInfo", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", 0));
#ifndef QT_NO_TOOLTIP
        m_pcboxSection->setToolTip(QApplication::translate("PersonBasicInfo", "\346\211\200\345\234\250\347\247\221\345\256\244", 0));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class PersonBasicInfo: public Ui_PersonBasicInfo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PERSONBASICINFO_H
