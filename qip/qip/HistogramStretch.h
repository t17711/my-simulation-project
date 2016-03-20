#ifndef HISTOGRAMSTRETCH_H
#define HISTOGRAMSTRETCH_H

#include "ImageFilter.h"

class HistogramStretch :public ImageFilter {
	Q_OBJECT
public:
	HistogramStretch(QWidget *parent = 0);
	QGroupBox*			controlPanel();			// create control panel
	void				reset();		// reset parameters
	bool		applyFilter(ImagePtr, ImagePtr);// apply filter to input to init output
	void		disable(bool);
	int			getMin();
protected:
	void histogramStretch(ImagePtr I1, int min, int max, ImagePtr I2);

protected slots:
	void changeMin(int);
	void changeMax(int);
	void changeMinAuto(int);
	void changeMaxAuto(int);

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