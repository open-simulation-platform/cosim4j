
#ifndef COSIMJNI_COSIM_MODEL_DESCRIPTION_PARSER_HPP
#define COSIMJNI_COSIM_MODEL_DESCRIPTION_PARSER_HPP

#include <cosim/model_description.hpp>

#include <memory>
#include <string>

namespace cosim
{

std::shared_ptr<const cosim::model_description> parse_model_description(const std::string& xml);

}

#endif
