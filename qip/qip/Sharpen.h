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
		void				disable(bool);
		void				reset();
		double				getFctr(double);

protected slots:
	void changeFctrD(double);  // for slider
	void changeFctr(int);	// for spinbox
	void changeSlider(int);
protected:
	void		getSharp(ImagePtr I1, int sz, double fctr, ImagePtr I2);
private:
	QGroupBox	*m_ctrlGrp;			// groupbox for panel
	
	QSlider		*m_sliderSz;		// x size slider
	QSpinBox	*m_spinBoxSz;

	QSlider			*m_sliderFctr;		// x size slider
	QDoubleSpinBox	*m_spinBoxFctr;


};

#endif