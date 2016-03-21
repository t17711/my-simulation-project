#include "HistogramEqualize.h"
#include "MainWindow.h"

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

	m_CheckBoxCummulative	= new QCheckBox		(m_ctrlGrp);
	QLabel *label1			= new QLabel		(tr("Cumulative Equalize"));
	
	
	m_CheckBoxEntropy		= new QCheckBox		(m_ctrlGrp);
	QLabel *label2			= new QLabel		(tr("Entropy Equalize"));

	QGridLayout *layout		= new QGridLayout;
	layout->			addWidget	(m_CheckBoxCummulative	,	0, 0, Qt::AlignRight);
	layout->			addWidget	(label1			,	0, 1, Qt::AlignLeft);
	layout->			addWidget	(m_CheckBoxEntropy, 2, 0, Qt::AlignRight);
	layout->			addWidget	(label2,			2, 1, Qt::AlignLeft);

	connect(m_CheckBoxCummulative,	SIGNAL(stateChanged(int)), this, SLOT(cumulativeHist	(int)));
	connect(m_CheckBoxEntropy,		SIGNAL(stateChanged(int)), this, SLOT(maxEntropyHist	(int)));

	m_ctrlGrp->		setLayout(layout);
	
	disable(true);

	return(m_ctrlGrp);
}

bool
HistogramEqualize::applyFilter(ImagePtr I1, ImagePtr I2){
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

	int type;
	ChannelPtr<uchar> p1, p2, endd;

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

	for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
		IP_getChannel(I2, ch, p2, type); // gets channle 0 1 or 2 (r, g ,b) array 
		for (endd = p1 + total; p1 < endd;) *p2++ = lut[*p1++];  // set rgb to 0 below threshold and 255 above
	}

	g_mainWindowP->displayOut();

}

void
HistogramEqualize::maxEntropyHist(int){
	m_CheckBoxCummulative->	blockSignals	(true);

	m_CheckBoxCummulative->	setChecked		(false);

	m_CheckBoxCummulative->	blockSignals	(false);


	ImagePtr I1 = g_mainWindowP->imageSrc();
	ImagePtr I2 = g_mainWindowP->imageDst();

	int type;
	ChannelPtr<uchar> p1, p2, endd;

	IP_copyImageHeader(I1, I2);  // copys width height and other properties from i1 to i2
	int w = I1->width();  // input image
	int h = I1->height();
	int total = w * h; // 

	int histogram[MXGRAY];
	getHistogram(I1, histogram);

	int lut[MXGRAY];
	lut[0] = histogram[0];
	for (int i = 1; i < MXGRAY; ++i) lut[i] = histogram[i] + lut[i - 1];

	double histTotal = lut[MXGRAY - 1] + 0.0;

	for (int i = 0; i < MXGRAY; ++i)
		lut[i] = CLIP((lut[i] * ((MXGRAY - 0.0) / histTotal)), 0, MXGRAY);

	for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
		IP_getChannel(I2, ch, p2, type); // gets channle 0 1 or 2 (r, g ,b) array 
		for (endd = p1 + total; p1 < endd;) *p2++ = lut[*p1++];  // set rgb to 0 below threshold and 255 above
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