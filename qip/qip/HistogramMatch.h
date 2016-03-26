#ifndef HISTOGRAMMATCH_H
#define HISTOGRAMMATCH_H

#include "ImageFilter.h"

class HistogramMatch :
	public ImageFilter
{
	Q_OBJECT

public:
						HistogramMatch	(QWidget *parent = 0);
	QGroupBox*			controlPanel	();		// create control panel
	void				reset			();		// reset parameters
	void				disable			(bool);
	bool				applyFilter		(ImagePtr, ImagePtr);// apply filter to input to init output

protected slots:
	void				changeHist		(int);
	void				changeHistSpin	(int);

protected:
	void				getHistogram	(ImagePtr, int[]);
	void				HistMatch(ImagePtr, long double[], ImagePtr);

private:
	// widgets and groupbox
	QGroupBox				*m_ctrlGrp;			// groupbox for panel
	QSlider					*m_SliderHistMatch;
	QSpinBox				*m_SpinBoxHistMatch;


};
#endif