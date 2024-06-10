#include "config.h"


// 遍历所有结点
void ListAllMember(const std::string& prefix,
                          const YAML::Node& node,
                          std::list<std::pair<std::string, const YAML::Node> >& output) {
    if(prefix.find_first_not_of("abcdefghikjlmnopqrstuvwxyz._0123456789")
            != std::string::npos) {
        std::cout << "Config invalid name: " << prefix << " : " << node;
        return;
    }
    output.push_back(std::make_pair(prefix, node));
    if(node.IsMap()) {
        for(auto it = node.begin();
                it != node.end(); ++it) {
            ListAllMember(prefix.empty() ? it->first.Scalar()
                    : prefix + "." + it->first.Scalar(), it->second, output);
        }
    }
}

void Config::LoadFromYaml(const YAML::Node &root){
    // 结点类型<string,YAML::Node>
    std::list<std::pair<std::string, const YAML::Node>> all_nodes;
    ListAllMember("", root, all_nodes);

    for (auto &i : all_nodes){
        // 遍历，获取key，查找是否包含key，如果包含，将之前修改为从文件中加载的值
        std::string key = i.first;
        if (key.empty()){
            continue;
        }
        // 将key转为小写
        std::transform(key.begin(), key.end(), key.begin(), ::tolower);
        // 查询是否包含key
        ConfigVarBase::ptr var = LookupBase(key);

        // 如果存在key才从文件中加载更新，不存在直接跳过
        if (var){
            if (i.second.IsScalar()){
                // 将YAML::内结点值转为Scalar类型
                // 然后从字符串中加载（已通过实现偏特化实现了类型的转换），设置m_val，进行更新
                var->fromString(i.second.Scalar());
            }
            else{
                // 其他类型 Sequence,偏特化中fromString有对应的处理方法
                std::stringstream ss;
                ss << i.second;
                var->fromString(ss.str());
            }
        }
    }
}
