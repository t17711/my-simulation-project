#include "Quantization.h"
#include "MainWindow.h"

extern MainWindow *g_mainWindowP;

Quantization::Quantization(QWidget *parent) : ImageFilter(parent)
{}

////////////////////////////////////////////////////////////////////////////////////////////
/*virtual functions first*/
///////////////////////////////////////////////////////////

// create layout for quantization
QGroupBox*
Quantization::controlPanel(){
	m_ctrlGrp = new QGroupBox("Quantization");

	// init widgets
	// create label[i]
	QLabel *label = new QLabel;
	label->setText(QString("Quantization"));

	// slider
	m_slider = new QSlider(Qt::Horizontal, m_ctrlGrp);
	m_slider->setTickPosition(QSlider::TicksBelow);
	m_slider->setTickInterval(10);
	m_slider->setRange(1, MXGRAY - 1); // range of slider 1 to 8 for quantization
	m_slider->setValue(MXGRAY - 1);

	// spinbox
	m_spinBox = new QSpinBox(m_ctrlGrp);
	m_spinBox->setRange(1, MXGRAY - 1);
	m_spinBox->setValue(MXGRAY - 1);

	// checkbox
	m_checkBox = new QCheckBox(tr("Dither"), m_ctrlGrp);

	QGridLayout *layout = new QGridLayout;
	layout->addWidget(label, 0, 0);
	layout->addWidget(m_slider, 0, 1);
	layout->addWidget(m_spinBox, 0, 2);

	layout->addWidget(m_checkBox, 1, 1, Qt::AlignLeft);

	connect(m_slider, SIGNAL(valueChanged(int)), this, SLOT(changeQnt(int)));
	connect(m_spinBox, SIGNAL(valueChanged(int)), this, SLOT(changeQnt(int)));
	connect(m_checkBox, SIGNAL(stateChanged(int)), this, SLOT(changeDither(int)));

	m_ctrlGrp->setLayout(layout);

	disable(true);
	return m_ctrlGrp;

}

/* this applies the Quantization filter on the images inserted I1 and outputs to I2 image*/
bool
Quantization::applyFilter(ImagePtr I1, ImagePtr I2){
	// error checking
	if (I1.isNull()) return 0;
	int level = m_slider->value();
	bool dither = m_checkBox->isChecked();
	quantization(I1, level, dither, I2);

	return 1;
}

/* this resets all the sliders and spinboxes to initial condition*/
void
Quantization::reset() {
	m_slider->setValue(MXGRAY - 1); // just set the value. signals will take care of everything
	m_checkBox->setChecked(false);
	;
}

/* this disables all components if flag is true or enables all component if flag is false*/
void
Quantization::disable(bool flag){
	m_slider->setDisabled(flag);
	m_spinBox->setDisabled(flag);
	m_checkBox->setDisabled(flag);

}

///////////////////////////////////////////////////////////////////////////////////////
/* slot functions*/
///////////////////////////////////////////////////////////////////////////////

// this is called when slider or spinbox for quantization levels is clanged
void 
Quantization :: changeQnt(int level){
	m_slider->blockSignals(true);
	m_slider->setValue(level);
	m_slider->blockSignals(false);
	m_spinBox->blockSignals(true);
	m_spinBox->setValue(level);
	m_spinBox->blockSignals(false);

	// apply filter to source image; save result in destination image
	applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

	// display output
	g_mainWindowP->displayOut();

}

// this is called when checkbox for diyther is checked or unchecked
void 
Quantization::changeDither(int){
	applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());
	g_mainWindowP->displayOut();

}


///////////////////////////////////////////////////////////////////////////////////////
/* filter functios*/
///////////////////////////////////////////////////////////////////////////////

// this createss a look up table for quantization and based on whether to dither or not it applies that lut
void 
Quantization::quantization(ImagePtr I1, int level, bool dither, ImagePtr I2){
	// any value below thr is 0.
	IP_copyImageHeader(I1, I2);  // copys width height and other properties from i1 to i2
	int w = I1->width();  // input image
	int h = I1->height();
	int scale = MXGRAY / level;
	int bias = scale / 2; // assign random error first
	int total = w * h; // 
	// compute lut[]

	int i, lut[MXGRAY];  // size 256
	for (i = 0; i < MXGRAY; ++i) lut[i] = scale * (int)(i / scale);

	int type;
	ChannelPtr<uchar> p1, p2, endd;

	if (dither){
		int k = 0;
		int j = 0;
		int ttttt = 0;
		int osc = 1;
		for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
			IP_getChannel(I2, ch, p2, type); // gets channle 0 1 or 2 (r, g ,b) array 
			for (endd = p1 + total; p1 < endd; p1++){
				 ttttt = ((rand() & 0x7FFF));
				j = ttttt/32767. * bias; //bias = scale / 2;
				if (osc == 1){
					k = CLIP(lut[*p1] + j, 0, MXGRAY);
					osc = -1;
				}
				else{
					k = CLIP(lut[*p1] - j, 0, MXGRAY);
					osc = 1;
				}
				*p2++ = k;
			}

		}
	}
	
	else{
		for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
			IP_getChannel(I2, ch, p2, type); // gets channle 0 1 or 2 (r, g ,b) array 
			for (endd = p1 + total; p1 < endd;){
				*p2++ = (lut[*p1++]);
			}
		}
	}
}

