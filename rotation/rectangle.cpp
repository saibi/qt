#include "rectangle.h"

rectangle::rectangle()
{
	static int id = 0;

	m_width = 0;
	m_height = 0;

	m_id = new int;

	*m_id = id++;
}

rectangle::~rectangle()
{
	delete m_id;
}
