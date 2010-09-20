#ifndef _XIAO5GEPROJECT_BITMAP_H_
#define _XIAO5GEPROJECT_BITMAP_H_

class Bitmap
{
public:

	Bitmap(size_t size = 1024)
	{
		m_bit_vector = new char[size];
		::memset(m_bit_vector, 0, size);
	}

	~Bitmap()
	{
		delete [] m_bit_vector;
	}

	void Set(size_t pos)
	{
		size_t index = pos / s_char_size;
		size_t offset = pos % s_char_size;
		m_bit_vector[index] |= (0x1 << offset);
	}

	bool IsSet(size_t pos) const
	{
		size_t index = pos / s_char_size;
		size_t offset = pos % s_char_size;
		return m_bit_vector[index] & (0x1 << offset);
	}

private:

	char * m_bit_vector;

	static const char s_char_size = 8;
};

#endif //_XIAO5GEPROJECT_BITMAP_H_

