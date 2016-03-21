#ifndef HISTOGRAMEEQUALIZE_H
#define HISTOGRAMEEQUALIZE_H

#include "ImageFilter.h"

class HistogramEqualize :public ImageFilter
{
	Q_OBJECT

public:
						HistogramEqualize	(QWidget *parent = 0);
	QGroupBox*			controlPanel		();						// create control panel
	void				reset				();						// reset parameters
	void				disable				(bool);
	void				getHistogram		(ImagePtr I, int histogram[]);
	bool				applyFilter(ImagePtr, ImagePtr);// apply filter to input to init output

protected slots:
	void				cumulativeHist		(int);
	void				maxEntropyHist		(int);


private:
	// widgets and groupbox
	QGroupBox	*m_ctrlGrp;				// groupbox for panel
	QCheckBox	*m_CheckBoxCummulative;	// auto min check box
	QCheckBox	*m_CheckBoxEntropy;		// auto min check box
};

#endif