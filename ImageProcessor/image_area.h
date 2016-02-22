#pragma once

#include "qwidget.h"
#include <qlabel.h>
#include <qpixmap.h>
#include <qstring.h>
#include "qgroupbox.h"
#include "qstackedlayout.h"

class image_area :
	public QWidget
{
public:
	image_area();
	image_area(QString path);
	~image_area();

	void				m_setImage(QString path);
	void				m_saveImage(QString filename);
	void				m_setImagePath(QString path);
	void				m_setBrightness_contrast();
	void				m_setRgb();
	

	void				m_previewImage();
	

private:
	QPixmap				m_curr_image;
	QLabel*				m_image_area = new QLabel("image", this);

	//image path
	QString				m_currentFilePath;

	// set up image processing panels for the side
	QGroupBox*			m_brightness_contrast;  // does brightness and contrast selection
	QGroupBox*			m_rgb;					// does rgb selection

};
