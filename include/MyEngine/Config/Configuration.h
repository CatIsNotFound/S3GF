
#ifndef MYENGINE_CONFIG_CONFIGURATION_H
#define MYENGINE_CONFIG_CONFIGURATION_H
#include "../Utils/Variant.h"

namespace MyEngine::Config {
    using ConfigObject = std::unordered_map<std::string, Variant>;
    using ConfigObjectIterator = ConfigObject::iterator;
    using ConfigObjectConstIterator = ConfigObject::const_iterator;
    using ConfigArray = std::vector<ConfigObject>;
    using ConfigArrayIterator = ConfigArray::iterator;
    using ConfigArrayConstIterator = ConfigArray::const_iterator;
    using ConfigVariant = std::variant<std::monostate, ConfigObject, ConfigArray>;
    constexpr static uint32_t Var_ConfigObject = 16;
    constexpr static uint32_t Var_ConfigArray = 32;

    class AbstractConfigParser {
    public:
        explicit AbstractConfigParser(const std::string& file_name) : _default_file_name(file_name) {};
        explicit AbstractConfigParser(const ConfigVariant& your_config) : _variant(your_config) {}
        explicit AbstractConfigParser(ConfigVariant&& your_config) noexcept : _variant(std::move(your_config)) {}
        virtual ~AbstractConfigParser();

        void setDefaultFileName(const std::string& _file_name);
        [[nodiscard]] std::string_view defaultFileName() const;
        virtual bool saveFile() = 0;
        virtual bool saveFile(const std::string& file_name) = 0;
        virtual bool loadFile() = 0;
        virtual bool loadFile(const std::string& file_name) = 0;
        [[nodiscard]] ConfigArray getConfigArray() const;
        [[nodiscard]] ConfigObject getConfigObject() const;
        [[nodiscard]] ConfigArray& configArray();
        [[nodiscard]] ConfigObject& configObject();
        [[nodiscard]] bool isObject() const;
        [[nodiscard]] bool isArray() const;
        void setConfigObject(const ConfigObject& config_object);
        void setConfigObject(ConfigObject&& config_object) noexcept;
        void setConfigArray(const ConfigArray& config_array);
        void setConfigArray(ConfigArray&& config_array) noexcept;

        explicit AbstractConfigParser(const AbstractConfigParser&) = delete;
        explicit AbstractConfigParser(AbstractConfigParser&&) = delete;
        AbstractConfigParser& operator=(const AbstractConfigParser&) = delete;
        AbstractConfigParser& operator=(AbstractConfigParser&&) = delete;
    protected:
        std::string _default_file_name{};
        ConfigVariant _variant{std::monostate{}};
    };

    class BinaryConfigParser : public AbstractConfigParser {
    public:
        explicit BinaryConfigParser(const std::string& file_name);
        explicit BinaryConfigParser(const ConfigVariant& your_config);
        explicit BinaryConfigParser(ConfigVariant&& your_config) noexcept;
        ~BinaryConfigParser() override;
        bool saveFile() override;
        bool saveFile(const std::string& file_name) override;
        bool loadFile() override;
        bool loadFile(const std::string& file_name) override;
    private:
        static bool parseFromFile(std::ifstream &stream);
        static void dumpToFile(std::ofstream &stream);
        static std::string objectToString(const ConfigObject& var);
        static std::string arrayToString(const ConfigArray& var);
        static ConfigObject stringToObject(std::ifstream &stream, bool* ok);
        static ConfigArray stringToArray(std::ifstream &stream, size_t array_length, bool* ok);
        inline static ConfigVariant _temp_var{};
    };

}

#endif //MYENGINE_CONFIG_CONFIGURATION_H