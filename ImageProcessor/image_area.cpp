#include "image_area.h"
#include "qstring.h"


image_area::image_area(QString path)
{
	m_currentFilePath = path;
	m_previewImage();
	//m_setBrightness_contrast();
	//m_setRgb();
	QStackedLayout* optionbox = new QStackedLayout();
}

image_area::image_area()
{
}

void image_area::m_setImage(QString path)
{
	m_currentFilePath = path;
	m_previewImage();
}

void image_area::m_previewImage(){
	m_image_area->clear();
	m_curr_image.load(m_currentFilePath);
	// now scale image to screen to show
	QPixmap temp;
	int  max_height = this->height();
	int max_width = this->width();

	if (m_curr_image.width() > max_width || m_curr_image.height() > max_height)
		m_image_area->setPixmap(m_curr_image.scaled(max_width, max_height, Qt::KeepAspectRatio));
	
	else
		m_image_area->setPixmap(m_curr_image);
}

void image_area::m_saveImage(QString filename)
{
	m_image_area->pixmap()->save(filename);
}

image_area::~image_area()
{
}
