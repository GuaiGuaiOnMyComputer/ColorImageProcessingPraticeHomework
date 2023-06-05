#include <stdint.h>

namespace my
{
    class LapFilter
    {
        public:
        const int32_t LAP_FILTER[25] = {
            0 ,  0, -1,  0, 0,
            0 , -1, -2, -1, 0,
            -1, -2, 16, -2, -1,
            0 , -1, -2, -1, 0,
            0 ,  0, -1,  0, 0,
        };

        const int KER_SIZE = 5;

        LapFilter() = delete;
    };
} // namespace my
