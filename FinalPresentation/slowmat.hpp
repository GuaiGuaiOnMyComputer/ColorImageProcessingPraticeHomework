#include <stdint.h>
#include <cstdlib>

namespace my
{
    class SlowMat
    {
        public:
        static SlowMat& zeros(size_t _rows, size_t _cols, int _channels)
        {
            SlowMat* tmp = new SlowMat(_rows, _cols, _channels);
        }

        explicit SlowMat(size_t _rows, size_t _cols, int _channels) noexcept : rows{_rows}, cols{_cols}, channels{_channels}
        {
            m_data = new uint8_t**[_channels];
            for(size_t channel = 0; channel < _channels; channel ++){
                m_data[channel] = new uint8_t*[_rows];
                for(size_t row = 0; row < _rows; row ++){
                  m_data[channel][row] = new uint8_t[_cols]; 
                }
            }
        }

        SlowMat(SlowMat&& other) : channels{other.channels}, rows{other.rows}, cols{other.cols}
        {
            this->m_data = other.m_data;
            other.m_data = nullptr;
            other.channels = 0;
            other.cols = 0; 
            other.rows = 0;
        }

        virtual ~SlowMat()
        {
            for(int channel = 0; channel < channels; channel++){
                for(size_t row = 0; row < rows; row++){
                    for(size_t col = 0; col < cols; col++){
                        delete[] m_data[channel][row];
                    }
                }
            }
        }

        public:
        int channels;
        size_t rows;
        size_t cols;

        private:
        private:
        uint8_t*** m_data;
    };

}
