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

		// set contrast from 1/4 to 5, from -100 to 100
		c = getContrast(c);
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
	m_sliderB->setTickInterval(25);
	m_sliderB->setRange(-MXGRAY, MXGRAY); // range of slider - 256 to 256
	m_sliderB->setValue(0);

	// spinbox
	m_spinBoxB = new QSpinBox(m_ctrlGrp);
	m_spinBoxB->setRange(-MXGRAY, MXGRAY);
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
	m_sliderC->setRange(-100, 100); // range of slider - 256 to 256
	m_sliderC->setValue(0);
	// spinbox
	m_spinBoxC = new QDoubleSpinBox(m_ctrlGrp);
	m_spinBoxC->setRange(0.25, 5); // range of slider - 256 to 256
	m_spinBoxC->setValue(1);

	// set cortrast signals
	connect(m_sliderC, SIGNAL(valueChanged(int)), this, SLOT(changeContrast(int)));
	connect(m_spinBoxC, SIGNAL(valueChanged(double)), this, SLOT(changeContrast(double)));
	
	// add contrast to layout
	layout->addWidget(labelC, 1, 0);
	layout->addWidget(m_sliderC,1, 1);
	layout->addWidget(m_spinBoxC,1, 2);
	
	m_ctrlGrp->setLayout(layout);
	disable(true);
	return(m_ctrlGrp);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Contrast::reset:
//
// Reset parameters.
//
void
Contrast::reset() {
	// disable signals to prevent duplicate calculations

	m_sliderB->blockSignals(true);
	m_sliderC->blockSignals(true);
	m_spinBoxB->blockSignals(true);
	m_spinBoxC->blockSignals(true);

	// set 0 to all fields
	m_sliderB->setValue(0);
	m_sliderC->setValue(0);
	m_spinBoxB->setValue(0);
	m_spinBoxC->setValue(1); // normal contrast is 1

	// enable signals
	m_sliderB->blockSignals(false);
	m_sliderC->blockSignals(false);
	m_spinBoxB->blockSignals(false);
	m_spinBoxC->blockSignals(false);

	// apply filter to source image; save result in destination image as 0 brightness and contrast
	applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

	// display output
	g_mainWindowP->displayOut();
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// contrast:
//
// INSERT YOUR CODE HERE.
//

/* this disables all components if flag is true or enables all component if flag is false*/
void
Contrast::disable(bool flag){
	m_sliderB->setDisabled(flag);
	m_spinBoxB->setDisabled(flag);
	m_sliderC->setDisabled(flag);
	m_spinBoxC->setDisabled(flag);

}

///////////////////////////////////////////////////////////////////////////////////////
/* slot functions*/
///////////////////////////////////////////////////////////////////////////////

// this function is called when slider or spinbox for brightness is called
// it just makes both same value and calls apply filter
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

// this function is called when slider  for contrast is called
// it just converts slider value to contrast value and updates spinbox
void Contrast::changeContrast(int cont){
	// disable signals to prevent duplicate calculations
	double c = cont * 1.0;
	c = getContrast(c);
	m_sliderC->blockSignals(true);
	m_sliderC->setValue(cont);
	m_sliderC->blockSignals(false);
	m_spinBoxC->blockSignals(true);
	// to do
	m_spinBoxC->setValue(c);
	m_spinBoxC->blockSignals(false);

	// apply filter to source image; save result in destination image
	applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

	// display output
	g_mainWindowP->displayOut();


}

// this function is called when spinbox  for contrast is called
// it just converts spinbox value to integer value and updates  slider
void
Contrast::changeContrast(double c){
	// set contrast from 1/4 to 5, to -100 to 100
	if (c == 1) changeContrast(0);
	if (c > 1)
		c = ((c - 1.0) *25.0);
	else
		c =  ((c - 1)*133);
	
	int ctr = (int)c;
	changeContrast(ctr);
}

///////////////////////////////////////////////////////////////////////////////////////
/* filter functions*/
///////////////////////////////////////////////////////////////////////////////

// this is main contrast filter that creates LUT based on formula and applies LUT to I1 and outputs to I2
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
		// value is always 0 to 255
		lut[i] = (int)CLIP((i - reference)*contrast + shift, 0, 255);
	}
	// point operation
	int type;
	ChannelPtr<uchar> p1, p2, endd;
	for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
		IP_getChannel(I2, ch, p2, type); // gets channle 0 1 or 2 (r, g ,b) array 
		for (endd = p1 + total; p1 < endd;) *p2++ = lut[*p1++];  // set rgb to 0 below threshold and 255 above
	}

}

// this function gives a decimal contrast value from integer of slider for double spinbox
double
Contrast::getContrast(double c){
	// set contrast to 1/4 to 5, from -100 to 100

	if (c >= 0)
		c = c / 25.0 + 1.0;
	else
		c = 1.0 + c / 133.;
	return c;
}

