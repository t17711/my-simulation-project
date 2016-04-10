#include "MainWindow.h"
#include "Median.h"

extern MainWindow *g_mainWindowP;

Median::Median(QWidget *parent) : ImageFilter(parent)
{
}

QGroupBox*
Median::controlPanel(){		// create control panel

	m_ctrlGrp = new QGroupBox("Median");

	QLabel *labelS = new QLabel;
	labelS->setText(QString("Window Size"));

	m_sliderSz = new QSlider(Qt::Horizontal, m_ctrlGrp);
	m_sliderSz->setTickPosition(QSlider::TicksBelow);
	m_sliderSz->setTickInterval(10);
	m_sliderSz->setRange(1, MXGRAY); // range of slider - 256 to 256
	m_sliderSz->setValue(1);

	// spinbox
	m_spinBoxSz = new QSpinBox(m_ctrlGrp);
	m_spinBoxSz->setRange(1, MXGRAY);
	m_spinBoxSz->setValue(1);

	// avg	
	QLabel *labelA = new QLabel;
	labelA->setText(QString("Average Size"));

	m_sliderAvg = new QSlider(Qt::Horizontal, m_ctrlGrp);
	m_sliderAvg->setTickPosition(QSlider::TicksBelow);
	m_sliderAvg->setTickInterval(10);
	m_sliderAvg->setRange(0, MXGRAY - 1); // range of slider - 256 to 256
	m_sliderAvg->setValue(0);
	// spinbox
	m_spinBoxAvg = new QSpinBox(m_ctrlGrp);
	m_spinBoxAvg->setRange(0, MXGRAY-1); // range of slider - 256 to 256
	m_spinBoxAvg->setValue(0);

	connect(m_spinBoxSz, SIGNAL(valueChanged(int)), this, SLOT(changeSlider(int)));
	connect(m_sliderSz, SIGNAL(valueChanged(int)), this, SLOT(changeSlider(int)));

	connect(m_sliderAvg, SIGNAL(valueChanged(int)), this, SLOT(changeAvg(int)));
	connect(m_spinBoxAvg, SIGNAL(valueChanged(int)), this, SLOT(changeAvg(int)));
	
	
	QGridLayout *layout = new QGridLayout;

	layout->addWidget(labelS, 1, 0);
	layout->addWidget(m_sliderSz, 1, 1);
	layout->addWidget(m_spinBoxSz, 1, 2);

	layout->addWidget(labelA, 0, 0);
	layout->addWidget(m_sliderAvg, 0, 1);
	layout->addWidget(m_spinBoxAvg, 0, 2);

	m_ctrlGrp->setLayout(layout);
	disable(true);

	return m_ctrlGrp;

}

bool
Median::applyFilter(ImagePtr I1, ImagePtr I2){
	if (I1.isNull()) return 0;
	int sz= m_sliderSz->value();
	int avg = m_sliderAvg->value();

	getMedian(I1,sz, avg, I2);
	return 1;
}

void
Median::disable(bool flag){
	m_sliderSz->setDisabled(flag);
	m_spinBoxSz->setDisabled(flag);
	m_sliderAvg->setDisabled(flag);
	m_spinBoxAvg->setDisabled(flag);

}

void
Median::reset(){
	m_sliderSz->blockSignals(true);
	m_spinBoxSz->blockSignals(true);
	m_sliderAvg->blockSignals(true);
	m_spinBoxAvg->blockSignals(true);

	m_sliderSz->setValue(1);
	m_spinBoxSz->setValue(1);
	m_sliderAvg->setValue(0);
	m_spinBoxAvg->setValue(0);

	m_sliderSz->blockSignals(false);
	m_spinBoxSz->blockSignals(false);

	m_sliderAvg->blockSignals(false);
	m_spinBoxAvg->blockSignals(false);
}

void
Median::changeSlider(int sz){
	if (sz % 2 == 0 && sz > 0){ // maintain odd
		if (sz > m_sliderSz->value() || sz > m_spinBoxSz->value())	sz++; // if increasing increase
		else{
			sz--;
		}
	}
	m_sliderSz->blockSignals(true);
	m_spinBoxSz->blockSignals(true);

	m_sliderSz->setValue(sz);
	m_spinBoxSz->setValue(sz);

	m_sliderSz->blockSignals(false);
	m_spinBoxSz->blockSignals(false);


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

std::vector<uchar>
Median::getRowBuff(ChannelPtr<uchar> p1, int width, int pad){
	std::vector<uchar> temp;
	for (int i = 0; i < pad; ++i){
		temp.push_back(*p1);
	}
	for (int i = 0; i < width; ++i){
		temp.push_back(*p1++);
	}
	for (int i = 0; i < pad; ++i){
		temp.push_back(*p1);
	}

	return temp;
}

void
Median::getMedian(ImagePtr I1, int sz, int avg, ImagePtr I2){
	IP_copyImageHeader(I1, I2);  // copys width height and other properties from i1 to i2

	int w = I1->width();  // input image
	int h = I1->height();
	int total = w * h; // 

	int type;
	ChannelPtr<uchar> p1, p2, endd;

	std::deque<std::vector<uchar> > buffer;

	int pad = (sz) / 2;


	for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
		IP_getChannel(I2, ch, p2, type); // gets channle 0 1 or 2 (r, g ,b) array 
		endd = p1 + total;

		// create top pad 
		for (int i = 0; i < pad; ++i){
			buffer.push_back(getRowBuff(p1, w, pad));
		}
		// add 1st few rows
		for (int i = 0; i < sz - pad; ++i){
			buffer.push_back(getRowBuff(p1, w, pad));
			p1 += w;
		}
		// traverse 
		// do until last  row
	//	int end_l = total - (sz - pad)*w;
		for (; p1 < endd; p1 += w, p2 += w) {
			// get median and put it on p2
			get_med(p2, buffer, sz, w, avg);		
			buffer.pop_front();
			buffer.push_back(getRowBuff(p1, w, pad));
		}
		///////
		//for end
		////
		
		// add 1st few rows
		// add last rows
		p1 -= w;
		for (int i = 0; i < pad; ++i){
			get_med(p2, buffer, sz , w , avg);
			p2+=w;
			buffer.pop_front();
			buffer.push_back(getRowBuff(p1, w, pad));		
		}

	}

}
uchar
Median::median(std::vector<uchar> sum, int avg){
	int size = sum.size();
	int med=0;

//	avg = avg * 2 + 1; // gets nearest avg items
	if (avg > size) avg = size;

	std::sort(sum.begin(), sum.end());

	// get # of median to get
	for (int i = (size/2)-avg , end = i+ 2*avg+1; i != end; ++i){
		med += sum[i];
	}
	int o = med / (2 * avg + 1);
	return o;
}

void
Median::get_med(ChannelPtr<uchar> p2,
				std::deque <std::vector<uchar> > buffer,
				int sz, int w , int avg){

		std::vector<uchar> sum(sz*sz);
		size_t si = buffer.size();
		// sum each neighbor
		for (int i = 0; i < w; i++){
			for (int j = 0; j < si; j++){
				// sum each row
				int  k = i;
				int nxt = i + sz;
			
				for (; k < nxt; k++){
					sum.push_back(buffer.at(j)[k]);
				}
				
			}
			int m = CLIP(median(sum, avg), 0, MXGRAY);
			*p2 = m;
			p2++;
			sum.clear();
		}
}
