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
									int xsz ,int ysz, int w, int avg);

protected:
	void		getMedian(ImagePtr I1, int xsz,int ysz, int avg, ImagePtr I2);
protected slots:
	void		changeSliderX(int);
	void		changeSliderY(int);

	void		changeAvg(int);
	void		combine(int);
private:
	QGroupBox * m_ctrlGrp;
	
	QSlider		*m_sliderXsz;		// x size slider
	QSpinBox	*m_spinBoxXsz;

	QSlider		*m_sliderYsz;		// x size slider
	QSpinBox	*m_spinBoxYsz;

	QSlider			*m_sliderAvg;		// x size slider
	QSpinBox		*m_spinBoxAvg;

	QCheckBox		*m_combinexy;

	uchar* 		getRowBuff(ChannelPtr<uchar> p1, int width, int pad);
};

#endif