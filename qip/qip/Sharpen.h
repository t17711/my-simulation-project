#ifndef SHARPEN_H
#define SHARPEN_H

#include "Blur.h"
class Sharpen :
	public Blur
{
	Q_OBJECT

public:
		Sharpen		(QWidget *parent = 0);
		QGroupBox*			controlPanel();			// create control panel
		bool				applyFilter(ImagePtr, ImagePtr);// apply filter to input to init output
protected slots:
	void sharp(int);
protected:
	void		getSharp(ImagePtr I1, ImagePtr I2);
};

#endif