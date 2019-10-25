
#include <cse/model_description_parser.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <exception>
#include <sstream>

using boost::property_tree::ptree;

cse::variable_causality parse_causality(const std::string& str)
{
    if (str == "calculatedParameter") {
        return cse::variable_causality::calculated_parameter;
    } else if (str == "parameter") {
        return cse::variable_causality::parameter;
    } else if (str == "input") {
        return cse::variable_causality::input;
    } else if (str == "output") {
        return cse::variable_causality::output;
    } else if (str == "local" || str == "independent") {
        return cse::variable_causality::local;
    } else {
        std::ostringstream oss;
        oss << "Don't know how to parse causality '" << str << "'";
        throw std::invalid_argument(oss.str());
    }
}

cse::variable_variability parse_variability(const std::string& str)
{
    if (str == "constant") {
        return cse::variable_variability::constant;
    } else if (str == "fixed") {
        return cse::variable_variability::fixed;
    } else if (str == "tunable") {
        return cse::variable_variability::tunable;
    } else if (str == "discrete") {
        return cse::variable_variability::discrete;
    } else if (str == "continuous") {
        return cse::variable_variability::continuous;
    } else {
        std::ostringstream oss;
        oss << "Don't know how to parse variability '" << str << "'";
        throw std::invalid_argument(oss.str());
    }
}

cse::variable_description parse_variable(const ptree& node)
{

    cse::variable_description variable;

    variable.name = node.get<std::string>("<xmlattr>.name");
    variable.causality = parse_causality(node.get<std::string>("<xmlattr>.causality", ""));
    variable.variability = parse_variability(node.get<std::string>("<xmlattr>.variability", ""));

    for (const auto& v : node) {
        const auto type = v.first;
        if (type == "Integer") {
            variable.type = cse::variable_type::integer;
        } else if (type == "Real") {
            variable.type = cse::variable_type::real;
        } else if (type == "String") {
            variable.type = cse::variable_type::string;
        } else if (type == "Boolean") {
            variable.type = cse::variable_type::boolean;
        } else {
            std::ostringstream oss;
            oss << "Unsupported type '" << type << "'";
            throw std::invalid_argument(oss.str());
        }
    }

    return variable;
}

void parse_model_variables(const ptree& node, std::vector<cse::variable_description>& variables)
{
    for (const ptree::value_type& v : node) {
        if (v.first == "ScalarVariable") {
            auto var = parse_variable(v.second);
            variables.push_back(var);
        }
    }
}

std::shared_ptr<const cse::model_description> cse::parse_model_description(const std::string& xml)
{
    std::stringstream ss;
    ss << xml;
    ptree tree;
    read_xml(ss, tree);
    ptree root = tree.get_child("fmiModelDescription");

    auto desc = std::make_shared<cse::model_description>();

    desc->uuid = root.get<std::string>("<xmlattr>.guid");
    desc->name = root.get<std::string>("<xmlattr>.modelName");
    desc->description = root.get<std::string>("<xmlattr>.description", "");
    desc->author = root.get<std::string>("<xmlattr>.author", "");
    desc->version = root.get<std::string>("<xmlattr>.version", "");

    for (const auto& v : root) {
        if (v.first == "ModelVariables") {
            parse_model_variables(v.second, desc->variables);
        }
    }

    return desc;
}
