#pragma once

#include "pathway.hpp"
#include <cmath>

namespace adaptive {
double path_entropy(const GenericPathProbabilities& path_prob) {
    double entropy = 0;
    for(const auto& endpoint: path_prob) {
        for(const auto& prob: std::get<1>(endpoint)) {
            entropy += prob * std::log(prob);
        }
    }
    return -entropy;
}

double endpoint_entropy(const GenericPathProbabilities& path_prob) {
    double entropy = 0;
    for(const auto& endpoint: path_prob) {
        double endpoint_prob = 0;
        for(const auto& prob: std::get<1>(endpoint)) {
            endpoint_prob += prob;
        }
        entropy += endpoint_prob * std::log(endpoint_prob);
    }
    return -entropy;
}

double conditional_entropy(const GenericPathProbabilities& path_prob) {
    double entropy = 0;
    for (const auto& endpoint: path_prob) {
        double endpoint_prob = 0;
        for(const auto& prob : std::get<1>(endpoint)) {
            endpoint_prob += prob;
        }
        for(const auto& prob: std::get<1>(endpoint)) {
            entropy += prob * std::log(endpoint_prob / prob);
        }
    }
    return entropy;
}

}
