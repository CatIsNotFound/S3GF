
#include "Configuration.h"

#include "Algorithm/String.h"

namespace MyEngine::Config {
    AbstractConfigParser::~AbstractConfigParser() = default;

    void AbstractConfigParser::setDefaultFileName(const std::string &_file_name) {
        _default_file_name = _file_name;
    }

    std::string_view AbstractConfigParser::defaultFileName() const {
        return _default_file_name;
    }

    ConfigArray AbstractConfigParser::getConfigArray() const {
        return std::holds_alternative<ConfigArray>(_variant) ? std::get<ConfigArray>(_variant) : ConfigArray();
    }

    ConfigObject AbstractConfigParser::getConfigObject() const {
        return std::holds_alternative<ConfigObject>(_variant) ? std::get<ConfigObject>(_variant) : ConfigObject();
    }

    ConfigArray& AbstractConfigParser::configArray() {
        return std::get<ConfigArray>(_variant);
    }

    ConfigObject & AbstractConfigParser::configObject() {
        return std::get<ConfigObject>(_variant);
    }

    bool AbstractConfigParser::isObject() const {
        return std::holds_alternative<ConfigObject>(_variant);
    }

    bool AbstractConfigParser::isArray() const {
        return std::holds_alternative<ConfigArray>(_variant);
    }

    BinaryConfigParser::BinaryConfigParser(const std::string &file_name)
            : AbstractConfigParser(file_name) {}

    BinaryConfigParser::BinaryConfigParser(const ConfigVariant &your_config)
            : AbstractConfigParser(your_config) {}

    BinaryConfigParser::BinaryConfigParser(ConfigVariant &&your_config) noexcept
            : AbstractConfigParser(std::move(your_config)) {}

    BinaryConfigParser::~BinaryConfigParser() = default;

    bool BinaryConfigParser::saveFile() {
        auto real_file_path = FileSystem::getAbsolutePath(_default_file_name);
        if (_default_file_name.empty())
            return false;
        _temp_var = _variant;
        return FileSystem::writeBinaryFile(real_file_path, false, &BinaryConfigParser::dumpToFile);
    }

    bool BinaryConfigParser::saveFile(const std::string &file_name) {
        auto real_file_path = FileSystem::getAbsolutePath(file_name);
        if (file_name.empty())
            return false;
        _temp_var = _variant;
        return FileSystem::writeBinaryFile(real_file_path, false, &BinaryConfigParser::dumpToFile);
    }

    bool BinaryConfigParser::loadFile() {
        auto real_file_path = FileSystem::getAbsolutePath(_default_file_name);
        if (_default_file_name.empty())
            return false;
        auto _ret = FileSystem::readBinaryFile(real_file_path, &BinaryConfigParser::parseFromFile, false);
        if (_ret) _variant = _temp_var;
        return _ret;
    }

    bool BinaryConfigParser::loadFile(const std::string &file_name) {
        auto real_file_path = FileSystem::getAbsolutePath(_default_file_name);
        if (_default_file_name.empty())
            return false;
        auto _ret = FileSystem::readBinaryFile(real_file_path, &BinaryConfigParser::parseFromFile, false);
        if (_ret) _variant = _temp_var;
        return _ret;
    }

    bool BinaryConfigParser::parseFromFile(std::ifstream &stream) {
        std::string buf(4, '\0');
        stream.read(buf.data(), 4);
        if (buf != "CONF") {
            return false;
        }
        char len[1];
        stream.read(len, 1);
        std::vector<uint8_t> len_arr(len[0], '\0');
        stream.read(reinterpret_cast<char*>(len_arr.data()), len_arr.size());
        size_t length = Algorithm::bin2Number(len_arr);
        bool _ret = false;
        if (length == 1) {
            // Object
            _temp_var = stringToObject(stream, &_ret);
        } else if (length > 1) {
            // Array
            _temp_var = stringToArray(stream, length, &_ret);
        } else {
            return false;
        }
        return _ret;
    }

    void BinaryConfigParser::dumpToFile(std::ofstream &stream) {
        std::string str_view;
        // head
        str_view.append("CONF");
        stream.write(str_view.data(), str_view.size());
        // main
        if (std::holds_alternative<ConfigObject>(_temp_var)) {
            str_view = objectToString(std::get<ConfigObject>(_temp_var));
            stream.write(str_view.data(), str_view.size());
        } else if (std::holds_alternative<ConfigArray>(_temp_var)) {
            str_view = arrayToString(std::get<ConfigArray>(_temp_var));
            stream.write(str_view.data(), str_view.size());
        }
        // eof
        str_view.assign("EOF");
        stream.write(str_view.data(), str_view.size());
        stream.flush();
    }

    std::string BinaryConfigParser::objectToString(const ConfigObject &var) {
        std::string result;
        // head
        result.push_back(1);
        result.push_back(1);
        auto n_array = Algorithm::number2Bin(var.size());
        result.append("MAP");
        result.push_back(n_array.size());
        result.append(n_array.begin(), n_array.end());
        // write
        for (auto& [key, value] : var) {
            // Key
            result.push_back('K');
            result.push_back(key.size());
            result.append(key.data(), key.size());
            // Value: V (type) (value_length) length value
            result.push_back('V');
            result.push_back(value.type());
            auto bin_arr = value.valueAsBinary();
            n_array = Algorithm::number2Bin(bin_arr.size());
            result.push_back(n_array.size());
            result.append(n_array.begin(), n_array.end());
            result.append(bin_arr.begin(), bin_arr.end());
        }

        return result;
    }

    std::string BinaryConfigParser::arrayToString(const ConfigArray &var) {
        std::string result;
        // head
        auto var_len = Algorithm::number2Bin(var.size());
        result.push_back(var_len.size());
        result.append(var_len.begin(), var_len.end());
        // map
        for (auto& arr : var) {
            result.append("MAP");
            var_len = Algorithm::number2Bin(arr.size());
            result.push_back(var_len.size());
            result.append(var_len.begin(), var_len.end());
            // write
            for (auto& [key, value] : arr) {
                // Key
                result.push_back('K');
                result.push_back(key.size());
                result.append(key.data(), key.size());
                // Value: V (type) (value_length) length value
                result.push_back('V');
                result.push_back(value.type());
                auto bin_arr = value.valueAsBinary();
                var_len = Algorithm::number2Bin(bin_arr.size());
                result.push_back(var_len.size());
                result.append(var_len.begin(), var_len.end());
                result.append(bin_arr.begin(), bin_arr.end());
            }
        }
        return result;
    }

    ConfigObject BinaryConfigParser::stringToObject(std::ifstream &stream, bool* ok) {
        ConfigObject result;
        std::string buf(3, '\0');
        stream.read(buf.data(), 3);
        if (buf != "MAP") {
            if (ok) *ok = false;
            return result;
        }
        char len[1];
        stream.read(len, 1);
        std::vector<uint8_t> len_arr(len[0], '\0');
        stream.read(reinterpret_cast<char*>(len_arr.data()), len_arr.size());
        size_t length = Algorithm::bin2Number(len_arr);
        while (length--) {
            // Read key
            stream.read(len, 1);
            if (len[0] != 'K') {
                if (ok) *ok = false;
                return result;
            }
            stream.read(len, 1);
            std::string key(len[0], '\0');
            stream.read(key.data(), key.size());
            // Read value
            stream.read(len, 1);
            if (len[0] != 'V') {
                if (ok) *ok = false;
                return result;
            }
            stream.read(len, 1);
            auto var_type = static_cast<Variant::Type>(len[0]);
            stream.read(len, 1);
            len_arr.assign(len[0], '\0');
            stream.read(reinterpret_cast<char*>(len_arr.data()), len_arr.size());
            size_t str_len = Algorithm::bin2Number(len_arr);
            std::vector<uint8_t> bin_arr(str_len, '\0');
            stream.read(reinterpret_cast<char*>(bin_arr.data()), bin_arr.size());
            Variant new_var;
            new_var.binaryToValue(bin_arr, var_type);
            result.emplace(key, new_var);
        }
        if (ok) *ok = true;
        return result;
    }

    ConfigArray BinaryConfigParser::stringToArray(std::ifstream &stream, size_t array_length, bool* ok) {
        ConfigArray result;
        while (array_length--) {
            ConfigObject object;
            std::string buf(3, '\0');
            stream.read(buf.data(), 3);
            if (buf != "MAP") {
                if (ok) *ok = false;
                return result;
            }
            char len[1];
            stream.read(len, 1);
            std::vector<uint8_t> len_arr(len[0], '\0');
            stream.read(reinterpret_cast<char*>(len_arr.data()), len_arr.size());
            size_t length = Algorithm::bin2Number(len_arr);
            while (length--) {
                // Read key
                stream.read(len, 1);
                if (len[0] != 'K') {
                    if (ok) *ok = false;
                    return result;
                }
                stream.read(len, 1);
                std::string key(len[0], '\0');
                stream.read(key.data(), key.size());
                // Read value
                stream.read(len, 1);
                if (len[0] != 'V') {
                    if (ok) *ok = false;
                    return result;
                }
                stream.read(len, 1);
                Variant::Type var_type = static_cast<Variant::Type>(len[0]);
                stream.read(len, 1);
                len_arr.assign(len[0], '\0');
                stream.read(reinterpret_cast<char*>(len_arr.data()), len_arr.size());
                size_t str_len = Algorithm::bin2Number(len_arr);
                std::vector<uint8_t> bin_arr(str_len, '\0');
                stream.read(reinterpret_cast<char*>(bin_arr.data()), bin_arr.size());
                Variant new_var;
                new_var.binaryToValue(bin_arr, var_type);
                object.emplace(key, new_var);
            }
            result.push_back(object);
        }
        if (ok) *ok = true;
        return result;
    }
}
