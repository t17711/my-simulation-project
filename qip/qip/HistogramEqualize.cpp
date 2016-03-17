#include "HistogramEqualize.h"
#include "MainWindow.h"



HistogramEqualize::
	HistogramEqualize (QWidget *parent) : ImageFilter(parent)
{
}

QGroupBox*
HistogramEqualize::controlPanel()
{
	// init group box
	m_ctrlGrp = new QGroupBox("");
	return(m_ctrlGrp);
}

void
HistogramEqualize::reset(){

}
