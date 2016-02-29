// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// Contrast.cpp - Brightness/Contrast widget.
//
// Written by: George Wolberg, 2016
// ======================================================================

#include "MainWindow.h"
#include "Contrast.h"

extern MainWindow *g_mainWindowP;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::Contrast:
//
// Constructor.
//
Contrast::Contrast(QWidget *parent) : ImageFilter(parent)
{}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::applyFilter:
//
// Run filter on the image, transforming I1 to I2.
// Overrides ImageFilter::applyFilter().
// Return 1 for success, 0 for failure.
//
bool
Contrast::applyFilter(ImagePtr I1, ImagePtr I2)
{
	// INSERT YOUR CODE HERE
	if (I1.isNull()) return 0;

	
	// apply filter
	double b, c;	// brightness, contrast parameters
		b = m_sliderB->value();
		c = m_sliderC->value();

        contrast(I1, b, c, I2);

	return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::createGroupBox:
//
// Create group box for control panel.
//
QGroupBox*
Contrast::controlPanel()
{
	// init group box
	m_ctrlGrp = new QGroupBox("Brightness and Contrast");
	
	QGridLayout *layout = new QGridLayout;

	//label
	QLabel *labelB = new QLabel;
	labelB->setText(QString("Brightness"));

	// slider
	m_sliderB = new QSlider(Qt::Horizontal, m_ctrlGrp);
	m_sliderB->setTickPosition(QSlider::TicksBelow);
	m_sliderB->setTickInterval(25);
	m_sliderB->setMinimum(1);
	m_sliderB->setMaximum(MXGRAY);
	m_sliderB->setValue(MXGRAY >> 1);

	// spinbox
	m_spinBoxB = new QSpinBox(m_ctrlGrp);
	m_spinBoxB->setMinimum(1);
	m_spinBoxB->setMaximum(MXGRAY);
	m_spinBoxB->setValue(MXGRAY >> 1);
	// signal
	connect(m_sliderB, SIGNAL(valueChanged(int)), this, SLOT(changeBright(int)));
	connect(m_spinBoxB, SIGNAL(valueChanged(int)), this, SLOT(changeBright(int)));

	// add to layout
	layout->addWidget(labelB, 0, 0);
	layout->addWidget(m_sliderB, 0, 1);
	layout->addWidget(m_spinBoxB, 0, 2);

	
	// label
	QLabel *labelC = new QLabel;
	labelC->setText(QString("Contrast"));

	// slider
	m_sliderC = new QSlider(Qt::Horizontal, m_ctrlGrp);
	m_sliderC->setTickPosition(QSlider::TicksBelow);
	m_sliderC->setTickInterval(25);
	m_sliderC->setMinimum(1);
	m_sliderC->setMaximum(MXGRAY);
	m_sliderC->setValue(MXGRAY >> 1);
	// spinbox
	m_spinBoxC = new QSpinBox(m_ctrlGrp);
	m_spinBoxC->setMinimum(1);
	m_spinBoxC->setMaximum(MXGRAY);
	m_spinBoxC->setValue(MXGRAY >> 1);

	// set cortrast signals
	connect(m_sliderC, SIGNAL(valueChanged(int)), this, SLOT(changeContrast(int)));
	connect(m_spinBoxC, SIGNAL(valueChanged(int)), this, SLOT(changeContrast(int)));
	
	// add contrast to layout
	layout->addWidget(labelC, 1, 0);
	layout->addWidget(m_sliderC,1, 1);
	layout->addWidget(m_spinBoxC,1, 2);
	
	m_ctrlGrp->setLayout(layout);

	return(m_ctrlGrp);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// contrast:
//
// INSERT YOUR CODE HERE.
//
void
Contrast::contrast(ImagePtr I1, double brightness, double contrast, ImagePtr I2)
{
	IP_copyImageHeader(I1, I2);  // copys width height and other properties from i1 to i2
	int w = I1->width();  // input image
	int h = I1->height();
	int total = w * h; // 

	// calculate brightness
	//int i, bri[255];
	//for (i = 0; i < 255; ++i)
//.		bri[i] = brightness;

// more stuff

}
// change bright ness
void Contrast::changeBright(int bri){
	m_sliderB->blockSignals(true);
	m_sliderB->setValue(bri);
	m_sliderB->blockSignals(false);
	m_spinBoxB->blockSignals(true);
	m_spinBoxB->setValue(bri);
	m_spinBoxB->blockSignals(false);

	// apply filter to source image; save result in destination image
	applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

	// display output
	g_mainWindowP->displayOut();

}

void Contrast::changeContrast(int cont){
	m_sliderC->blockSignals(true);
	m_sliderC->setValue(cont);
	m_sliderC->blockSignals(false);
	m_spinBoxC->blockSignals(true);
	m_spinBoxC->setValue(cont);
	m_spinBoxC->blockSignals(false);

	// apply filter to source image; save result in destination image
	applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

	// display output
	g_mainWindowP->displayOut();


}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::reset:
//
// Reset parameters.
//
void
Contrast::reset() {}
