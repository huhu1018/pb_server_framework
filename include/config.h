#include <string>
#include <memory>
#include <algorithm>
#include <functional>
#include <yaml-cpp/yaml.h>
#include <boost/lexical_cast.hpp>
#include "log.h"

// 配置模块基础信息对象
class ConfigVarBase {
public:
    typedef std::shared_ptr<ConfigVarBase> ptr;
    ConfigVarBase(const std::string& name, const std::string& description = "")
        :m_name(name)
        ,m_description(description) {
        std::transform(m_name.begin(), m_name.end(), m_name.begin(), tolower);
    }
    virtual ~ConfigVarBase() {}

    const std::string& getName() const { return m_name;}
    const std::string& getDescription() const { return m_description;}

    virtual std::string toString() = 0;
    virtual bool fromString(const std::string& val) = 0;
    virtual std::string getTypeName() const = 0;
protected:
    std::string m_name;
    std::string m_description;
};

// 配置变量模板类
template<class T>
class ConfigVar : public ConfigVarBase {
public:
    typedef std::shared_ptr<ConfigVar> ptr;
    ConfigVar(const std::string& name
            ,const T& default_value
            ,const std::string& description = "")
        :ConfigVarBase(name, description)
        ,m_val(default_value) {
    }
    std::string toString() override {
        try {
            return boost::lexical_cast<std::string>(m_val);
        } catch (std::exception& e) {
            std::cout << "ConfigVar::toString exception"
                << e.what() << " convert: " << typeid(m_val).name() << " to string";
        }
        return "";
    }
    bool fromString(const std::string& val) override {
        try {
            m_val = boost::lexical_cast<T>(val);
        } catch (std::exception& e) {
            std::cout << "ConfigVar::toString exception"
                << e.what() << " convert: string to " << typeid(m_val).name()
                << " - " << val;
        }
        return false;
    }
    const T getValue() const { return m_val;}
    void setValue(const T& v) {
        m_val = v;
    }
    std::string getTypeName() const override { return typeid(T).name();}
private:
    T m_val;
};


// 配置模块用于解析yaml文件
class Config {
public:
    typedef std::unordered_map<std::string, ConfigVarBase::ptr> ConfigVarMap;

    // 查找或创建一个新的配置变量
    template<class T>
    static typename ConfigVar<T>::ptr Lookup(const std::string& name,
            const T& default_value, const std::string& description = "") {
        auto it = GetDatas().find(name);
        if(it != GetDatas().end()) {
            auto tmp = std::dynamic_pointer_cast<ConfigVar<T> >(it->second);
            if(tmp) {
                std::cout << "Lookup name=" << name << " exists";
                return tmp;
            } else {
                std::cout << "Lookup name=" << name << " exists but type not "
                        << typeid(T).name() << " real_type=" << it->second->getTypeName()
                        << " " << it->second->toString();
                return nullptr;
            }
        }

        if(name.find_first_not_of("abcdefghikjlmnopqrstuvwxyz._0123456789")
                != std::string::npos) {
            std::cout << "Lookup name invalid " << name;
            throw std::invalid_argument(name);
        }

        typename ConfigVar<T>::ptr v(new ConfigVar<T>(name, default_value, description));
        GetDatas()[name] = v;
        return v;
    }

    template<class T>
    static typename ConfigVar<T>::ptr Lookup(const std::string& name) {
        auto it = GetDatas().find(name);
        if(it == GetDatas().end()) {
            return nullptr;
        }
        return std::dynamic_pointer_cast<ConfigVar<T> >(it->second);
    }

    static void LoadFromYaml(const YAML::Node& root);
    static ConfigVarBase::ptr LookupBase(const std::string& name);
private:
    static ConfigVarMap& GetDatas() {
        static ConfigVarMap s_datas;
        return s_datas;
    }
};

ConfigVarBase::ptr Config::LookupBase(const std::string& name) {
    auto it = GetDatas().find(name);
    return it == GetDatas().end() ? nullptr : it->second;
}

void ListAllMember(const std::string& prefix,
                          const YAML::Node& node,
                          std::list<std::pair<std::string, const YAML::Node> >& output);

