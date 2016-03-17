#ifndef HISTOGRAMEEQUALIZE_H
#define HISTOGRAMEEQUALIZE_H

#include "ImageFilter.h"

class HistogramEqualize :
	public ImageFilter
{
public:
	HistogramEqualize	(QWidget *parent = 0);
	QGroupBox*			controlPanel();			// create control panel
	void				reset();		// reset parameters

private:
	// widgets and groupbox
	QGroupBox	*m_ctrlGrp;			// groupbox for panel

};

#endif