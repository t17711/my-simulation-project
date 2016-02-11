/*************************************************************************
 *
 * Copyright (c) 2012 Digia Plc.
 * All rights reserved.
 *
 * See the LICENSE.txt file shipped along with this file for the license.
 *
 *************************************************************************/

#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("First GUI App");

    QLabel *itemLabel = new QLabel("Item:");
    QLineEdit *itemEdit = new QLineEdit;
    QPushButton *addItem = new QPushButton("Add");

    QGroupBox *groupBox = new QGroupBox("Item List");

    QListWidget *itemList = new QListWidget(groupBox);
    itemList->addItem("Item One");
    itemList->addItem("Item Two");
    itemList->addItem("Item Three");

    QVBoxLayout *layout = new QVBoxLayout;

    QHBoxLayout *itemAddLayout = new QHBoxLayout;
    itemAddLayout->addWidget(itemLabel);
    itemAddLayout->addWidget(itemEdit);
    itemAddLayout->addWidget(addItem);
    layout->addLayout(itemAddLayout);

    // Optional part

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(itemList);

    QVBoxLayout *editRemoveLayout = new QVBoxLayout;
    editRemoveLayout->addWidget(new QPushButton("Edit"));
    editRemoveLayout->addWidget(new QPushButton("Remove"));
    editRemoveLayout->addStretch(1);
    hbox->addLayout(editRemoveLayout);
    groupBox->setLayout(hbox);
    layout->addWidget(groupBox);

    window.setLayout(layout);
    window.show();

    return app.exec();
}
