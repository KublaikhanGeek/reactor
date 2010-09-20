#ifndef _XIAO5GEPROJECT_BITMAP_H_
#define _XIAO5GEPROJECT_BITMAP_H_

#include <assert.h>

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
        m_size = size;
		m_bit_vector = new char[size / s_char_size];
        ::memset(m_bit_vector, 0, size / s_char_size);
	}

	~Bitmap()
	{
		delete [] m_bit_vector;
	}

	///set the bit at position 'pos'
	void Set(size_t pos)
	{
        assert(pos < 1024);
		size_t index = pos / s_char_size;
		size_t offset = pos % s_char_size;
		m_bit_vector[index] |= (0x1 << offset);
	}

    void Clear(size_t pos)
    {}

	///determine whether the bit at position 'pos' is set
	bool Test(size_t pos) const
	{
        assert(pos < 1024);
		size_t index = pos / s_char_size;
		size_t offset = pos % s_char_size;
		return ((m_bit_vector[index] >> offset) & 0x1) == 1;
	}

    ///get the bitmap size
    size_t Size() const
    {
        return m_size;
    }

private:

	char * m_bit_vector;

    size_t m_size;

	static const int s_char_size = 8;

    static const int s_shift = 3;

    static const int s_mask = 0x7;
};

#endif //_XIAO5GEPROJECT_BITMAP_H_

