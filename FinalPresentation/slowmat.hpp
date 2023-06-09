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
        // use of new operator can easily lead to memory leak
        // heap allocation is bad for performance, calling new operator for many times is discouraged
        explicit SlowMat(size_t _rows, size_t _cols, int _channels) : rows{_rows}, cols{_cols}, channels{_channels}
        {
            m_data = new uint8_t**[_channels]; // allocate an array of uint8_t** channel pointers for an image
            for(size_t channel = 0; channel < _channels; channel ++){
                m_data[channel] = new uint8_t*[_rows]; // allocate an array of uint8_t* row pointers for each channel
                for(size_t row = 0; row < _rows; row ++){
                  m_data[channel][row] = new uint8_t[_cols]; // allocate an array of uint8_t for each row
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
            #ifdef FALSE_COPY

            #elif DOUBLE_DELETE
            this->m_data = other.m_data;

            #else
            this->m_data = new uint8_t**[channels];
            for(size_t channel = 0; channel < channels; channel ++){
                this->m_data[channel] = new uint8_t*[rows];
                for(size_t row = 0; row < rows; row ++){
                    this->m_data[channel][row] = new uint8_t[cols];
                    memcpy(m_data[channel][row], other.m_data[channel][row], sizeof(uint8_t) * cols); 
                }
            }
            #endif
        }

        void Convolution(const int32_t _kernal[])
        {
            constexpr int KER_SIZE = 5;
            SlowMat original = SlowMat(*this);
            const int halfKerLen = (KER_SIZE - 1) / 2;
            const int convHozStart = (KER_SIZE - 1) / 2;
            const int convVrtStart = (KER_SIZE - 1) / 2;
            const int convHozEnd = cols - (KER_SIZE - 1) / 2 - 1;
            const int convVrtEnd = rows - (KER_SIZE - 1) / 2 - 1;
            for (int channel = 0; channel < channels; channel ++){
                for (int row = convVrtStart; row < convVrtEnd; row ++){
                    for (int col = convHozStart; col < convHozEnd; col ++){
                        for (int i = 0; i < KER_SIZE; i ++){
                            for (int j = 0; j < KER_SIZE; j++){
                                m_data[channel][row][col] += original.m_data[channel][row - halfKerLen + i][col - halfKerLen + j] * _kernal[j + KER_SIZE * i];
                            }
                        }
                    }
                }
            }
        }

        void Add(const SlowMat& other)
        {
            for (int channel = 0; channel < this->channels; channel++){
                for (int row = 0; row < this->rows; row ++){
                    for (int col = 0; col < this->cols; col++){
                        this->m_data[channel][row][col] += other.m_data[channel][row][col];
                    }
                }
            }
        }

        void Minus(const SlowMat& other)
        {
            for (int channel = 0; channel < this->channels; channel++){
                for (int row = 0; row < this->rows; row ++){
                    for (int col = 0; col < this->cols; col++){
                        this->m_data[channel][row][col] -= other.m_data[channel][row][col];
                    }
                }
            }
        }

        void ElementMul(const SlowMat& other)
        {
            for (int channel = 0; channel < this->channels; channel++){
                for (int row = 0; row < this->rows; row ++){
                    for (int col = 0; col < this->cols; col++){
                        this->m_data[channel][row][col] *= other.m_data[channel][row][col];
                    }
                }
            }
        }

        // custom indexing operator
        uint8_t** operator[](int channel)
        {
            return m_data[channel];
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
