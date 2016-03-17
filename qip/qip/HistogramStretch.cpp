#include "HistogramStretch.h"
#include "MainWindow.h"




HistogramStretch::
HistogramStretch(QWidget *parent) : ImageFilter(parent)
{
}

QGroupBox*
HistogramStretch::controlPanel()
{
	// init group box
	m_ctrlGrp = new QGroupBox("Histogram Stretching");
	// create minimum slider

	// for minimum
	QLabel *label_min = new QLabel(tr("Minimum"));

	m_sliderMin = new QSlider(Qt::Horizontal, m_ctrlGrp);
	m_sliderMin->setTickPosition(QSlider::TicksBelow);
	m_sliderMin->setTickInterval(25);
	m_sliderMin->setMinimum(0);
	m_sliderMin->setMaximum(MXGRAY - 5); /// max of max is mxgray so i have 5 difference on both
	m_sliderMin->setValue(0); // min is 0

	m_spinBoxMin = new QSpinBox(m_ctrlGrp);
	m_spinBoxMin->setMinimum(0);
	m_spinBoxMin->setMaximum(MXGRAY-5);
	m_spinBoxMin->setValue(0);

	m_CheckBoxAutoMin= new QCheckBox(m_ctrlGrp);
	QLabel *label_min_auto = new QLabel(tr("auto"));


	// for maximum
	QLabel *label_max = new QLabel(tr("Minimum"));

	m_sliderMax = new QSlider(Qt::Horizontal, m_ctrlGrp);
	m_sliderMax->setTickPosition(QSlider::TicksBelow);
	m_sliderMax->setTickInterval(25);
	m_sliderMax->setMinimum(0);
	m_sliderMax->setMaximum(MXGRAY - 5); /// max of max is mxgray so i have 5 difference on both
	m_sliderMax->setValue(0); // min is 0

	m_spinBoxMax = new QSpinBox(m_ctrlGrp);
	m_spinBoxMax->setMinimum(0);
	m_spinBoxMax->setMaximum(MXGRAY - 5);
	m_spinBoxMax->setValue(0);

	m_CheckBoxAutoMax = new QCheckBox(m_ctrlGrp);
	QLabel *label_max_auto = new QLabel(tr("auto"));

	// layout
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(label_min,			0, 0);
	layout->addWidget(m_sliderMin,			1, 0);
	layout->addWidget(m_spinBoxMin,			1, 1);
	layout->addWidget(m_CheckBoxAutoMin,	1, 2, Qt::AlignRight);
	layout->addWidget(label_min_auto,		1, 3, Qt::AlignLeft);

	layout->addWidget(label_max,			2, 0);
	layout->addWidget(m_sliderMax,			3, 0);
	layout->addWidget(m_spinBoxMax, 3, 1);
	layout->addWidget(m_CheckBoxAutoMax, 3, 2, Qt::AlignRight);
	layout->addWidget(label_max_auto, 3, 3, Qt::AlignLeft);

	m_ctrlGrp->setLayout(layout);
	// connect signal here

	return(m_ctrlGrp);
}

void
HistogramStretch::reset(){

}