#include "HistogramEqualize.h"
#include "MainWindow.h"
#include <math.h>       /* ceil */

extern MainWindow *g_mainWindowP;

HistogramEqualize::
	HistogramEqualize (QWidget *parent) : ImageFilter(parent)
{
}

QGroupBox*
HistogramEqualize::controlPanel()
{
	// init group box
	m_ctrlGrp = new QGroupBox("Histogram Equalize");

	m_CheckBoxCummulative = new QCheckBox(tr("Cumulative Equalize"),m_ctrlGrp);
	
	m_CheckBoxEntropy = new QCheckBox(tr("Entropy Equalize"),m_ctrlGrp);

	QGridLayout *layout		= new QGridLayout;
	layout->			addWidget	(m_CheckBoxCummulative,		0, 0, Qt::AlignLeft);
	layout->			addWidget	(m_CheckBoxEntropy,			2, 0, Qt::AlignLeft);

	connect(m_CheckBoxCummulative,	SIGNAL(stateChanged(int)), this, SLOT(cumulativeHist	(int)));
	connect(m_CheckBoxEntropy,		SIGNAL(stateChanged(int)), this, SLOT(maxEntropyHist	(int)));

	m_ctrlGrp->		setLayout(layout);
	
	disable(true);

	return(m_ctrlGrp);
}

bool
HistogramEqualize::applyFilter(ImagePtr I1, ImagePtr I2){
	if (I1.isNull()) return 0;

	if (m_CheckBoxCummulative->isChecked())		cumulativeHist(0);
	else if (m_CheckBoxEntropy->isChecked())	maxEntropyHist(0);
	else reset();
	return 1;
}

void
HistogramEqualize::reset(){
	m_CheckBoxCummulative->	blockSignals	(true);
	m_CheckBoxEntropy->		blockSignals	(true);

	m_CheckBoxCummulative->	setChecked		(false);
	m_CheckBoxEntropy->		setChecked		(false);

	m_CheckBoxCummulative->	blockSignals	(false);
	m_CheckBoxEntropy->		blockSignals	(false);

	ImagePtr I1 = g_mainWindowP->imageSrc();
	ImagePtr I2 = g_mainWindowP->imageDst();


	IP_copyImageHeader(I1, I2);  // copys width height and other properties from i1 to i2
	int w = I1->width();  // input image
	int h = I1->height();
	int total = w * h; // 
	
	int type;
	ChannelPtr<uchar> p1, p2, endd;

	for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
			IP_getChannel(I2, ch, p2, type); // gets channle 0 1 or 2 (r, g ,b) array 
			for (endd = p1 + total; p1 < endd;) *p2++ = *p1++;  // set rgb to 0 below threshold and 255 above
	}

	g_mainWindowP->displayOut();

}

void
HistogramEqualize::disable(bool flag){
	m_CheckBoxCummulative->	blockSignals	(true);
	m_CheckBoxEntropy->		blockSignals	(true);

	m_CheckBoxCummulative->	setDisabled(flag);
	m_CheckBoxEntropy->		setDisabled		(flag);

	m_CheckBoxCummulative->	blockSignals	(false);
	m_CheckBoxEntropy->		blockSignals	(false);

}

void
HistogramEqualize::cumulativeHist(int){
	if (!m_CheckBoxCummulative->isChecked()) {
		reset();
		return;
	}
	
	m_CheckBoxEntropy->		blockSignals	(true);

	m_CheckBoxEntropy->		setChecked		(false);

	m_CheckBoxEntropy->		blockSignals	(false);
	
	ImagePtr I1 = g_mainWindowP->imageSrc();
	ImagePtr I2 = g_mainWindowP->imageDst();

	IP_copyImageHeader(I1, I2);  // copys width height and other properties from i1 to i2
	int w = I1->width();  // input image
	int h = I1->height();
	int total = w * h; // 

	int histogram[MXGRAY];
	getHistogram(I1, histogram);
	
	int lut[MXGRAY];
	lut[0] = histogram[0];
	for (int i = 1; i < MXGRAY; ++i) lut[i] = histogram[i]	+ lut[i - 1];
	
	double histTotal = lut[MXGRAY-1] +	0.0;

	for (int i = 0; i < MXGRAY; ++i) 
		lut[i] = CLIP((lut[i] * ((MXGRAY-0.0) / histTotal)), 0, MXGRAY);


	int type;
	ChannelPtr<uchar> p1, p2, endd;

	for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
		IP_getChannel(I2, ch, p2, type); // gets channle 0 1 or 2 (r, g ,b) array 
		for (endd = p1 + total; p1 < endd;) *p2++ = lut[*p1++];  // set rgb to 0 below threshold and 255 above
	}

	g_mainWindowP->displayOut();

}

void
HistogramEqualize::maxEntropyHist(int){
	if (!m_CheckBoxEntropy->isChecked()) {
		reset();
		return;
	}

	m_CheckBoxCummulative->	blockSignals	(true);

	m_CheckBoxCummulative->	setChecked		(false);

	m_CheckBoxCummulative->	blockSignals	(false);


	ImagePtr I1 = g_mainWindowP->imageSrc();
	ImagePtr I2 = g_mainWindowP->imageDst();


	IP_copyImageHeader(I1, I2);  // copys width height and other properties from i1 to i2
	int w = I1->width();  // input image
	int h = I1->height();
	int total = w * h; // 

	int histo[MXGRAY];
	int left[MXGRAY];
	int right[MXGRAY];
	int reserveLeft[MXGRAY];
	getHistogram(I1, histo);

	int Havg = (int)(ceil((0.0 + total) / (MXGRAY + 0.0))); // find level to equalize

	int R = 0;
	int Hsum = 0;
	int temp = 0;

	/* normalize h2 to conform with dimensions of I1 */
	//create temporary left to track overflow later
	int left2[MXGRAY];

	for (int i = 0; i<MXGRAY; i++) {
		left[i] = R; /* left end of interval */
		left2[i] = R; /* left end of interval */
		temp = Havg - Hsum;
		reserveLeft[i] = (temp > histo[i]) ? 0 : temp; // max amount of i on left

		Hsum += histo[i]; /* cum. interval value */
		while (Hsum>Havg && (MXGRAY - 1)>R) { /* make interval wider */
			Hsum -= Havg; /* adjust Hsum */
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
			if (histo[p] < Havg){
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

	g_mainWindowP->displayOut();

}

void
HistogramEqualize::getHistogram(ImagePtr I1, int histogram[]){

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