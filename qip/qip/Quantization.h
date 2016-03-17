#ifndef QUANTIZATION_H
#define QUANTIZATION_H

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
protected:
	void quantization(ImagePtr I1, int level,bool dither, ImagePtr I2);

protected slots:
	void changeQnt(int);
	void changeDither(int);

private:
	// threshold controls
	QSlider		*m_slider;	// Threshold sliders
	QSpinBox	*m_spinBox;	// Threshold spin boxes

	// label for Otsu thresholds
	QLabel		*m_label;	// Label for printing out quantization level

	// widgets and groupbox
	QGroupBox	*m_ctrlGrp;	// Groupbox for panel

	// checkbox for dither
	QCheckBox	*m_checkBox;
};

#endif	//QUANTIZATION_H
