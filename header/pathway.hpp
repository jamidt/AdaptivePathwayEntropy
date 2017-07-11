#pragma once

#include "landscape.hpp"
#include "sequence.hpp"
#include <unordered_map>
#include <vector>

namespace adaptive {

class GenericPathProbabilities {
  protected:
    FitnessLandscape* const _landscape;

    using container_prob_t = std::unordered_map<Sequence, std::vector<double>>;
    // Store path probabilities to a given endpoint
    container_prob_t _path_prob;

  public:
    GenericPathProbabilities() = delete;
    GenericPathProbabilities(FitnessLandscape* landscape)
        : _landscape(landscape) {}

    virtual void calcPathProb() = 0;

    void reset() { _path_prob.clear(); }

    std::size_t numberMaxima() const { return _path_prob.size(); }

    std::size_t numberPathways() const {
        std::size_t numpath = 0;
        for (const auto& max : _path_prob) {
            numpath += std::get<1>(max).size();
        }
        return numpath;
    }

    using iterator = typename container_prob_t::iterator;
    using const_iterator = typename container_prob_t::const_iterator;
    iterator begin() { return _path_prob.begin(); }
    const_iterator begin() const { return _path_prob.cbegin(); }
    const_iterator cbegin() const { return _path_prob.cbegin(); }
    iterator end() { return _path_prob.end(); }
    const_iterator end() const { return _path_prob.cend(); }
    const_iterator cend() const { return _path_prob.cend(); }
};

// Undirected hypercube
class PathProbabilities : public GenericPathProbabilities {
  public:
    PathProbabilities(FitnessLandscape* landscape)
        : GenericPathProbabilities(landscape) {}
    void calcPathProb() {
        reset();
        Sequence seq(_landscape->loci());
        pathProb_impl(seq, 1.0);
    }

  private:
    void pathProb_impl(const Sequence& seq, double probability) {
        SeqVec_t neighbours = _landscape->getFitterNeighbours(seq);
        if (neighbours.size() == 0) {
            _path_prob[seq].push_back(probability);
        } else {
            double denominator = 0;
            int count = 0;
            for (const auto& n : neighbours) {
                denominator += _landscape->at(n);
                ++count;
            }
            denominator -= count * _landscape->at(seq);
            for (const auto& n : neighbours) {
                double nominator = _landscape->at(n) - _landscape->at(seq);
                double step_prob = nominator / denominator;
                pathProb_impl(n, probability * step_prob);
            }
        }
    }
};

// Directed hypercube
class PathProbabilitiesDirected : public GenericPathProbabilities {
  public:
    PathProbabilitiesDirected(FitnessLandscape* landscape)
        : GenericPathProbabilities(landscape) {}
    void calcPathProb() {
        reset();
        // Start at (0000...000)
        Sequence seq(_landscape->loci());
        pathProb_impl(seq, 1.0);
    }

  private:
    void pathProb_impl(const Sequence& seq, double probability) {
        SeqVec_t neighbours = _landscape->getFitterNeighboursDirected(seq);
        // Check if this is a local maxima
        if (neighbours.size() == 0) {
            _path_prob[seq].push_back(probability);
        }
        // If not an endpoint, next step
        else {
            double denominator = 0;
            int count = 0;
            for (const auto& n : neighbours) {
                denominator += _landscape->at(n);
                ++count;
            }
            denominator -= count * _landscape->at(seq);
            for (const auto& n : neighbours) {
                double nominator = _landscape->at(n) - _landscape->at(seq);
                double step_prob = nominator / denominator;
                pathProb_impl(n, probability * step_prob);
            }
        }
    }
};
}
