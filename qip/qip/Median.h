#ifndef MEDIAN_H
#define MEDIAN_H

#include "ImageFilter.h"
#include <deque>
#include <algorithm>

class Median :
	public ImageFilter
{
	Q_OBJECT

public:
		Median	(QWidget *parent = 0);
		QGroupBox*			controlPanel();			// create control panel
		void				reset();		// reset parameters
		bool				applyFilter(ImagePtr, ImagePtr);// apply filter to input to init output
		void				disable(bool);
		uchar				median(uchar* sum, int avg, int size);
		void				get_med(ChannelPtr<uchar> p2, 
									std::deque < uchar* > buffer,
									int sz ,int w, int avg);

protected:
	void		getMedian(ImagePtr I1, int sz, int avg, ImagePtr I2);
protected slots:
	void		changeSlider(int);
	void		changeAvg(int);
private:
	QGroupBox * m_ctrlGrp;
	
	QSlider		*m_sliderSz;		// x size slider
	QSpinBox	*m_spinBoxSz;

	QSlider			*m_sliderAvg;		// x size slider
	QSpinBox		*m_spinBoxAvg;

	uchar* 		getRowBuff(ChannelPtr<uchar> p1, int width, int pad);
};

#endif