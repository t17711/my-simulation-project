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

protected slots:
	void			changeFctr(int);	// for spinbox

	void			changeXsz(int);
	void			changeYsz(int);

	void			changeCouple(int);

protected:
	void		getSharp(ImagePtr I1, int xsz,int ysz, int fctr, ImagePtr I2);
private:
	QGroupBox	*m_ctrlGrp;			// groupbox for panel

	QSlider		*m_sliderXsz;		// x size slider
	QSlider		*m_sliderYsz;		// y size  slider
	QSpinBox	*m_spinBoxXsz;
	QSpinBox	*m_spinBoxYsz;

	QSlider			*m_sliderFctr;		// x size slider
	QSpinBox		*m_spinBoxFctr;

	QCheckBox	*m_coupleSZ;

};

#endif