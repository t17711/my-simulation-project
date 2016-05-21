#include "HistogramEqualize.h"
#include "MainWindow.h"
#include <math.h>       /* ceil */

extern MainWindow *g_mainWindowP;

// this function inherits histogram match for flar equalix=zation
HistogramEqualize::
HistogramEqualize(QWidget *parent) : HistogramMatch(parent)
{
}

/*control panel
this returns a groupbox containing control panel*/
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

/* this applies the equalize filter on the images inserted I1 and outputs to I2 image*/
bool
HistogramEqualize::applyFilter(ImagePtr I1, ImagePtr I2){
	if (I1.isNull()) return 0;

	// this checks which check box is checked and then calls that function
	if (m_CheckBoxCummulative->isChecked())		cumulativeHist(0);
	else if (m_CheckBoxFlat->isChecked())	maxFlatHist(0);
	else reset();
	return 1;
}

/* this resets all the sliders and spinboxes to initial condition*/
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

/* this disables all components if flag is true or enables all component if flag is false*/
void
HistogramEqualize::disable(bool flag){
	m_CheckBoxCummulative->	blockSignals	(true);
	m_CheckBoxFlat->		blockSignals	(true);

	m_CheckBoxCummulative->	setDisabled(flag);
	m_CheckBoxFlat->		setDisabled		(flag);

	m_CheckBoxCummulative->	blockSignals	(false);
	m_CheckBoxFlat->		blockSignals	(false);

}

///////////////////////////////////////////////////////////////////////////////////////
/* slot functions as well as regular functions*/
///////////////////////////////////////////////////////////////////////////////

/* when cummulitative checkbox is checked other checkbox is unchecked and cummulative equalize is done
for this first i get cummulative histogram then i divide it by total pixel number to create lookup table,
it doesnot give flat histogram */
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
	
	// get max number of pixels same as total
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
/* this uses same function as hist match for 0 value i.e. flat*/
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

	long double hist[MXGRAY];
	for (int i = 0; i < MXGRAY; ++i) hist[i] = 1.;
	this->HistMatch(I1, hist, I2);

	g_mainWindowP->displayOut();

}

// get histogram of image
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