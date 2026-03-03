
#ifndef MYENGINE_UTILS_WAVWRITER_H
#define MYENGINE_UTILS_WAVWRITER_H
#include "../Libs.h"
namespace MyEngine {
    class WAVWriter {
    public:
        explicit WAVWriter() : _path() {}
        explicit WAVWriter(std::string_view&& path) noexcept;
        ~WAVWriter();

        void setOutputPath(std::string_view&& path) noexcept;
        const std::string& outputPath() const noexcept;

        bool begin(int sample_rates, int channels, int bytes) noexcept;
        bool end() noexcept;
        bool write(void* data, size_t size) noexcept;
        bool isOpen() const noexcept;

    private:
        template <typename T>
        void writeData(T value) {
            _file.write(reinterpret_cast<const char*>(&value), sizeof(T));
        }

        std::ofstream _file;
        size_t _data_size{};
        std::streampos _position{};
        std::string _path{};
    };
}

#endif //MYENGINE_UTILS_WAVWRITER_H