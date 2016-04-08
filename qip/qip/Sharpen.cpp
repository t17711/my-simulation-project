#include "MainWindow.h"
#include "Sharpen.h"

extern MainWindow *g_mainWindowP;


Sharpen::Sharpen(QWidget *parent) : Blur(parent)
{}

QGroupBox* 
Sharpen::controlPanel(){		// create control panel
	return Blur::controlPanel();
	
	connect(m_sliderXsz, SIGNAL(valueChanged(int)), this, SLOT(getSharp(int)));
	connect(m_spinBoxXsz, SIGNAL(valueChanged(int)), this, SLOT(getSharp(int)));
	connect(m_sliderYsz, SIGNAL(valueChanged(int)), this, SLOT(getSharp(int)));
	connect(m_spinBoxYsz, SIGNAL(valueChanged(int)), this, SLOT(getSharp(int)));

}

void
Sharpen::sharp(int){
	getSharp(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());
	g_mainWindowP->displayOut();
}

void
Sharpen::getSharp(ImagePtr I1, ImagePtr I2){
	ImagePtr I3;
	if (I2.isNull()) return;
	if (I1.isNull()) return;

	IP_copyImageHeader(I1, I3);  // copys width height and other properties from i1 to i2
	int w = I1->width();  // input image
	int h = I1->height();
	int total = w * h; // 

	int type;
	ChannelPtr<uchar> p1, p2,p3, endd;

	for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
		IP_getChannel(I2, ch, p2, type); // gets channle 0 1 or 2 (r, g ,b) array 
		IP_getChannel(I3, ch, p3, type); // gets channle 0 1 or 2 (r, g ,b) array 

		for (endd = p1 + total; p1 < endd;) {
			*p3 = CLIP(*p1 - *p2, 0 ,MXGRAY-1);
			p1++; p2++; p3++;
		}
	}

for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
		IP_getChannel(I2, ch, p2, type); // gets channle 0 1 or 2 (r, g ,b) array 
		IP_getChannel(I3, ch, p3, type); // gets channle 0 1 or 2 (r, g ,b) array 

		for (endd = p1 + total; p1 < endd;) {
			*p2 = CLIP(*p3+*p1,0,MXGRAY-1); // copy to ouput
			p1++; p2++; p3++;

		}
	}
}

bool
Sharpen::applyFilter(ImagePtr I1, ImagePtr I2){
	Blur::applyFilter(I1, I2);
	getSharp(I1, I2);
	return 1;
}