#ifndef HISTOGRAMMATCH_H
#define HISTOGRAMMATCH_H

#include "ImageFilter.h"

class HistogramMatch :
	public ImageFilter
{
	Q_OBJECT

public:
	HistogramMatch(QWidget *parent = 0);
	QGroupBox*		controlPanel();		// create control panel
	void			reset();		// reset parameters

private:
	// widgets and groupbox
	QGroupBox	*m_ctrlGrp;			// groupbox for panel

};
#endif