#include "MainWindow.h"
#include "Sharpen.h"

extern MainWindow *g_mainWindowP;


Sharpen::Sharpen(QWidget *parent) : Blur(parent){}

QGroupBox* 
Sharpen::controlPanel(){		// create control panel
	m_ctrlGrp = new QGroupBox("Sharpen");

	QLabel *labelF = new QLabel;
	labelF->setText(QString("Sharpen factor"));
	
	// for minimum
	QLabel *labelXsz = new QLabel(tr("X size"));

	m_sliderXsz = new QSlider(Qt::Horizontal, m_ctrlGrp);
	m_sliderXsz->setTickPosition(QSlider::TicksBelow);
	m_sliderXsz->setTickInterval(10);
	m_sliderXsz->setRange(1, 101);
	m_sliderXsz->setValue(1); // neighborhood can be 1*1 min

	m_spinBoxXsz = new QSpinBox(m_ctrlGrp);
	m_spinBoxXsz->setRange(1, 101);
	m_spinBoxXsz->setValue(1);


	// for maximum
	QLabel *labelYsz = new QLabel(tr("Y size"));

	m_sliderYsz = new QSlider(Qt::Horizontal, m_ctrlGrp);
	m_sliderYsz->setTickPosition(QSlider::TicksBelow);
	m_sliderYsz->setTickInterval(10);
	m_sliderYsz->setRange(1, 101); /// range is 1 to 11 
	m_sliderYsz->setValue(1); // neighborhood can be 1*1 min

	m_spinBoxYsz = new QSpinBox(m_ctrlGrp);
	m_spinBoxYsz->setRange(1, 101);
	m_spinBoxYsz->setValue(1);

	// couple checkbox
	m_coupleSZ = new QCheckBox("Combine x y values", m_ctrlGrp);

	m_sliderFctr = new QSlider(Qt::Horizontal, m_ctrlGrp);
	m_sliderFctr->setTickPosition(QSlider::TicksBelow);
	m_sliderFctr->setTickInterval(10);
	m_sliderFctr->setRange(0, 100); // range of slider - 256 to 256
	m_sliderFctr->setValue(0);
	// spinbox
	m_spinBoxFctr = new QSpinBox(m_ctrlGrp);
	m_spinBoxFctr->setRange(0, 100); // range of slider - 256 to 256
	m_spinBoxFctr->setValue(0);

	connect(m_sliderXsz, SIGNAL(valueChanged(int)), this, SLOT(changeXsz(int)));
	connect(m_spinBoxXsz, SIGNAL(valueChanged(int)), this, SLOT(changeXsz(int)));

	connect(m_sliderYsz, SIGNAL(valueChanged(int)), this, SLOT(changeYsz(int)));
	connect(m_spinBoxYsz, SIGNAL(valueChanged(int)), this, SLOT(changeYsz(int)));

	connect(m_sliderFctr, SIGNAL(valueChanged(int)), this, SLOT(changeFctr(int)));
	connect(m_spinBoxFctr, SIGNAL(valueChanged(int)), this, SLOT(changeFctr(int)));

	connect(m_coupleSZ, SIGNAL(stateChanged(int)), this, SLOT(changeCouple(int)));

	QGridLayout *layout = new QGridLayout;

	layout->addWidget(labelF, 0, 0);
	layout->addWidget(m_sliderFctr, 0, 1);
	layout->addWidget(m_spinBoxFctr, 0, 2);

	layout->addWidget(labelXsz, 1, 0);
	layout->addWidget(m_sliderXsz, 1, 1);
	layout->addWidget(m_spinBoxXsz, 1, 2);

	layout->addWidget(labelYsz, 2, 0);
	layout->addWidget(m_sliderYsz, 2, 1);
	layout->addWidget(m_spinBoxYsz, 2, 2);

	layout->addWidget(m_coupleSZ, 3, 0, 1 , 3);

	m_ctrlGrp->setLayout(layout);
	disable(true);
	return m_ctrlGrp;

}

/*this function returns sharpened image*/
void
Sharpen::getSharp(ImagePtr I1, int xsz,int ysz, int fctr, ImagePtr I2){

	Blur::getBlur(I1, xsz, ysz, I2);

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
			val= CLIP((val * fctr),0,MXGRAY-1); // multiply by factor
			*p2 = CLIP(val + *p1, 0, MXGRAY - 1); // add to input
			p1++; p2++;
		}
	}

}

bool
Sharpen::applyFilter(ImagePtr I1, ImagePtr I2){
	if (I1.isNull()) return 0;
	int xsz = m_sliderXsz->value();
	int ysz = m_sliderYsz->value();
	int fctr = m_spinBoxFctr->value();

	getSharp(I1,xsz,ysz,fctr, I2);


	return 1;
}

void
Sharpen::disable(bool flag){
	m_sliderXsz->setDisabled(flag);
	m_sliderYsz->setDisabled(flag);

	m_spinBoxXsz->setDisabled(flag);
	m_spinBoxYsz->setDisabled(flag);

	m_sliderFctr->setDisabled(flag);
	m_spinBoxFctr->setDisabled(flag);

}

void 
Sharpen::reset(){
	// disable signals
	m_sliderYsz->blockSignals(true);
	m_spinBoxYsz->blockSignals(true);

	m_sliderXsz->blockSignals(true);
	m_spinBoxXsz->blockSignals(true);

	m_coupleSZ->blockSignals(true);

	m_sliderFctr->blockSignals(true);
	m_spinBoxFctr->blockSignals(true);

	// set 0 to all fields
	// reset values
	m_sliderXsz->setValue(1);		 // start is 0
	m_spinBoxXsz->setValue(2);
	m_sliderYsz->setValue(1);	 // start is 0
	m_spinBoxYsz->setValue(1);	 // start is 0

	m_sliderFctr->setValue(0);
	m_spinBoxFctr->setValue(1.); // normal contrast is 1

	m_coupleSZ->setChecked(false);

	// enable signals
	m_sliderYsz->blockSignals(false);
	m_spinBoxYsz->blockSignals(false);

	m_sliderXsz->blockSignals(false);
	m_spinBoxXsz->blockSignals(false);
	m_coupleSZ->blockSignals(false);

	m_sliderFctr->blockSignals(false);
	m_spinBoxFctr->blockSignals(false);

	// apply filter to source image; save result in destination image as 0 brightness and contrast
	applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

	// display output
	g_mainWindowP->displayOut();

}

void
Sharpen::changeXsz(int xsz){
	if (xsz % 2 == 0 && xsz > 0){ // maintain odd
		if (xsz >= m_sliderXsz->value() && xsz >= m_spinBoxXsz->value())	xsz++; // if increasing increase
		else
			xsz--;
	}
	// if checked change all value
	if (m_coupleSZ->isChecked()){
		m_sliderYsz->blockSignals(true);
		m_spinBoxYsz->blockSignals(true);

		m_sliderYsz->setValue(xsz);
		m_spinBoxYsz->setValue(xsz);

		m_sliderYsz->blockSignals(false);
		m_spinBoxYsz->blockSignals(false);
	}
	m_sliderXsz->blockSignals(true);
	m_spinBoxXsz->blockSignals(true);

	m_sliderXsz->setValue(xsz);
	m_spinBoxXsz->setValue(xsz);

	m_sliderXsz->blockSignals(false);
	m_spinBoxXsz->blockSignals(false);

	// apply filter to source image; save result in destination image
	applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

	// display output
	g_mainWindowP->displayOut();
}

void
Sharpen::changeYsz(int ysz){
	if (ysz % 2 == 0 && ysz > 0){ // maintain odd
		if (ysz >= m_sliderYsz->value() || ysz >= m_spinBoxYsz->value())	ysz++; // if increasing increase
		else{
			ysz--;
		}
	}
	// check if coupled
	if (m_coupleSZ->isChecked()){
		m_sliderXsz->blockSignals(true);
		m_spinBoxXsz->blockSignals(true);

		m_sliderXsz->setValue(ysz);
		m_spinBoxXsz->setValue(ysz);

		m_sliderXsz->blockSignals(false);
		m_spinBoxXsz->blockSignals(false);
	}
	m_sliderYsz->blockSignals(true);
	m_spinBoxYsz->blockSignals(true);

	m_sliderYsz->setValue(ysz);
	m_spinBoxYsz->setValue(ysz);

	m_sliderYsz->blockSignals(false);
	m_spinBoxYsz->blockSignals(false);

	// apply filter to source image; save result in destination image
	applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

	// display output
	g_mainWindowP->displayOut();
}

void
Sharpen::changeCouple(int){
	if (m_coupleSZ->isChecked()){
		int val;

		// use smaller value
		val = (m_sliderXsz->value() < m_sliderYsz->value()) ? m_sliderXsz->value() : m_sliderYsz->value();

		m_sliderYsz->blockSignals(true);
		m_spinBoxYsz->blockSignals(true);

		m_sliderYsz->setValue(val);
		m_spinBoxYsz->setValue(val);

		m_sliderYsz->blockSignals(false);
		m_spinBoxYsz->blockSignals(false);

		m_sliderXsz->blockSignals(true);
		m_spinBoxXsz->blockSignals(true);

		m_sliderXsz->setValue(val);
		m_spinBoxXsz->setValue(val);

		m_sliderXsz->blockSignals(false);
		m_spinBoxXsz->blockSignals(false);
		// apply filter to source image; save result in destination image
		applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

		// display output
		g_mainWindowP->displayOut();
	}
	else
		// if unchecked no need to apply filter
		return;
}

void
Sharpen::changeFctr(int fctr){

	m_sliderFctr->blockSignals(true);
	m_spinBoxFctr->blockSignals(true);

	m_sliderFctr->setValue(fctr);
	m_spinBoxFctr->setValue(fctr);

	m_sliderFctr->blockSignals(false);
	m_spinBoxFctr->blockSignals(false);


	// apply filter to source image; save result in destination image
	applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

	// display output
	g_mainWindowP->displayOut();

}
