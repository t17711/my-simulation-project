#include "MainWindow.h"
#include "Median.h"

extern MainWindow *g_mainWindowP;

Median::Median(QWidget *parent) : ImageFilter(parent)
{
}

QGroupBox*
Median::controlPanel(){		// create control panel

	m_ctrlGrp = new QGroupBox("Median");

	QLabel *labelSx = new QLabel;
	labelSx->setText(QString("Column Size"));

	QLabel *labelSy = new QLabel;
	labelSy->setText(QString("Row Size"));

	// x neighborhood
	m_sliderXsz = new QSlider(Qt::Horizontal, m_ctrlGrp);
	m_sliderXsz->setTickPosition(QSlider::TicksBelow);
	m_sliderXsz->setTickInterval(1);
	m_sliderXsz->setRange(1, 31); // range of slider - 256 to 256
	m_sliderXsz->setValue(1);

	// spinbox
	m_spinBoxXsz = new QSpinBox(m_ctrlGrp);
	m_spinBoxXsz->setRange(1, 31);

	m_spinBoxXsz->setValue(1);

	// y neighborhood
	m_sliderYsz = new QSlider(Qt::Horizontal, m_ctrlGrp);
	m_sliderYsz->setTickPosition(QSlider::TicksBelow);
	m_sliderYsz->setTickInterval(1);
	m_sliderYsz->setRange(1, 31); // range of slider - 256 to 256
	m_sliderYsz->setValue(1);

	// spinbox
	m_spinBoxYsz = new QSpinBox(m_ctrlGrp);
	m_spinBoxYsz->setRange(1, 31);
	m_spinBoxYsz->setValue(1);

	// avg	
	QLabel *labelA = new QLabel;
	labelA->setText(QString("Average Size"));

	m_sliderAvg = new QSlider(Qt::Horizontal, m_ctrlGrp);
	m_sliderAvg->setTickPosition(QSlider::TicksBelow);
	m_sliderAvg->setTickInterval(1);
	m_sliderAvg->setRange(0, 100); // range of slider - 256 to 256
	m_sliderAvg->setValue(0);
	
	// spinbox
	m_spinBoxAvg = new QSpinBox(m_ctrlGrp);
	m_spinBoxAvg->setRange(0, MXGRAY-1); // range of slider - 256 to 256
	m_spinBoxAvg->setValue(0);

	// comboine
	m_combinexy = new QCheckBox("Combine row and column window", m_ctrlGrp);

	// hist method
	m_HistMedian = new QCheckBox("Use histogram", m_ctrlGrp);

	//clock label
	m_clock = new QTextEdit("CLock: ", m_ctrlGrp);
	m_clock->isReadOnly();
	// 
	// connect signal
	connect(m_sliderXsz, SIGNAL(valueChanged(int)), this, SLOT(changeSliderX(int)));
	connect(m_spinBoxXsz, SIGNAL(valueChanged(int)), this, SLOT(changeSliderX(int)));
	
	connect(m_sliderYsz, SIGNAL(valueChanged(int)), this, SLOT(changeSliderY(int)));
	connect(m_spinBoxYsz, SIGNAL(valueChanged(int)), this, SLOT(changeSliderY(int)));

	connect(m_sliderAvg, SIGNAL(valueChanged(int)), this, SLOT(changeAvg(int)));
	connect(m_spinBoxAvg, SIGNAL(valueChanged(int)), this, SLOT(changeAvg(int)));

	connect(m_combinexy, SIGNAL(stateChanged(int)), this, SLOT(combine(int)));
	connect(m_HistMedian, SIGNAL(stateChanged(int)), this, SLOT(useHist(int)));


	QGridLayout *layout = new QGridLayout;

	layout->addWidget(m_combinexy, 3, 0,1,3); // extend to 3 columns right
	layout->addWidget(m_HistMedian, 4, 0, 1, 3); // extend to 3 columns right

	layout->addWidget(labelSx, 1, 0);
	layout->addWidget(m_sliderXsz, 1, 1);
	layout->addWidget(m_spinBoxXsz, 1, 2);

	layout->addWidget(labelSy, 2, 0);
	layout->addWidget(m_sliderYsz, 2, 1);
	layout->addWidget(m_spinBoxYsz, 2, 2);

	layout->addWidget(labelA, 0, 0);
	layout->addWidget(m_sliderAvg, 0, 1);
	layout->addWidget(m_spinBoxAvg, 0, 2);


	layout->addWidget(m_clock, 5, 0, 3,3);

	m_ctrlGrp->setLayout(layout);
	disable(true);

	return m_ctrlGrp;

}

bool
Median::applyFilter(ImagePtr I1, ImagePtr I2){
	if (I1.isNull()) return 0;
	int xsz= m_sliderXsz->value();
	int ysz = m_sliderYsz->value();

	int avg = m_sliderAvg->value();

	
	getMedian(I1, xsz, ysz, avg, I2);
	
	return 1;
}

void
Median::disable(bool flag){
	m_sliderXsz->setDisabled(flag);
	m_spinBoxXsz->setDisabled(flag);

	m_sliderYsz->setDisabled(flag);
	m_spinBoxYsz->setDisabled(flag);

	m_sliderAvg->setDisabled(flag);
	m_spinBoxAvg->setDisabled(flag);

}

void
Median::reset(){
	m_sliderXsz->blockSignals(true);
	m_spinBoxXsz->blockSignals(true);
	m_sliderYsz->blockSignals(true);
	m_spinBoxYsz->blockSignals(true);
	m_combinexy->blockSignals(true);

	m_sliderAvg->blockSignals(true);
	m_spinBoxAvg->blockSignals(true);

	m_sliderYsz->setValue(1);
	m_spinBoxYsz->setValue(1);

	m_sliderXsz->setValue(1);
	m_spinBoxXsz->setValue(1);

	m_sliderAvg->setValue(0);
	m_spinBoxAvg->setValue(0);

	m_combinexy->setChecked(false);

	m_sliderXsz->blockSignals(false);
	m_spinBoxXsz->blockSignals(false);

	m_sliderYsz->blockSignals(false);
	m_spinBoxYsz->blockSignals(false);

	m_sliderAvg->blockSignals(false);
	m_spinBoxAvg->blockSignals(false);

	m_combinexy->blockSignals(false);

}

void
Median::changeSliderX(int sz){
	if (sz % 2 == 0 && sz > 0){ // maintain odd
		if (sz > m_sliderXsz->value() || sz > m_spinBoxXsz->value())	sz++; // if increasing increase
		else{
			sz--;
		}
	}
	// if checked change all value
	if (m_combinexy->isChecked()){
		m_sliderYsz->blockSignals(true);
		m_spinBoxYsz->blockSignals(true);

		m_sliderYsz->setValue(sz);
		m_spinBoxYsz->setValue(sz);

		m_sliderYsz->blockSignals(false);
		m_spinBoxYsz->blockSignals(false);
	}
	m_sliderXsz->blockSignals(true);
	m_spinBoxXsz->blockSignals(true);

	m_sliderXsz->setValue(sz);
	m_spinBoxXsz->setValue(sz);

	m_sliderXsz->blockSignals(false);
	m_spinBoxXsz->blockSignals(false);


	// apply filter to source image; save result in destination image
	applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

	// display output
	g_mainWindowP->displayOut();

}

void
Median::changeSliderY(int ysz){
	if (ysz % 2 == 0 && ysz > 0){ // maintain odd
		if (ysz > m_sliderYsz->value() || ysz > m_spinBoxYsz->value())	ysz++; // if increasing increase
		else{
			ysz--;
		}
	}
	// check if coupled
	if (m_combinexy->isChecked()){
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
Median::combine(int){
	if (m_combinexy->isChecked()){
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
Median::useHist(int){
	if (m_HistMedian->isChecked()) {
		m_sliderAvg->setDisabled(true);
		m_spinBoxAvg->setDisabled(true);
	}
	else{
		m_sliderAvg->setDisabled(false);
		m_spinBoxAvg->setDisabled(false);
	}
	// apply filter to source image; save result in destination image
	applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

	// display output
	g_mainWindowP->displayOut();

}

void
Median::changeAvg(int sz){

	m_sliderAvg->blockSignals(true);
	m_spinBoxAvg->blockSignals(true);

	m_sliderAvg->setValue(sz);
	m_spinBoxAvg->setValue(sz);

	m_sliderAvg->blockSignals(false);
	m_spinBoxAvg->blockSignals(false);


	// apply filter to source image; save result in destination image
	applyFilter(g_mainWindowP->imageSrc(), g_mainWindowP->imageDst());

	// display output
	g_mainWindowP->displayOut();

}

uchar*
Median::getRowBuff(ChannelPtr<uchar> p1, int width, int pad){
	uchar* temp = (uchar*)malloc(sizeof(uchar*)*(width + 2 * pad));
	if (temp == NULL) exit(1);

	// copy leftmost pixel left pad
	int i = 0;
	while( i < pad){
		temp[i]=*p1;
		++i;
	}

	// copy row
	width += i;
	while( i < width){
		temp[i]=(*p1++);
		++i;
	}

	// copy rightmost pixel for right pad
	pad += i;
	while (i < pad){
		temp[i]=(*p1);
		++i;
	}

	return temp;
}

void
Median::getMedian(ImagePtr I1, int xsz, int ysz, int avg, ImagePtr I2){
	clock_t t;
	t = clock();

	IP_copyImageHeader(I1, I2);  // copys width height and other properties from i1 to i2

	int w = I1->width();  // input image
	int h = I1->height();
	int total = w * h; // 

	int type;
	ChannelPtr<uchar> p1, p2, endd,endd2;

	std::deque<uchar*> buffer;

	int xpad = (xsz) / 2; // padding for left and right of w
	int ypad = (ysz) / 2; // padding for top and bottom


	for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
		IP_getChannel(I2, ch, p2, type); // gets channle 0 1 or 2 (r, g ,b) array 
		endd = p1 + total;
		endd2 = p2 + total;

		// create top pad 
		for (int i = 0; i < ypad; ++i){
			
			//xpad is number of pixels to be added to left and right of w 
			buffer.push_back(getRowBuff(p1, w, xpad));
		}
		// add 1st ysz - ypad  rows
		// i used ysz since it determines neighborhood size of column
		for (int i = 0; i < ysz - ypad; ++i){
			buffer.push_back(getRowBuff(p1, w, xpad));
			p1 += w;
		}
		// traverse 
		// do until last  row
	//	int end_l = total - (sz - pad)*w;
		for (; p1 < endd; p1 += w, p2 += w) {
			// get median and put it on p2
			get_med(p2, buffer, xsz, ysz, w, avg);	
			free(buffer.front());
			buffer.pop_front();
			buffer.push_back(getRowBuff(p1, w, xpad));
		}
		///////
		//for end
		////
		
		// add last rows
		
		// since i already copied last pixel it has gone over it so go back
		p1 -= w;  

		// get median and pop out values
		// need to do for last + pad row
		do{
			get_med(p2, buffer, xsz, ysz, w, avg);
			p2 += w;
			free(buffer.front());
			buffer.pop_front();
			buffer.push_back(getRowBuff(p1, w, xpad));
		} while (p2 < endd2);
			

	}
	for (auto i : buffer){
		free(i);
	}

	t = clock() - t;
	
	QString c = m_clock->toPlainText();

	if (m_HistMedian->isChecked()){
		m_clock->setText(c + " \n Hist time is " + QString::number((float)t));
	}
	else{
		m_clock->setText(c + " \n SORT time is " + QString::number((float)t));

	}

}

uchar
Median::median(uchar*  sum, int avg, int size){
	int med=0;

	if (avg > size) avg = size;

	//std::sort(sum, sum+size);

	// counting sort
	int count[MXGRAY] = {0}; //Can store the count of 255 positive numbers

	// get hist
	for (int i = 0; i<size; i++)
		count[sum[i]]++;

	// get cummulative hist
	for (int i = 1; i<MXGRAY; i++)
		count[i] += count[i - 1];

	// sorted array
	int *output = (int*)malloc(sizeof(int*)*size);

	for (int i = 0; i<size; i++)
	{
		int ind = count[sum[i]] - 1;
		*(output + ind) = sum[i];
		count[sum[i]]--;
	}
	

	// get # of median to get
	for (int i = (size/2)-avg , end = i+ 2*avg+1; i != end; ++i){
		med += output[i];
	}


	free(output);

	return med / (2 * avg + 1);
}

void
Median::get_med(ChannelPtr<uchar> p2,
std::deque <uchar* > buffer,
int xsz, int ysz, int w, int avg){
	if (m_HistMedian->isChecked()){
		get_med_Hist(p2, buffer, xsz, ysz, w);
		
		return;
	}

	int size = 0;
	uchar* sum;
	sum = (uchar*)malloc(xsz*ysz);
	size_t si = buffer.size();

	// there are w neighborhood in a buffer
	for (int i = 0; i < w; i++){

		// for each rows in buffer copy values
		for (int j = 0; j < si; j++){

			// so iterate theough buffer
			int  k = i;

			// copy values from i to i + xsz of each row
			int nxt = i + xsz;

			for (; k < nxt; k++){
				sum[size] = (buffer.at(j)[k]);
				size++;
			}

		}

		// get median value and set output value, increment pointer
		int m = median(sum, avg, size);
		*p2 = m;
		p2++;
		size = 0;
	}
}

void
Median::get_med_Hist(ChannelPtr<uchar> p2,
	std::deque <uchar* > buffer,
	int xsz, int ysz, int w){

	int med = xsz*ysz / 2 + 1 ; // median of 3 is 3/2 +1  = 1+1 =2
	int m = 0;// = median(sum, avg, size);


	int sum[MXGRAY] = { 0 };

	// 1st get hist of 1st neighbor hood
	for (int j = 0; j < ysz; j++){

		// so iterate theough buffer
		
		// get histogram of 1st xsz items of each item in buffer
		for (int k = 0; k < xsz; k++){
			sum[buffer[j][k]]++;
		}
	}

	// get median value and set output value, increment pointer
	m = sum[0];
	int k;
	for (k = 1; m < med && k < MXGRAY; ++k){
		m += sum[k];
	}
	*p2 = k;
	k = 0;
	p2++;

	// there are w neighborhood in a buffer
	for (int i = 0; i < w -1 ; i++){

		
		// remove leftmost item from hist
		for (int b = 0; b < ysz; ++b){
			// delete last item
			sum[buffer[b][i]]--;
			// add new item
			sum[buffer[b][i+xsz]]++;
		}
		
		// get median value and set output value, increment pointer

		m = sum[0];
		for (k = 1; m < med && k < MXGRAY; ++k){
			m += sum[k];
		}

		*p2 = k;
		p2++;
	}

}