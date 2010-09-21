#ifndef _XIAO5GEPROJECT_BITMAP_H_
#define _XIAO5GEPROJECT_BITMAP_H_

/**
 * @file    bitmap.h
 * @brief   A bitmap implementation class
 * @author  Wu Zesheng <wuzesheng@wuzesheng.com>
 * @since   2010-09-20
 * @version 1.0.0
 */

#include <assert.h>

class Bitmap
{
public:

    Bitmap(size_t size = 1024)
    {
        m_size = size;
        m_bit_vector = new char[size>>s_shift];
        ::memset(m_bit_vector, 0, size>>s_shift);
    }

    ~Bitmap()
    {
        delete [] m_bit_vector;
    }

    ///set the bit at position 'pos'
    void Set(size_t pos)
    {
        assert(pos < m_size);
        m_bit_vector[pos>>s_shift] |= (1<<(pos & s_mask));
    }

    ///clear the bit at position 'pos'
    void Clear(size_t pos)
    {
        assert(pos < m_size);
        m_bit_vector[pos>>s_shift] &= ~(1<<(pos & s_mask));
    }

    ///determine whether the bit at position 'pos' is set
    bool Test(size_t pos) const
    {
        assert(pos < m_size);
        return !((m_bit_vector[pos>>s_shift] & (1<<(pos & s_mask))) == 0);
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

