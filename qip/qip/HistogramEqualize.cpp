#include "HistogramEqualize.h"
#include "MainWindow.h"
#include <math.h>       /* ceil */

extern MainWindow *g_mainWindowP;

HistogramEqualize::
HistogramEqualize(QWidget *parent) : HistogramMatch(parent)
{
}

QGroupBox*
HistogramEqualize::controlPanel()
{
	// init group box
	m_ctrlGrp = new QGroupBox("Histogram Equalize");

	m_CheckBoxCummulative = new QCheckBox(tr("Cumulative Equalize"),m_ctrlGrp);
	
	m_CheckBoxFlat = new QCheckBox(tr("Flat Equalize"),m_ctrlGrp);

	QGridLayout *layout		= new QGridLayout;
	layout->			addWidget	(m_CheckBoxCummulative,		0, 0, Qt::AlignLeft);
	layout->			addWidget	(m_CheckBoxFlat,			2, 0, Qt::AlignLeft);

	connect(m_CheckBoxCummulative,	SIGNAL(stateChanged(int)), this, SLOT(cumulativeHist	(int)));
	connect(m_CheckBoxFlat,		SIGNAL(stateChanged(int)), this, SLOT(maxFlatHist	(int)));

	m_ctrlGrp->		setLayout(layout);
	
	disable(true);

	return(m_ctrlGrp);
}

bool
HistogramEqualize::applyFilter(ImagePtr I1, ImagePtr I2){
	if (I1.isNull()) return 0;

	if (m_CheckBoxCummulative->isChecked())		cumulativeHist(0);
	else if (m_CheckBoxFlat->isChecked())	maxFlatHist(0);
	else reset();
	return 1;
}

void
HistogramEqualize::reset(){
	m_CheckBoxCummulative->	blockSignals	(true);
	m_CheckBoxFlat->		blockSignals	(true);

	m_CheckBoxCummulative->	setChecked		(false);
	m_CheckBoxFlat->		setChecked		(false);

	m_CheckBoxCummulative->	blockSignals	(false);
	m_CheckBoxFlat->		blockSignals	(false);

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
	m_CheckBoxFlat->		blockSignals	(true);

	m_CheckBoxCummulative->	setDisabled(flag);
	m_CheckBoxFlat->		setDisabled		(flag);

	m_CheckBoxCummulative->	blockSignals	(false);
	m_CheckBoxFlat->		blockSignals	(false);

}

void
HistogramEqualize::cumulativeHist(int){
	if (!m_CheckBoxCummulative->isChecked()) {
		reset();
		return;
	}
	// disable other checkbox 1st
	m_CheckBoxFlat->		blockSignals	(true);

	m_CheckBoxFlat->		setChecked		(false);

	m_CheckBoxFlat->		blockSignals	(false);
	
	// read current image
	ImagePtr I1 = g_mainWindowP->imageSrc();
	ImagePtr I2 = g_mainWindowP->imageDst();

	IP_copyImageHeader(I1, I2);  // copys width height and other properties from i1 to i2
	int w = I1->width();  // input image
	int h = I1->height();
	int total = w * h; // 

	int histogram[MXGRAY];
	getHistogram(I1, histogram);  // create histogram
	
	int lut[MXGRAY];
	lut[0] = histogram[0];
	// nou create cummulative histogram i.e. foe i>0 hist[i] = hist[i] +hist[(i-i)]
	for (int i = 1; i < MXGRAY; ++i) lut[i] = histogram[i]	+ lut[i - 1];
	
	double histTotal = lut[MXGRAY-1] +	0.0;

	// create look up table from the cummulative histogram by dividing each cummulative value by HistTOtal
	for (int i = 0; i < MXGRAY; ++i) 
		lut[i] = CLIP((lut[i] * ((MXGRAY-0.0) / histTotal)), 0, MXGRAY);


	int type;
	ChannelPtr<uchar> p1, p2, endd;
	// create image from look up table
	for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
		IP_getChannel(I2, ch, p2, type); // gets channle 0 1 or 2 (r, g ,b) array 
		for (endd = p1 + total; p1 < endd;) *p2++ = lut[*p1++];  // set rgb to 0 below threshold and 255 above
	}

	g_mainWindowP->displayOut();

}

// this distributes the histogram values to its neighbors to create flat hist
void
HistogramEqualize::maxFlatHist(int){
	if (!m_CheckBoxFlat->isChecked()) {
		reset();
		return;
	}
	// uncheck other checkbox
	m_CheckBoxCummulative->	blockSignals	(true);

	m_CheckBoxCummulative->	setChecked		(false);

	m_CheckBoxCummulative->	blockSignals	(false);

	// read image
	ImagePtr I1 = g_mainWindowP->imageSrc();
	ImagePtr I2 = g_mainWindowP->imageDst();

#if 0

	IP_copyImageHeader(I1, I2);  // copys width height and other properties from i1 to i2
	int w = I1->width();  // input image
	int h = I1->height();
	int total = w * h; // 
	// here we need
	int histo[MXGRAY]; // this histogram will store how much of slots in istogram are filled. it will look like output histogram
	int left[MXGRAY];  // stores the left position of a pixel of a intensity in result histogram
	int right[MXGRAY];	// stores the right position of a pixel of a intensity in result histogram
	int reserveLeft[MXGRAY]; // this stores how many pixels of a intensity can be mapped in leftmost position
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


#endif

	long double hist[MXGRAY];
	for (int i = 0; i < MXGRAY; ++i) hist[i] = 1.;
	this->HistMatch(I1, hist, I2);

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