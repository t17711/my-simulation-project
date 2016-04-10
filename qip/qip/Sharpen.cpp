#include "MainWindow.h"
#include "Sharpen.h"

extern MainWindow *g_mainWindowP;


Sharpen::Sharpen(QWidget *parent) : Blur(parent)
{
}

QGroupBox* 
Sharpen::controlPanel(){		// create control panel
	m_ctrlGrp = new QGroupBox("Sharpen");

	QLabel *labelS= new QLabel;
	labelS->setText(QString("Window Size"));

	QLabel *labelF = new QLabel;
	labelF->setText(QString("Sharpen factor"));
	
	// slider

	m_sliderSz = new QSlider(Qt::Horizontal, m_ctrlGrp);
	m_sliderSz->setTickPosition(QSlider::TicksBelow);
	m_sliderSz->setTickInterval(10);
	m_sliderSz->setRange(1, MXGRAY); // range of slider - 256 to 256
	m_sliderSz->setValue(1);

	// spinbox
	m_spinBoxSz = new QSpinBox(m_ctrlGrp);
	m_spinBoxSz->setRange(1,MXGRAY);
	m_spinBoxSz->setValue(1);

	m_sliderFctr = new QSlider(Qt::Horizontal, m_ctrlGrp);
	m_sliderFctr->setTickPosition(QSlider::TicksBelow);
	m_sliderFctr->setTickInterval(10);
	m_sliderFctr->setRange(-100, 100); // range of slider - 256 to 256
	m_sliderFctr->setValue(0);
	// spinbox
	m_spinBoxFctr = new QDoubleSpinBox(m_ctrlGrp);
	m_spinBoxFctr->setRange(0.25, 10.0); // range of slider - 256 to 256
	m_spinBoxFctr->setValue(0);

	connect(m_spinBoxSz, SIGNAL(valueChanged(int)), this, SLOT(changeSlider(int)));
	connect(m_sliderSz, SIGNAL(valueChanged(int)), this, SLOT(changeSlider(int)));
		
	connect(m_sliderFctr, SIGNAL(valueChanged(int)), this, SLOT(changeFctr(int)));
	connect(m_spinBoxFctr, SIGNAL(valueChanged(double)), this, SLOT(changeFctrD(double)));

	QGridLayout *layout = new QGridLayout;

	layout->addWidget(labelS, 1, 0);
	layout->addWidget(m_sliderSz, 1, 1);
	layout->addWidget(m_spinBoxSz, 1, 2);

	layout->addWidget(labelF, 2, 0);
	layout->addWidget(m_sliderFctr, 2, 1);
	layout->addWidget(m_spinBoxFctr, 2, 2);

	m_ctrlGrp->setLayout(layout);
	disable(true);
	return m_ctrlGrp;

}

void
Sharpen::getSharp(ImagePtr I1, int sz, double fctr, ImagePtr I2){

	Blur::getBlur(I1, sz, sz, I2);

	int w = I1->width();  // input image
	int h = I1->height();
	int total = w * h; // 

	int type;
	ChannelPtr<uchar> p1, p2, endd;
	int val = 0;
	for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
		IP_getChannel(I2, ch, p2, type); // gets channle 0 1 or 2 (r, g ,b) array 

		for (endd = p1 + total; p1 < endd;) {
			val= *p1 - *p2; // diff to orgnal picture to blurred
			val= (int)(val * fctr); // multiply by factor
			*p2 = CLIP(val + *p1, 0, MXGRAY - 1); // add to input

			p1++; p2++;
		}
	}

}

bool
Sharpen::applyFilter(ImagePtr I1, ImagePtr I2){
	if (I1.isNull()) return 0;
	int sz = m_sliderSz->value();
	double fctr = m_spinBoxFctr->value();

	getSharp(I1,sz,fctr, I2);


	return 1;
}

void
Sharpen::disable(bool flag){
	m_sliderSz->setDisabled(flag);
	m_spinBoxSz->setDisabled(flag);
	m_sliderFctr->setDisabled(flag);
	m_spinBoxFctr->setDisabled(flag);

}

void 
Sharpen::reset(){
	m_sliderSz->blockSignals(true);
	m_spinBoxSz->blockSignals(true);
	m_sliderFctr->blockSignals(true);
	m_spinBoxFctr->blockSignals(true);

	// set 0 to all fields
	m_sliderSz->setValue(1);
	m_spinBoxSz->setValue(1);
	m_sliderFctr->setValue(0);
	m_spinBoxFctr->setValue(1.); // normal contrast is 1

	// enable signals
	m_sliderSz->blockSignals(false);
	m_spinBoxSz->blockSignals(false);
	m_sliderFctr->blockSignals(false);
	m_spinBoxFctr->blockSignals(false);

	// apply filter to source image; save result in destination image as 0 brightness and contrast
	applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

	// display output
	g_mainWindowP->displayOut();

}

void 
Sharpen::changeSlider(int sz){
	if (sz % 2 == 0 && sz > 0){ // maintain odd
		if (sz > m_sliderSz->value() || sz > m_spinBoxSz->value())	sz++; // if increasing increase
		else{
			sz--;
		}
	}
	m_sliderSz->blockSignals(true);
	m_spinBoxSz->blockSignals(true);

	m_sliderSz->setValue(sz);
	m_spinBoxSz->setValue(sz);

	m_sliderSz->blockSignals(false);
	m_spinBoxSz->blockSignals(false);


	// apply filter to source image; save result in destination image
	this->applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

	// display output
	g_mainWindowP->displayOut();

}

void
Sharpen::changeFctr(int fctr){
	double c = fctr;
	c = getFctr(c);

	m_sliderFctr->blockSignals(true);
	m_spinBoxFctr->blockSignals(true);

	m_sliderFctr->setValue(fctr);
	m_spinBoxFctr->setValue(c);

	m_sliderFctr->blockSignals(false);
	m_spinBoxFctr->blockSignals(false);


	// apply filter to source image; save result in destination image
	applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

	// display output
	g_mainWindowP->displayOut();

}

void
Sharpen::changeFctrD(double c){
	// set factr from 1/4 to 5, to -100 to 100
	if (c == 1) changeFctr(0);
	if (c > 1)
		c = ((c - 1.0) *25.0);
	else
		c = ((c - 1) * 133);

	int ctr = (int)c;
	changeFctr(ctr);
}

double
Sharpen::getFctr(double c){
	// set contrast to 1/4 to 5, from -100 to 100

	if (c >= 0)
		c = c / 25.0 + 1.0;
	else
		c = 1.0 + c / 133.;
	return c;
}

