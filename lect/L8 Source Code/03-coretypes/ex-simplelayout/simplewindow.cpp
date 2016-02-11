#include "simplewindow.h"

#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

SimpleWindow::SimpleWindow(QWidget *parent) :
    QWidget(parent)
{
    QLabel* noteLabel = new QLabel("Note:", this);
    QTextEdit* noteEdit = new QTextEdit(this);
    QPushButton* clearButton = new QPushButton("Clear", this);
    QPushButton* saveButton = new QPushButton("Save", this);

    // outer horizontal-layout
    QVBoxLayout *outer = new QVBoxLayout;
    outer->addWidget(noteLabel);
    outer->addWidget(noteEdit);
    {
        // nested inner layout
        QHBoxLayout *inner = new QHBoxLayout;
        inner->addWidget(clearButton);
        inner->addWidget(saveButton);
        outer->addLayout(inner);
    }
    this->setLayout(outer);
}
