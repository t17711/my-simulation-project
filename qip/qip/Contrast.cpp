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
		b = m_sliderB->value();  // brightness
		c = m_sliderC->value();	// contrast

		// set contrast from 1/4 to 5
		if (c >= 0)
			c = c / 25.0 + 1.0;
		else
			c = 1.0 + c / 133.;
		// set brightness to -256 to 256
		// 0 is middle

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
	// brightness setup
	//label
	QLabel *labelB = new QLabel;
	labelB->setText(QString("Brightness"));

	// slider
	m_sliderB = new QSlider(Qt::Horizontal, m_ctrlGrp);
	m_sliderB->setTickPosition(QSlider::TicksBelow);
	m_sliderB->setTickInterval(10);
	m_sliderB->setMinimum(-MXGRAY);
	m_sliderB->setMaximum(MXGRAY);
	m_sliderB->setValue(0);

	// spinbox
	m_spinBoxB = new QSpinBox(m_ctrlGrp);
	m_spinBoxB->setMinimum(-MXGRAY);
	m_spinBoxB->setMaximum(MXGRAY);
	m_spinBoxB->setValue(0);

	// signal
	connect(m_sliderB, SIGNAL(valueChanged(int)), this, SLOT(changeBright(int)));
	connect(m_spinBoxB, SIGNAL(valueChanged(int)), this, SLOT(changeBright(int)));

	// add to layout
	layout->addWidget(labelB, 0, 0);
	layout->addWidget(m_sliderB, 0, 1);
	layout->addWidget(m_spinBoxB, 0, 2);

	// contrast setup
	// label
	QLabel *labelC = new QLabel;
	labelC->setText(QString("Contrast"));

	// slider range is -100 to 100
	m_sliderC = new QSlider(Qt::Horizontal, m_ctrlGrp);
	m_sliderC->setTickPosition(QSlider::TicksBelow);
	m_sliderC->setTickInterval(25);
	m_sliderC->setMinimum(-100);
	m_sliderC->setMaximum(100);
	m_sliderC->setValue(0);
	// spinbox
	m_spinBoxC = new QSpinBox(m_ctrlGrp);
	m_spinBoxC->setMinimum(-100);
	m_spinBoxC->setMaximum(100);
	m_spinBoxC->setValue(0);

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

	int lut[MXGRAY];
	int shift = reference + brightness;
	// apply brightness or contrast
	for (int i = 0; i < MXGRAY; ++i){
		double temp = ((i - 128)*contrast + shift);
		// value is always 0 to 255
		lut[i] = (temp < 255) ? (temp>0? temp: 0 ): 255;
	}
	int type;
	ChannelPtr<uchar> p1, p2, endd;
	for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
		IP_getChannel(I2, ch, p2, type); // gets channle 0 1 or 2 (r, g ,b) array 
		for (endd = p1 + total; p1 < endd;) *p2++ = lut[*p1++];  // set rgb to 0 below threshold and 255 above
	}
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
