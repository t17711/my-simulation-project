#include "HistogramMatch.h"
#include "MainWindow.h"
#include <math.h>       /* ceil */

extern MainWindow *g_mainWindowP;

HistogramMatch::
HistogramMatch(QWidget *parent) : ImageFilter(parent)
{
}

QGroupBox*
HistogramMatch::controlPanel(){
	m_ctrlGrp = new QGroupBox("Histogram Match");
	m_SliderHistMatch = new QSlider				(Qt::Horizontal,m_ctrlGrp);
	m_SpinBoxHistMatch	 = new QSpinBox							(m_ctrlGrp);
	
	QLabel *label1			 = new QLabel			(tr("Histo Match"));
	
	m_SliderHistMatch	->setRange		(-255, 255);
	m_SpinBoxHistMatch	->setRange		(-255, 255);

	m_SliderHistMatch->setTickInterval(1);

	QGridLayout *layout		= new QGridLayout;
	layout				->addWidget	(label1,			0, 0);
	layout				->addWidget	(m_SliderHistMatch, 0, 1);
	layout				->addWidget	(m_SpinBoxHistMatch,0, 2, Qt::AlignLeft);


	connect(m_SliderHistMatch,	SIGNAL(valueChanged(int)),		this, SLOT(changeHist		(int)));
	connect(m_SpinBoxHistMatch, SIGNAL(valueChanged(int)),		this, SLOT(changeHistSpin	(int)));

	m_ctrlGrp			->setLayout	(layout);

	disable(true);

	return(m_ctrlGrp);
}

bool
HistogramMatch::applyFilter(ImagePtr I1, ImagePtr I2){
	if (I1.isNull()) return 0;
	// create histogram and send to match
	long double histogram[MXGRAY];
	int scale = m_SliderHistMatch->value();	
	long double temp = 0.0;

	if (scale >= 0)
	for (int i = 0; i < MXGRAY; ++i) {
		temp = pow((long double)i / (long double)(MXGRAY-1), scale);
		histogram[i] = CLIP (temp,0,MXGRAY-1);
	}
	else{
		for (int i = 0; i < MXGRAY; ++i) {
			temp = (long double)i / (long double)(MXGRAY-1.0);
			temp = pow(temp, abs(scale));
			temp = 1 - temp;
			histogram[i] = CLIP(temp,0,MXGRAY-1);
		}
	}

	HistMatch(I1, histogram, I2);
	return 1;
}

void
HistogramMatch::reset(){
	m_SliderHistMatch->blockSignals(true);
	m_SpinBoxHistMatch->blockSignals(true);

	m_SliderHistMatch->setValue(0);
	m_SpinBoxHistMatch->setValue(0);

	m_SliderHistMatch->blockSignals(false);
	m_SpinBoxHistMatch->blockSignals(false);

}

void
HistogramMatch::disable(bool flag){
	m_SliderHistMatch->blockSignals(true);
	m_SpinBoxHistMatch->blockSignals(true);

	m_SliderHistMatch	->setDisabled(flag);
	m_SpinBoxHistMatch	->setDisabled(flag);

	m_SliderHistMatch->blockSignals(false);
	m_SpinBoxHistMatch->blockSignals(false);

}

void
HistogramMatch::getHistogram(ImagePtr I1, int histogram[]){

	int w = I1->width();  // input image
	int h = I1->height();
	int total = w * h; // 
	int type;

	for (int i = 0; i < MXGRAY; ++i) histogram[i] = 0;

	ChannelPtr<uchar> p1, endd;

	for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++){
		for (endd = p1 + total; p1 < endd;) histogram[*p1++]++;
	}
}

void 
HistogramMatch::changeHist(int val){
	m_SpinBoxHistMatch->blockSignals(true);
	m_SpinBoxHistMatch->setValue(val);
	m_SpinBoxHistMatch->blockSignals(false);

	m_SliderHistMatch->blockSignals(true);
	m_SliderHistMatch->setValue(val);
	m_SliderHistMatch->blockSignals(false);

	// apply filter to source image; save result in destination image
	applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

	// display output
	g_mainWindowP->displayOut();

}

void
HistogramMatch::changeHistSpin(int val){
	m_SpinBoxHistMatch->blockSignals(true);
	m_SpinBoxHistMatch->setValue(val);
	m_SpinBoxHistMatch->blockSignals(false);

	m_SliderHistMatch->blockSignals(true);
	m_SliderHistMatch->setValue(val);
	m_SliderHistMatch->blockSignals(false);

	// apply filter to source image; save result in destination image
	applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

	// display output
	g_mainWindowP->displayOut();


}

void 
HistogramMatch::HistMatch(ImagePtr I1, long double Histogram[], ImagePtr I2){

	IP_copyImageHeader(I1, I2);  // copys width height and other properties from i1 to i2
	int w = I1->width();  // input image
	int h = I1->height();
	int total = w * h; // 

	int histo[MXGRAY];
	int histogram[MXGRAY];

	int left[MXGRAY];
	int right[MXGRAY];
	int reserveLeft[MXGRAY];
	getHistogram(I1, histo);

	long double Havg; // find level to equalize
	/* normalize h2 to conform with dimensions of I1 */
	for (int i = Havg = 0; i<MXGRAY; i++) Havg += Histogram[i];

	double scale = (double)total / Havg;

	if ((int)scale != 1)
	for (int i = 0; i < MXGRAY; i++) histogram[i] = (int)round(Histogram[i] * scale);

	int R = 0;
	int Hsum = 0;
	int temp = 0;

	/* normalize h2 to conform with dimensions of I1 */
	//create temporary left to track overflow later
	int left2[MXGRAY];

	for (int i = 0; i<MXGRAY; i++) {
		left[i] = R; /* left end of interval */
		left2[i] = R; /* left end of interval */
		temp = histogram[R] - Hsum;
		reserveLeft[i] = (temp > histo[i]) ? 0 : temp; // max amount of i on left

		Hsum += histo[i]; /* cum. interval value */
		while (Hsum>histogram[R] && (MXGRAY - 1)>R) { /* make interval wider */
			Hsum -= histogram[R]; /* adjust Hsum */
			R++; /* update right end */
		}
		right[i] = R;
	}

	// create temporary histogram
	for (int i = 0; i<MXGRAY; i++) histo[i] = 0;

	int type;
	ChannelPtr<uchar> p1, p2, endd;

	for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
		IP_getChannel(I2, ch, p2, type); // gets channle 0 1 or 2 (r, g ,b) array 
		for (endd = p1 + total; p1 < endd;) {
			int p = left[*p1];

			//now compare how much of value can be added to the slot by comparing reserved place
			if (histo[p] < histogram[p]){
				if (left2[*p1] != p)*p2 = p; // if histo value is less than avg then copy point
				/* so check if left[*p1] is original left if it is so then check the reserved value, if
				reserved value is 0, then move to next place */
				else {
					if (reserveLeft[*p1] > 0){
						reserveLeft[*p1]--;
						*p2 = p;
					}
					else{
						left[*p1] = (p + 1< right[*p1]) ? p + 1 : right[*p1];
						p = left[*p1];
						*p2 = p;
					}
				}
			}
			else{
				left[*p1] = (p + 1< right[*p1]) ? p + 1 : right[*p1];
				p = left[*p1];
				*p2 = p;
			}
			histo[p]++;
			*p1++;
			*p2++;
		}
	}
}