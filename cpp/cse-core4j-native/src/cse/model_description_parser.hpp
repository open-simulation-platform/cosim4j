
#ifndef CSECOREJNI_MODEL_DESCRIPTION_PARSER_HPP
#define CSECOREJNI_MODEL_DESCRIPTION_PARSER_HPP

#include <cse/model.hpp>

#include <memory>
#include <string>

namespace cse
{

std::shared_ptr<const cse::model_description> parse_model_description(const std::string& xml);

}

#endif
