#ifndef _XIAO5GEPROJECT_BITMAP_H_
#define _XIAO5GEPROJECT_BITMAP_H_

/**
 * @file    bitmap.h
 * @brief   A bitmap implementation class
 * @author  Wu Zesheng <wuzesheng@wuzesheng.com>
 * @since   2010-09-20
 * @version 1.0.0
 */

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

	///set the bit at position 'pos'
	void Set(size_t pos)
	{
		size_t index = pos / s_char_size;
		size_t offset = pos % s_char_size;
		m_bit_vector[index] |= (0x1 << offset);
	}

	///determine whether the bit at position 'pos' is set
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

