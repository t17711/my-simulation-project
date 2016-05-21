#ifndef BLUR_H
#define BLUR_H

#include "ImageFilter.h"
class Blur :
	public ImageFilter
{
	Q_OBJECT

public:
	// virtual classes
			Blur	(QWidget *parent = 0);
			QGroupBox*			controlPanel	();			// create control panel
			void				reset			();		// reset parameters
			bool				applyFilter		(ImagePtr, ImagePtr);// apply filter to input to init output
			void				disable			(bool);
protected:

	// widgets and groupbox
	QGroupBox	*m_ctrlGrp;			// groupbox for panel
	QSlider		*m_sliderXsz;		// x (row) size slider
	QSlider		*m_sliderYsz;		// y (column) size  slider
	QSpinBox	*m_spinBoxXsz;		// x size spinbox
	QSpinBox	*m_spinBoxYsz;
	QCheckBox	*m_coupleSZ;

// functions for blurring
	void		getBlur(ImagePtr I1, int xsz, int ysz, ImagePtr I2);
	void		getBlur_1D(IP::ChannelPtr<uchar>  p1, int width, int steps, int size, IP::ChannelPtr<uchar> p2);

// slots for spinboxes
protected slots:
	void		changeXsz		(int);
	void		changeYsz		(int);
	void		changeCouple	(int);

};

#endif