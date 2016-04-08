#include "Blur.h"
#include "MainWindow.h"
#include <vector>

#define maxbuffer 255

extern MainWindow *g_mainWindowP;

Blur::Blur(QWidget *parent) : ImageFilter(parent){
}

QGroupBox*
Blur::controlPanel(){			// create control panel
	m_ctrlGrp = new QGroupBox("Blur window");

	// for minimum
	QLabel *labelXsz = new QLabel(tr("X size"));

	m_sliderXsz = new QSlider(Qt::Horizontal, m_ctrlGrp);
	m_sliderXsz->setTickPosition(QSlider::TicksBelow);
	m_sliderXsz->setTickInterval(10);
	m_sliderXsz->setRange(1, maxbuffer);
	m_sliderXsz->setValue(1); // neighborhood can be 1*1 min

	m_spinBoxXsz = new QSpinBox(m_ctrlGrp);
	m_spinBoxXsz->setRange(1, maxbuffer);
	m_spinBoxXsz->setValue(1);


	// for maximum
	QLabel *labelYsz = new QLabel(tr("Y size"));

	m_sliderYsz = new QSlider(Qt::Horizontal, m_ctrlGrp);
	m_sliderYsz->setTickPosition(QSlider::TicksBelow);
	m_sliderYsz->setTickInterval(10);
	m_sliderYsz->setRange(1, maxbuffer); /// range is 1 to 11 
	m_sliderYsz->setValue(1); // neighborhood can be 1*1 min

	m_spinBoxYsz = new QSpinBox(m_ctrlGrp);
	m_spinBoxYsz->setRange(1, maxbuffer);
	m_spinBoxYsz->setValue(1);


	// layout
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(labelXsz, 0, 0);
	layout->addWidget(m_sliderXsz, 1, 0);
	layout->addWidget(m_spinBoxXsz, 1, 1);

	layout->addWidget(labelYsz, 2, 0);
	layout->addWidget(m_sliderYsz, 3, 0);
	layout->addWidget(m_spinBoxYsz, 3, 1);

	connect(m_sliderXsz, SIGNAL(valueChanged(int)), this, SLOT(changeXsz(int)));
	connect(m_spinBoxXsz, SIGNAL(valueChanged(int)), this, SLOT(changeXsz(int)));
	connect(m_sliderYsz, SIGNAL(valueChanged(int)), this, SLOT(changeYsz(int)));
	connect(m_spinBoxYsz, SIGNAL(valueChanged(int)), this, SLOT(changeYsz(int)));


	m_ctrlGrp->setLayout(layout);
	// connect signal here
	
	disable(true);
	return(m_ctrlGrp);

}

void
Blur::disable(bool flag){
	m_sliderXsz->blockSignals(true);
	m_sliderYsz->blockSignals(true);
	m_spinBoxXsz->blockSignals(true);
	m_spinBoxYsz->blockSignals(true);

	m_sliderXsz->setDisabled(flag);
	m_sliderYsz->setDisabled(flag);

	m_spinBoxXsz->setDisabled(flag);
	m_spinBoxYsz->setDisabled(flag);

	m_sliderXsz->blockSignals(false);
	m_sliderYsz->blockSignals(false);

}

void
Blur::reset(){
	// disable signals
	m_sliderYsz->blockSignals(true);
	m_spinBoxYsz->blockSignals(true);

	m_sliderXsz->blockSignals(true);
	m_spinBoxXsz->blockSignals(true);

	// reset values
	m_sliderXsz->setValue(0);		 // start is 0
	m_spinBoxXsz->setValue(0);
	m_sliderYsz->setValue(0);	 // start is 0
	m_spinBoxYsz->setValue(0);	 // start is 0

	// enable signals
	m_sliderYsz->blockSignals(false);
	m_spinBoxYsz->blockSignals(false);

	m_sliderXsz->blockSignals(false);
	m_spinBoxXsz->blockSignals(false);

}


void 
Blur::changeXsz(int xsz){
	if (xsz % 2 == 0 && xsz > 0){ // maintain odd
		if (xsz > m_sliderXsz->value())	xsz++; // if increasing increase
		else
			xsz--;
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
Blur::changeYsz(int ysz){
	if (ysz % 2 == 0 && ysz > 0){ // maintain odd
		if (ysz > m_sliderYsz->value())	ysz++; // if increasing increase
		else{
			ysz--;
		}
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

bool
Blur::applyFilter(ImagePtr I1, ImagePtr I2){
	if (I1.isNull()) return 0;
	int xsz = m_sliderXsz->value();
	int ysz = m_sliderYsz->value();

	getBlur(I1, xsz, ysz, I2);

	return 1;
}
void
Blur::getBlur(ImagePtr I1, int xsz, int ysz, ImagePtr I2){
	ImagePtr temp;
	IP_copyImageHeader(I1, temp);  // copys width height and other properties from i1 to i2
	IP_copyImageHeader(I1, I2);  // copys width height and other properties from i1 to i2
	int w = I1->width();  // input image
	int h = I1->height();
	if (xsz >= w) xsz = 2 * ((int)w / 2) - 1; //use whole width -1
	if (ysz >= h) ysz = 2 * ((int)h / 2) - 1; //use whole width -1

	int total = w * h; 
	
	int type;
	ChannelPtr<uchar> p1, p2,p3, endd;
	

	for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++){

		IP_getChannel(temp, ch, p2, type); // gets channle 0 1 or 2 (r, g ,b) array 
		for (int i = 0; i < h;++i){ // go from top row to bottom
			// now get 1d blur of pixel width w, step 1, and neighborhood xsz
			getBlur_1D(p1, w, 1, xsz, p2);
			p1+=w; // go to next row
			p2+=w;
		}
		
		// reinitialize pointer to point to output picture to do columns
		p2 -= (total-1); // sent temp image to front

		// get pointer for output
		IP_getChannel(I2, ch, p3, type); // gets channle 0 1 or 2 (r, g ,b) array 

		for (int i = 0; i < w; ++i){ // go from 1st col to last
		// now get 1d blur of pixel width h, step w, and neighborhood ysz
			getBlur_1D(p2, h, w, ysz, p3); 
			p2++; // go to next row
			p3++;
		}
		
	}

}


void
Blur::getBlur_1D(IP::ChannelPtr<uchar> p1, int width, int steps, int size, IP::ChannelPtr<uchar> p2){
	int buffer_size = width + size - 1; // so buffer size is width plus size of neighborhood - 1 
	int extra = (size-1) / 2;
	if (size==1) {
		for (int i = 0; i < width; i++, p2 += steps, p1 += steps) {
			*p2 = *p1;
		}
		return;
	}
	std::vector<uint16_t> buffer; // save max row size
	buffer.reserve(buffer_size);
	
	// add val to front buffer, it is low(half (neighborehood size/2))
	for (int i = 0; i < extra; ++i) 	buffer.push_back(*p1);
	// copy row, step point by steps
	for (int i = 0; i < width; ++i) {
		buffer.push_back(*p1);
		p1 += steps;
	}

	p1 -= steps; // go back to pointer end
	// add val to end
	for (int i = 0; i < extra; ++i) buffer.push_back(*p1);

	// now find sum
	int sum = 0;
	// do front 1st
	int i = 0;
	for (; i < size; i++) sum += buffer[i];
	*p2= sum / size;
	p2 += steps;
	//goes from i+size to buffer end, deletes left adds new
	for (; i < buffer_size; i++, p2 += steps){
		sum += (buffer[i] -buffer[i - size]); // delete left end item  add right end of neighborhood
		// put in output
		*p2 = sum / size;
	}

}