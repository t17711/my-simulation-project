#ifndef SHARPEN_H
#define SHARPEN_H

#include "Blur.h"
class Sharpen :
	public Blur
{
	Q_OBJECT

public:
	// virtual classes
		Sharpen		(QWidget *parent = 0);
		QGroupBox*			controlPanel();			// create control panel
		bool				applyFilter(ImagePtr, ImagePtr);// apply filter to input to init output
		void				disable(bool);
		void				reset();

// slots for spinboxes
protected slots:
	void			changeFctr(int);	// function that occurs when factor spinbox or slider value chjanges
	void			changeXsz(int);		// for row neighborhood size change
	void			changeYsz(int);		// for column neighborhood size change
	void			changeCouple(int);	// this combines x and y

// this is the main function to get sharp
protected:
	void		getSharp(ImagePtr I1, int xsz,int ysz, int fctr, ImagePtr I2);

private:
	QGroupBox	*m_ctrlGrp;			// groupbox for panel
	QSlider		*m_sliderXsz;		// row size slider
	QSlider		*m_sliderYsz;		// column size  slider
	QSpinBox	*m_spinBoxXsz;		// x spinbox
	QSpinBox	*m_spinBoxYsz;
	QSlider		*m_sliderFctr;		// factor slider
	QSpinBox	*m_spinBoxFctr;
	QCheckBox	*m_coupleSZ;

};

#endif