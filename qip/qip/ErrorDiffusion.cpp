#include "ErrorDiffusion.h"


ErrorDiffusion::ErrorDiffusion(QWidget *parent) : ImageFilter(parent)
{}

bool
ErrorDiffusion::applyFilter(ImagePtr I1, ImagePtr I2){
	if (I1.isNull()) return 0;
	EDiffuse(I1, I2);
	return 1;
}

QGroupBox*
ErrorDiffusion::controlPanel(){
	m_ctrlGrp = new QGroupBox("ErrorDiffusion");

	return m_ctrlGrp;

}

void
ErrorDiffusion::EDiffuse(ImagePtr I1, ImagePtr I2){
	IP_copyImageHeader(I1, I2);  // copys width height and other properties from i1 to i2
	int w = I1->width();  // input image
	int h = I1->height();
	int total = w * h; // 



}