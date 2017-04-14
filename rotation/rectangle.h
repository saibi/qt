#ifndef RECTANGLE_H
#define RECTANGLE_H

class rectangle
{
public:
	rectangle();
	~rectangle();

	void set(int width, int height) { m_width = width; m_height = height; }

private:
	int m_width;
	int m_height;

	int *m_id;
};

#endif // RECTANGLE_H
