#include <stdint.h> // uint8_t
#include <memory>   // std::shared_ptr
#include <memory.h> // memset()
#include <opencv2/imgcodecs.hpp>
namespace my
{
    class SlowMat
    {
        public:

        // creates a SlowMat object whose pixel values are initialized to 0
        // returned as a shared pointer to object
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

        static std::shared_ptr<SlowMat> FromCvMat(const cv::Mat& _original)
        {
            auto tmp = std::make_shared<my::SlowMat>(_original.rows, _original.cols, _original.channels());
            for (int channel = 0; channel < tmp->channels; channel++){
                for (size_t row = 0; row < tmp->rows; row ++){
                    for (size_t col = 0; col < tmp->cols; col ++){
                        tmp->m_data[channel][row][col] = _original.ptr()[channel * _original.rows * _original.cols + row * _original.cols + col];
                    }
                }
            }
            return tmp;
        }

        cv::Mat ToCvMat() const
        {
            cv::Mat tmp = cv::Mat::zeros(rows, cols, CV_8UC3);
            for (int channel = 0; channel < tmp.channels(); channel++){
                for (size_t row = 0; row < tmp.rows; row ++){
                    for (size_t col = 0; col < tmp.cols; col ++){
                        tmp.ptr()[channel * tmp.rows * tmp.cols + row * tmp.cols + col] = m_data[channel][row][col];
                    }
                }
            }
            return tmp;
        }

        // regular constructor, heap-allocates memory to store pixel values
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

        // move constructor(not used)
        SlowMat(SlowMat&& other) : channels{other.channels}, rows{other.rows}, cols{other.cols}
        {
            this->m_data = other.m_data;
            other.m_data = nullptr;
            other.channels = 0;
            other.cols = 0; 
            other.rows = 0;
        }

        // copy constructor
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

        // move assignment operator
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

        void inline AlterAnElement(size_t index, uint8_t val){
            m_data[index % channels][index % rows][index % cols] = val % channels;
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
