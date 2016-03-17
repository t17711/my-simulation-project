#ifndef HISTOGRAMSTRETCH_H
#define HISTOGRAMSTRETCH_H

#include "ImageFilter.h"

class HistogramStretch :
	public ImageFilter
{
public:
	HistogramStretch(QWidget *parent = 0);
	QGroupBox*			controlPanel();			// create control panel
	void				reset();		// reset parameters

private:
	// widgets and groupbox
	QGroupBox	*m_ctrlGrp;			// groupbox for panel
	QSlider		*m_sliderMin;		// minimum slider
	QSlider		*m_sliderMax;		// max slider
	QSpinBox	*m_spinBoxMin;
	QSpinBox	*m_spinBoxMax;

	QCheckBox	*m_CheckBoxAutoMin;			// auto min check box
	QCheckBox	*m_CheckBoxAutoMax; 		// auto min check box

};

#endif