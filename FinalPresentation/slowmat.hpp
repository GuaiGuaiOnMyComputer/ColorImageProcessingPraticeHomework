#include <stdint.h> // uint8_t
#include <memory>   // std::shared_ptr
#include <memory.h> // memset()
namespace my
{
    class SlowMat
    {
        public:
        static std::shared_ptr<SlowMat> zeros(size_t _rows, size_t _cols, int _channels)
        {
            std::shared_ptr<SlowMat> tmp = std::make_shared<SlowMat>(_rows, _cols, _channels);
            for(int channel = 0; channel < tmp->channels; channel ++){
                for(size_t row = 0; row < tmp->rows; row ++){
                    memset(tmp->m_data[channel][row], 0, sizeof(uint8_t) * tmp->cols);
                }
            }
            return tmp;
        }

        explicit SlowMat(size_t _rows, size_t _cols, int _channels) : rows{_rows}, cols{_cols}, channels{_channels}
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

        SlowMat(const SlowMat& other) : channels{other.channels}, rows{other.rows}, cols{other.cols}
        {
            this->m_data = new uint8_t**[channels];
            for(size_t channel = 0; channel < channels; channel ++){
                this->m_data[channel] = new uint8_t*[rows];
                for(size_t row = 0; row < rows; row ++){
                    this->m_data[channel][row] = new uint8_t[cols];
                    memcpy(m_data[channel][row], other.m_data[channel][row], sizeof(uint8_t) * cols); 
                }
            }
        }


        SlowMat& operator=(SlowMat&& other) noexcept
        {
            if(this == &other)
                return *this;

            m_DeleteData();
            this->m_data = other.m_data;
            this->channels = other.channels;
            this->cols = other.cols;
            this->rows = other.rows;
            other.m_data = nullptr;
            other.channels = 0;
            other.rows = 0;
            other.cols = 0;
        }

        virtual ~SlowMat()
        {
            m_DeleteData();
        }

        public:
        int channels;
        size_t rows;
        size_t cols;

        private:
        void m_DeleteData()
        {
            for(int channel = 0; channel < channels; channel++){
                for(size_t row = 0; row < rows; row++){
                    delete[] m_data[channel][row];
                }
                delete[] m_data[channel];
            }
            delete[] m_data;
        }

        private:
        uint8_t*** m_data;
    };

}
