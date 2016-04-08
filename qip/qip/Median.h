#ifndef MEDIAN_H
#define MEDIAN_H

#include "ImageFilter.h"
class Median :
	public ImageFilter
{
	Q_OBJECT

public:
		Median	(QWidget *parent = 0);
//		QGroupBox*			controlPanel();			// create control panel
	//	void				reset();		// reset parameters
		//bool				applyFilter(ImagePtr, ImagePtr);// apply filter to input to init output
		//void				disable(bool);

};

#endif