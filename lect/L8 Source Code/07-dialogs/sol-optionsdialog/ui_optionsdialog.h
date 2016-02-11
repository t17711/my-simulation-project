/********************************************************************************
** Form generated from reading UI file 'optionsdialog.ui'
**
** Created: Thu Sep 27 10:05:54 2012
**      by: Qt User Interface Compiler version 5.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPTIONSDIALOG_H
#define UI_OPTIONSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OptionsDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox_4;
    QCheckBox *checkBox_5;
    QSpacerItem *verticalSpacer;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox;
    QFormLayout *formLayout;
    QLabel *label;
    QPushButton *pushButton;
    QLabel *label_2;
    QPushButton *pushButton_2;
    QLabel *label_3;
    QPushButton *pushButton_3;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *OptionsDialog)
    {
        if (OptionsDialog->objectName().isEmpty())
            OptionsDialog->setObjectName(QStringLiteral("OptionsDialog"));
        OptionsDialog->resize(222, 234);
        verticalLayout = new QVBoxLayout(OptionsDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tabWidget = new QTabWidget(OptionsDialog);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        verticalLayout_2 = new QVBoxLayout(tab);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        checkBox = new QCheckBox(tab);
        checkBox->setObjectName(QStringLiteral("checkBox"));

        verticalLayout_2->addWidget(checkBox);

        checkBox_2 = new QCheckBox(tab);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));

        verticalLayout_2->addWidget(checkBox_2);

        checkBox_3 = new QCheckBox(tab);
        checkBox_3->setObjectName(QStringLiteral("checkBox_3"));

        verticalLayout_2->addWidget(checkBox_3);

        checkBox_4 = new QCheckBox(tab);
        checkBox_4->setObjectName(QStringLiteral("checkBox_4"));

        verticalLayout_2->addWidget(checkBox_4);

        checkBox_5 = new QCheckBox(tab);
        checkBox_5->setObjectName(QStringLiteral("checkBox_5"));

        verticalLayout_2->addWidget(checkBox_5);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        verticalLayout_3 = new QVBoxLayout(tab_2);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        groupBox = new QGroupBox(tab_2);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        formLayout = new QFormLayout(groupBox);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        formLayout->setWidget(0, QFormLayout::FieldRole, pushButton);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        pushButton_2 = new QPushButton(groupBox);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        formLayout->setWidget(1, QFormLayout::FieldRole, pushButton_2);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        pushButton_3 = new QPushButton(groupBox);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        formLayout->setWidget(2, QFormLayout::FieldRole, pushButton_3);


        verticalLayout_3->addWidget(groupBox);

        tabWidget->addTab(tab_2, QString());

        verticalLayout->addWidget(tabWidget);

        buttonBox = new QDialogButtonBox(OptionsDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(OptionsDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), OptionsDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), OptionsDialog, SLOT(reject()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(OptionsDialog);
    } // setupUi

    void retranslateUi(QDialog *OptionsDialog)
    {
        OptionsDialog->setWindowTitle(QApplication::translate("OptionsDialog", "Dialog", 0));
        checkBox->setText(QApplication::translate("OptionsDialog", "Minimize to tray", 0));
        checkBox_2->setText(QApplication::translate("OptionsDialog", "Exit on Escape <Esc>", 0));
        checkBox_3->setText(QApplication::translate("OptionsDialog", "File Change Alert", 0));
        checkBox_4->setText(QApplication::translate("OptionsDialog", "Full Path in Title", 0));
        checkBox_5->setText(QApplication::translate("OptionsDialog", "Save Window Position", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("OptionsDialog", "General", 0));
        groupBox->setTitle(QApplication::translate("OptionsDialog", "Text Appearance", 0));
        label->setText(QApplication::translate("OptionsDialog", "Font", 0));
        pushButton->setText(QApplication::translate("OptionsDialog", "...", 0));
        label_2->setText(QApplication::translate("OptionsDialog", "Text Color", 0));
        pushButton_2->setText(QApplication::translate("OptionsDialog", "...", 0));
        label_3->setText(QApplication::translate("OptionsDialog", "Background", 0));
        pushButton_3->setText(QApplication::translate("OptionsDialog", "...", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("OptionsDialog", "Editor", 0));
    } // retranslateUi

};

namespace Ui {
    class OptionsDialog: public Ui_OptionsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPTIONSDIALOG_H
