#include "MainWindow.h"
#include "HistogramMatch.h"


HistogramMatch::
HistogramMatch(QWidget *parent) : ImageFilter(parent)
{
}

QGroupBox*
HistogramMatch::controlPanel()
{
	// init group box
	m_ctrlGrp = new QGroupBox("");
	return(m_ctrlGrp);
}

void
HistogramMatch::reset(){

}