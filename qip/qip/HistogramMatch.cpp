#include "HistogramMatch.h"
#include "MainWindow.h"
#include <math.h>       /* ceil */

extern MainWindow *g_mainWindowP;

// constructor
HistogramMatch::
HistogramMatch(QWidget *parent) : ImageFilter(parent)
{
}

////////////////////////////////////////////////////////////////////////////////////////////
/*virtual functions first*/
///////////////////////////////////////////////////////////

/* add slider and spinbox*/
QGroupBox*
HistogramMatch::controlPanel(){
	m_ctrlGrp = new QGroupBox("Histogram Match");
	// create objects
	m_SliderHistMatch = new QSlider				(Qt::Horizontal,m_ctrlGrp);
	m_SpinBoxHistMatch	 = new QSpinBox							(m_ctrlGrp);
	
	QLabel *label1			 = new QLabel			(tr("Histo Match"));
	// set range
	m_SliderHistMatch	->setRange		(-255, 255);
	m_SpinBoxHistMatch	->setRange		(-255, 255);

	m_SliderHistMatch->setTickInterval(1);

	// put them on layout
	QGridLayout *layout		= new QGridLayout;
	layout				->addWidget	(label1,			0, 0);
	layout				->addWidget	(m_SliderHistMatch, 0, 1);
	layout				->addWidget	(m_SpinBoxHistMatch,0, 2, Qt::AlignLeft);

	// set actions
	connect(m_SliderHistMatch,	SIGNAL(valueChanged(int)),		this, SLOT(changeHist		(int)));
	connect(m_SpinBoxHistMatch, SIGNAL(valueChanged(int)),		this, SLOT(changeHistSpin	(int)));
	// set layout to groupbox
	m_ctrlGrp			->setLayout	(layout);

	disable(true);

	return(m_ctrlGrp);
}

///////////////////////////////////////////////////////////////////////////
/* read slider value and perform the operation*/
/*creates a histogram based on slider value
firse it converts slider value to 0 to 1 range then creates a double histogram*/

bool
HistogramMatch::applyFilter(ImagePtr I1, ImagePtr I2){
	if (I1.isNull()) return 0;
	// create histogram and send to match
	long double histogram_target[MXGRAY]; // long double so that lower valuse dont always go to 0
	int scale = m_SliderHistMatch->value();	
	double temp = 0.0;  // long double so that lower valuse dont always go to 0

	if (scale >= 0) {
		// for 0 or greater slider value(v) histogram values v2 = (i/(MXGRAY-1))^v
		for (int i = 0; i < MXGRAY; ++i) {
			temp = pow((double)i / (MXGRAY - 1), scale);
			histogram_target[i] = temp;
		}
	} else {
		// for less than 0 slider value(v) histogram values v2 = 1 - (i/(MXGRAY-1))^|v|
		for (int i = 0; i < MXGRAY; ++i) {
			temp = (double)i / (MXGRAY - 1);
			temp = pow(temp, - scale);
			histogram_target[i] = 1 - temp;
		}
	}

	HistMatch(I1, histogram_target, I2);
	return 1;
}

///////////////////////////////////////////////////////////////////////////
/* set value of slider to 0*/
void
HistogramMatch::reset(){

	m_SliderHistMatch->setValue(0);

}

// disable slider and spinbox if true, enable if false
void
HistogramMatch::disable(bool flag){
	m_SliderHistMatch->blockSignals(true);
	m_SpinBoxHistMatch->blockSignals(true);

	m_SliderHistMatch	->setDisabled(flag);
	m_SpinBoxHistMatch	->setDisabled(flag);

	m_SliderHistMatch->blockSignals(false);
	m_SpinBoxHistMatch->blockSignals(false);

}

///////////////////////////////////////////////////////////////////////////////////////
/* slot functions*/
///////////////////////////////////////////////////////////////////////////////

// this is called by the slider signal  to call apply filter
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
// this is called by spibox to apply filter 

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

///////////////////////////////////////////////////////////////////////////////////////
/* filter functions*/
///////////////////////////////////////////////////////////////////////////////

// main function that matches the histogram
/* this takes the line function from apply filter then converts it to a pixel hoistpogram, then it allocates pixels to 
left and right of bins. then i check if the pixel is filled in left based on target histogram and if it is i go to right bin
i also check if it exceeds the left reserve. */
void 
HistogramMatch::HistMatch(ImagePtr I1, long double target_Histogram[], ImagePtr I2){

	IP_copyImageHeader(I1, I2);  // copys width height and other properties from i1 to i2
	int w = I1->width();  // input image
	int h = I1->height();
	int total = w * h; // 

	int histogram_input[MXGRAY]; // image histogram
	int histogram_target[MXGRAY];

	int left[MXGRAY];
	int right[MXGRAY];
	int reserveLeft[MXGRAY];
	
	// this function calculates histogram of image
	getHistogram(I1, histogram_input);

	double Havg; // find level to equalize
	/* normalize h2 to conform with dimensions of I1 */
	for (int i = Havg = 0; i<MXGRAY; i++) Havg += target_Histogram[i];

	double scale = (double)total / Havg;

	// get integer histogram to match our input image histogram to, if scale is 0 then we dont have to
	if ((int)scale != 1){
		for (int i = 0; i < MXGRAY; i++) histogram_target[i] = (int)round(target_Histogram[i] * scale);
	}
	int R = 0;
	int Hsum = 0;
	int amount_to_fill = 0;

	/* normalize h2 to conform with dimensions of I1 */
	//create temporary left to track overflow later
	int left2[MXGRAY];

	/// tis is to get left reserve for pixel frequency. so this shows how much pixel i can add in histogram
	for (int i = 0; i<MXGRAY; i++) {
		left[i] = R; /* left end of interval */
		left2[i] = R; /* left end of interval */
		amount_to_fill = histogram_target[R] - Hsum;

		// now reserve space for max amount of pixel i to put in leftmost space of histogram
		reserveLeft[i] = (amount_to_fill > histogram_input[i]) ? 0 : amount_to_fill; // max amount of i on left

		Hsum += histogram_input[i]; /* cum. interval value */
		while (Hsum>histogram_target[R] && (MXGRAY - 1)>R) { /* make interval wider */
			Hsum -= histogram_target[R]; /* adjust Hsum */
			R++; /* update right end */
		}
		right[i] = R;
	}

	// create temporary histogram
	for (int i = 0; i<MXGRAY; i++) histogram_input[i] = 0;

	int type;
	ChannelPtr<uchar> p1, p2, endd;

	for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
		IP_getChannel(I2, ch, p2, type); // gets channle 0 1 or 2 (r, g ,b) array 
		for (endd = p1 + total; p1 < endd;) {
			int p = left[*p1];
			
			//now compare how much of value can be added to the slot by comparing reserved place
			if (histogram_input[p] < histogram_target[p]){
				
				// if histogram output  value is less than avg then copy point
				// but 1st check if it is reserved space
				if (left2[*p1] != p) *p2 = p;
				
				// so check if left[*p1] is original left 
				else {
				
					//if it is so then check the reserved value, if
					if (reserveLeft[*p1] > 0){
						reserveLeft[*p1]--;
						*p2 = p;
					}
			
					//	reserved value is 0, then move to next place 
					else{
						left[*p1] = (p + 1 < right[*p1]) ? p + 1 : right[*p1];
						p = left[*p1];
						*p2 = p;
					}
				}
			}

			// if histogram target is met then simply set the left to be the tight bin unless it is already right
			else{
				left[*p1] = (p + 1 < right[*p1]) ? p + 1 : right[*p1];
				p = left[*p1];
				*p2 = p;
			}

			// fill output histogram and assigh value to output image
			histogram_input[p]++;
			*p1++;
			*p2++;
		}
	}
}

// get a histogram from a image ptr
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
