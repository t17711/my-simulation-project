#ifndef ERRORDIFFUSION_H
#define ERRORDIFFUSION_H

#include "ImageFilter.h"
class ErrorDiffusion :
	public ImageFilter
{
	Q_OBJECT
public:
	ErrorDiffusion(QWidget * parent =0);
	QGroupBox*	controlPanel();		// create control panel
	bool		applyFilter(ImagePtr, ImagePtr);// apply filter to input to init output

protected:
	void EDiffuse(ImagePtr I1, ImagePtr I2);

private:
	QGroupBox	*m_ctrlGrp;	// Groupbox for panel

};

#endif