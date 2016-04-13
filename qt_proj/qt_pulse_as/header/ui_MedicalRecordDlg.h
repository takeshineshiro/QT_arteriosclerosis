/********************************************************************************
** Form generated from reading UI file 'MedicalRecordDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEDICALRECORDDLG_H
#define UI_MEDICALRECORDDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MedicalRecordDlg
{
public:
    QGridLayout *gridLayout_2;
    QSplitter *splitter;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *m_pedPersonNo;
    QLabel *label_2;
    QLineEdit *m_pedName;
    QLabel *label_3;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *m_pbtnQuery;
    QDateEdit *m_pEndTime;
    QDateEdit *m_pStartTime;
    QSpacerItem *horizontalSpacer_3;
    QLabel *m_plblTotalRecord;
    QTreeView *m_pQueryResultView;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *m_pbtnShowDetail;

    void setupUi(QWidget *MedicalRecordDlg)
    {
        if (MedicalRecordDlg->objectName().isEmpty())
            MedicalRecordDlg->setObjectName(QStringLiteral("MedicalRecordDlg"));
        MedicalRecordDlg->resize(533, 581);
        gridLayout_2 = new QGridLayout(MedicalRecordDlg);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        splitter = new QSplitter(MedicalRecordDlg);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setStyleSheet(QStringLiteral(""));
        splitter->setFrameShape(QFrame::NoFrame);
        splitter->setFrameShadow(QFrame::Raised);
        splitter->setOrientation(Qt::Horizontal);
        splitter->setOpaqueResize(true);
        splitter->setHandleWidth(5);
        groupBox = new QGroupBox(splitter);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setMinimumSize(QSize(100, 0));
        groupBox->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label, 0, 0, 1, 1);

        m_pedPersonNo = new QLineEdit(groupBox);
        m_pedPersonNo->setObjectName(QStringLiteral("m_pedPersonNo"));
        m_pedPersonNo->setClearButtonEnabled(true);

        gridLayout->addWidget(m_pedPersonNo, 0, 1, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        m_pedName = new QLineEdit(groupBox);
        m_pedName->setObjectName(QStringLiteral("m_pedName"));
        m_pedName->setClearButtonEnabled(true);

        gridLayout->addWidget(m_pedName, 1, 1, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(168, 37, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        m_pbtnQuery = new QPushButton(groupBox);
        m_pbtnQuery->setObjectName(QStringLiteral("m_pbtnQuery"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(m_pbtnQuery->sizePolicy().hasHeightForWidth());
        m_pbtnQuery->setSizePolicy(sizePolicy);
        m_pbtnQuery->setMinimumSize(QSize(120, 40));

        horizontalLayout->addWidget(m_pbtnQuery);


        gridLayout->addLayout(horizontalLayout, 4, 0, 1, 2);

        m_pEndTime = new QDateEdit(groupBox);
        m_pEndTime->setObjectName(QStringLiteral("m_pEndTime"));
        m_pEndTime->setCurrentSection(QDateTimeEdit::DaySection);

        gridLayout->addWidget(m_pEndTime, 3, 1, 1, 1);

        m_pStartTime = new QDateEdit(groupBox);
        m_pStartTime->setObjectName(QStringLiteral("m_pStartTime"));
        m_pStartTime->setCurrentSection(QDateTimeEdit::YearSection);

        gridLayout->addWidget(m_pStartTime, 2, 1, 1, 1);

        splitter->addWidget(groupBox);

        gridLayout_2->addWidget(splitter, 0, 0, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(149, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_3, 0, 1, 1, 1);

        m_plblTotalRecord = new QLabel(MedicalRecordDlg);
        m_plblTotalRecord->setObjectName(QStringLiteral("m_plblTotalRecord"));

        gridLayout_2->addWidget(m_plblTotalRecord, 1, 0, 1, 1);

        m_pQueryResultView = new QTreeView(MedicalRecordDlg);
        m_pQueryResultView->setObjectName(QStringLiteral("m_pQueryResultView"));
        m_pQueryResultView->setFrameShape(QFrame::StyledPanel);
        m_pQueryResultView->setFrameShadow(QFrame::Sunken);
        m_pQueryResultView->setLineWidth(1);
        m_pQueryResultView->setMidLineWidth(0);
        m_pQueryResultView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        m_pQueryResultView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        m_pQueryResultView->setIndentation(0);

        gridLayout_2->addWidget(m_pQueryResultView, 2, 0, 1, 2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        m_pbtnShowDetail = new QPushButton(MedicalRecordDlg);
        m_pbtnShowDetail->setObjectName(QStringLiteral("m_pbtnShowDetail"));
        m_pbtnShowDetail->setMinimumSize(QSize(120, 0));

        horizontalLayout_2->addWidget(m_pbtnShowDetail);


        gridLayout_2->addLayout(horizontalLayout_2, 3, 0, 1, 2);

        QWidget::setTabOrder(m_pedPersonNo, m_pedName);
        QWidget::setTabOrder(m_pedName, m_pEndTime);

        retranslateUi(MedicalRecordDlg);

        m_pbtnQuery->setDefault(true);


        QMetaObject::connectSlotsByName(MedicalRecordDlg);
    } // setupUi

    void retranslateUi(QWidget *MedicalRecordDlg)
    {
        MedicalRecordDlg->setWindowTitle(QApplication::translate("MedicalRecordDlg", "Form", 0));
        groupBox->setTitle(QApplication::translate("MedicalRecordDlg", "\346\237\245\350\257\242\346\235\241\344\273\266", 0));
        label->setText(QApplication::translate("MedicalRecordDlg", "\347\274\226\345\217\267", 0));
#ifndef QT_NO_TOOLTIP
        m_pedPersonNo->setToolTip(QApplication::translate("MedicalRecordDlg", "\346\202\243\350\200\205\347\274\226\345\217\267", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        m_pedPersonNo->setWhatsThis(QApplication::translate("MedicalRecordDlg", "\346\202\243\350\200\205\347\274\226\345\217\267", 0));
#endif // QT_NO_WHATSTHIS
        label_2->setText(QApplication::translate("MedicalRecordDlg", "\345\247\223\345\220\215", 0));
        label_3->setText(QApplication::translate("MedicalRecordDlg", "\346\265\213\351\207\217\345\214\272\351\227\264\344\273\216", 0));
        label_4->setText(QApplication::translate("MedicalRecordDlg", "\345\210\260", 0));
        m_pbtnQuery->setText(QApplication::translate("MedicalRecordDlg", "\346\237\245\350\257\242", 0));
        m_pEndTime->setDisplayFormat(QApplication::translate("MedicalRecordDlg", "yyyy/MM/dd", 0));
        m_pStartTime->setDisplayFormat(QApplication::translate("MedicalRecordDlg", "yyyy/MM/dd", 0));
        m_plblTotalRecord->setText(QApplication::translate("MedicalRecordDlg", "\346\273\241\350\266\263\346\235\241\344\273\266\345\205\261\357\274\2320\346\235\241\350\256\260\345\275\225", 0));
        m_pbtnShowDetail->setText(QApplication::translate("MedicalRecordDlg", "\346\237\245\347\234\213\347\273\223\346\236\234", 0));
    } // retranslateUi

};

namespace Ui {
    class MedicalRecordDlg: public Ui_MedicalRecordDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEDICALRECORDDLG_H
