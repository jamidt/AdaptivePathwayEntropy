#pragma once

#include "sequence.hpp"
#include <random>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace adaptive {

using SeqVec_t = std::vector<Sequence>;

class FitnessLandscape {
  protected:
    std::size_t _loci;
    std::unordered_map<Sequence, double> _landscape;

  public:
    using sequence_type = Sequence;
    using fitness_type = double;
    using value_type = std::pair<Sequence, fitness_type>;

    FitnessLandscape() = delete;
    FitnessLandscape(std::size_t loci) : _loci(loci) {}

    double& operator[](const Sequence& seq) { return _landscape[seq]; }
    double& operator[](Sequence&& seq) { return _landscape[std::move(seq)]; }

    double& at(const Sequence& seq) { return _landscape.at(seq); }
    const double& at(const Sequence& seq) const { return _landscape.at(seq); }

    void clear() { _landscape.clear(); }

    std::size_t loci() const { return _loci; }

    std::vector<Sequence> getNeighbourhood(const Sequence& seq) const {
        std::vector<Sequence> neighbour;
        for (std::size_t pos = 0; pos != _loci; ++pos) {
            Sequence tmp = seq;
            tmp.flip(pos);
            if (_landscape.count(tmp) == 0) {
                neighbour.push_back(tmp);
                continue;
            } else {
                continue;
            }
        }
        return neighbour;
    }

    /// Get fitter neighbours on the directed hypercube
    SeqVec_t getFitterNeighboursDirected(const Sequence& seq) const {
        SeqVec_t neighbours;
        for (std::size_t pos = 0; pos != _loci; ++pos) {
            if (seq.is_zero(pos)) {
                Sequence tmp = seq;
                tmp.flip(pos);
                if (_landscape.count(tmp)) {
                    if (_landscape.at(tmp) > _landscape.at(seq)) {
                        neighbours.push_back(tmp);
                    }
                }
            }
        }
        return neighbours;
    }

    /// Get fitter neighbours on the undirected hypercube (with
    // backstep)
    SeqVec_t getFitterNeighbours(const Sequence& seq) const {
        SeqVec_t neighbours;
        for (std::size_t pos = 0; pos != _loci; ++pos) {
            Sequence tmp = seq;
            tmp.flip(pos);
            if (_landscape.count(tmp)) {
                if (_landscape.at(tmp) > _landscape.at(seq)) {
                    neighbours.push_back(tmp);
                }
            }
        }
        return neighbours;
    }

    // Size of the container (= number of elements)
    std::size_t size() const { return _landscape.size(); }
};

class FitnessGenerator {
  public:
    virtual double get(const Sequence&) = 0;
};

class RandomGenerator : public FitnessGenerator {
  protected:
    std::mt19937_64 _eng;

  public:
    RandomGenerator() : _eng(std::random_device{}()) {}
    virtual double get(const Sequence&) = 0;
};

class UniformGenerator : public RandomGenerator {
  protected:
    std::uniform_real_distribution<> _dist;

  public:
    UniformGenerator(double a = 0, double b = 1)
        : RandomGenerator(), _dist(a, b) {}
    double get(const Sequence&) { return _dist(_eng); }
};

class UniformGeneratorStart : public RandomGenerator {
  protected:
    std::uniform_real_distribution<> _dist;
    double _start;

  public:
    UniformGeneratorStart(double start = 0, double a = 0, double b = 1)
        : RandomGenerator(), _dist(a, b), _start(start) {}
    double get(const Sequence& seq) {
        if (seq.is_zero()) {
            return _start;
        } else {
            return _dist(_eng);
        }
    }
    void setStart(double start) { _start = start; }
};

class ExponentialGenerator : public RandomGenerator {
  protected:
    std::exponential_distribution<double> _dist;

  public:
    ExponentialGenerator(double lambda = 1.0)
        : RandomGenerator(), _dist(lambda) {}
    double get(const Sequence&) { return _dist(_eng); }
};

class RMF_Exp : public ExponentialGenerator {
  private:
    double _slope;

  public:
    RMF_Exp(double slope, double lambda = 1)
        : ExponentialGenerator(lambda), _slope(slope) {}
    double get(const Sequence& seq) {
        return seq.weight() * _slope + _dist(_eng);
    }
};

class RMF_Uniform : public UniformGenerator {
  private:
    double _slope;

  public:
    RMF_Uniform(double slope, double a = 0, double b = 1)
        : UniformGenerator(a, b), _slope(slope) {}
    double get(const Sequence& seq) {
        return seq.weight() * _slope + _dist(_eng);
    }
};

class RandomLandscape : public FitnessLandscape {
  private:
    FitnessGenerator* _fgen;

  public:
    RandomLandscape(std::size_t loci, FitnessGenerator* fgen)
        : FitnessLandscape(loci), _fgen(fgen) {}
    void generateLandscape() {
        _landscape.clear();
        Sequence seq(_loci);
        _landscape[seq] = _fgen->get(seq);
        neighbourStepGenerator_impl(seq);
    }

  private:
    void neighbourStepGenerator_impl(const Sequence& seq) {
        double fitness_seq = _landscape.at(seq);
        for (std::size_t pos = 0; pos != _loci; ++pos) {
            Sequence tmp = seq;
            tmp.flip(pos);
            if (!_landscape.count(tmp)) {
                double fitness_tmp = _fgen->get(tmp);
                _landscape[tmp] = fitness_tmp;
                if (fitness_tmp > fitness_seq) {
                    neighbourStepGenerator_impl(tmp);
                }
            }
        }
    }
};

}
