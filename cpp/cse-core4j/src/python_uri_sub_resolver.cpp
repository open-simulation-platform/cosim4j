
#include <cse/log/logger.hpp>
#include <cse/python_uri_sub_resolver.hpp>

namespace cse
{

std::shared_ptr<model> cse::python_uri_sub_resolver::lookup_model(const cse::uri& modelUri)
{
    assert(modelUri.scheme().has_value());
    if (*modelUri.scheme() != "file") return nullptr;
    if (modelUri.authority().has_value() &&
        !(modelUri.authority()->empty() || *modelUri.authority() == "localhost")) {
        return nullptr;
    }
    if (modelUri.query().has_value() || modelUri.fragment().has_value()) {
        BOOST_LOG_SEV(log::logger(), log::warning)
            << "Query and/or fragment component(s) in a file:// URI were ignored: "
            << modelUri;
    }
    const auto path = file_uri_to_path(modelUri);
    if (!path.has_extension()) return nullptr;
    if (path.extension() != ".py") return nullptr;
}

} // namespace cse
