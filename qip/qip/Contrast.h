// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// Contrast.h - Contrast widget
//
// Written by: George Wolberg, 2016
// ======================================================================

#ifndef CONTRAST_H
#define CONTRAST_H

#include "ImageFilter.h"
#include "qboxlayout.h"

class Contrast : public ImageFilter {
	Q_OBJECT

public:
	Contrast	(QWidget *parent = 0);		// constructor
	QGroupBox*	controlPanel	();		// create control panel
	bool		applyFilter(ImagePtr, ImagePtr);// apply filter to input to init output
	void		reset		();		// reset parameters
	void		disable(bool);


protected:
	void contrast(ImagePtr I1, double brightness, double contrast, ImagePtr I2);

protected slots:
	void		changeBright	(int);
	void		changeContrast	(int);
	void		changeContrast	(double);


private:
	// brightness/contrast controls
	QSlider			*m_sliderB ;	// brightness slider
	QSlider			*m_sliderC ;	// contrast   slider
	QSpinBox		*m_spinBoxB;	// brightness spin box
	QDoubleSpinBox	*m_spinBoxC;	// contrast   spin box

	// widgets and groupbox
	QGroupBox		*m_ctrlGrp;	// groupbox for panel
	// calculate contrast
	double			getContrast				(double);
	int				get_Contrast_to_range	(double);
	int				reference				 = 128;
};

#endif	// CONTRAST_H
