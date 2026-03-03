
#include "WavWriter.h"

#include "Logger.h"

namespace MyEngine {
    WAVWriter::WAVWriter(std::string_view &&path) noexcept
        : _path(FileSystem::getAbsolutePath(std::string(path))) {}

    WAVWriter::~WAVWriter() {
        if (_file.is_open()) {
            _file.close();
        }
    }

    void WAVWriter::setOutputPath(std::string_view &&path) noexcept {
        if (_file.is_open()) {
            Logger::log("WAVWriter: The file stream is already used, "
                        "please use `WAVWriter::end()` function at first!", Logger::Error);
            return;
        }
        _path = FileSystem::getAbsolutePath(std::string(path));
    }

    const std::string & WAVWriter::outputPath() const noexcept {
        return _path;
    }

    bool WAVWriter::begin(int sample_rates, int channels, int bits_per_sample, SDL_AudioFormat format) noexcept {
        _file.open(_path, std::ios::binary);
        if (!_file.is_open()) return false;

        // RIFF header
        _file.write("RIFF", 4);
        writeData<uint32_t>(0);
        _file.write("WAVE", 4);

        // fmt chunk
        _file.write("fmt ", 4);
        writeData<uint32_t>(16);
        writeData<uint16_t>(SDL_AUDIO_ISFLOAT(format) ? 3 : 1);
        writeData<uint16_t>(channels);
        writeData<uint32_t>(sample_rates);
        writeData<uint32_t>(sample_rates * channels * bits_per_sample / 8);
        writeData<uint16_t>(channels * bits_per_sample / 8);
        writeData<uint16_t>(bits_per_sample);

        // data chunk header
        _file.write("data", 4);
        _position = _file.tellp();
        writeData<uint32_t>(0);

        return true;
    }

    bool WAVWriter::end() noexcept {
        if (!_file.is_open()) return false;
        _file.seekp(_position);
        writeData<uint32_t>(static_cast<uint32_t>(_data_size));

        _file.seekp(4);
        writeData<uint32_t>(static_cast<uint32_t>(_data_size + 36));

        _file.close();
        _data_size = 0;
        return true;
    }

    bool WAVWriter::write(void *data, size_t size) noexcept {
        if (!_file) {
            Logger::log("WAVWriter: The file stream is not using! "
                        "Please use `WAVWriter::begin()` function at first!", Logger::Error);
            return false;
        }
        _file.write(static_cast<const char*>(data), size);
        _data_size += size;
        return true;
    }

    bool WAVWriter::isOpen() const noexcept {
        return _file.is_open();
    }
}
