#pragma once

#ifndef THRESHOLD_H
#define THRESHOLD_H


#include "ImageFilter.h"
class Quantization :
	public ImageFilter
{
	Q_OBJECT

public:
	Quantization(QWidget *parent = 0);		// constructor
	QGroupBox*	controlPanel();		// create control panel
	bool		applyFilter(ImagePtr, ImagePtr);// apply filter to input to init output
	void		reset();		// reset parameters
	void		disable(bool);
};

#endif	
