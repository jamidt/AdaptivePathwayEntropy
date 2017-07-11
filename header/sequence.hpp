#pragma once

#include <iostream>
#include <vector>

namespace adaptive {
class Sequence {
  private:
    std::vector<bool> _seq;
    std::size_t _loci;

  public:
    Sequence() = delete;
    Sequence(const std::vector<bool>& seq) : _seq(seq), _loci(seq.size()) {}
    Sequence(std::vector<bool>&& seq)
        : _seq(std::move(seq)), _loci(_seq.size()) {}
    Sequence(std::size_t loci) : _seq(loci), _loci(loci) {}
    Sequence(Sequence&& seq) : _seq(std::move(seq._seq)), _loci(seq._loci) {}
    Sequence(const Sequence& seq) = default;

    Sequence& operator=(const Sequence& seq) = default;
    Sequence& operator=(Sequence&& seq) = default;

    Sequence& flip(std::size_t pos) {
        _seq[pos] = !_seq[pos];
        return *this;
    }
    Sequence& flip() {
        for (std::size_t i = 0; i != _seq.size(); ++i) {
            flip(i);
        }
        return *this;
    }
    Sequence& reset(std::size_t pos) {
        _seq[pos] = false;
        return *this;
    }
    Sequence& reset() {
        for (std::size_t i = 0; i != _seq.size(); ++i) {
            reset(i);
        }
        return *this;
    }

    bool operator[](std::size_t pos) const { return _seq[pos]; }
    bool at(std::size_t pos) const { return _seq.at(pos); }

    bool is_one(std::size_t pos) const { return _seq[pos] == 1; }
    bool is_one() const {
        for (const auto& b : _seq) {
            if (b == 0) {
                return false;
            }
        }
        return true;
    }

    bool is_zero(std::size_t pos) const { return _seq[pos] == 0; }
    bool is_zero() const {
        for (const auto& b : _seq) {
            if (b == 1) {
                return false;
            }
        }
        return true;
    }

    std::size_t size() const { return _seq.size(); }

    std::size_t weight() const { return this->distance(); }
    std::size_t distance() const {
        std::size_t count = 0;
        for (std::size_t pos = 0; pos != _loci; ++pos) {
            if (_seq[pos]) ++count;
        }
        return count;
    }
    std::size_t distance(const Sequence& seq2) const {
        std::size_t count = 0;
        for (std::size_t pos = 0; pos != _loci; ++pos) {
            if (_seq[pos] != seq2[pos]) ++count;
        }
        return count;
    }

    friend bool operator==(const Sequence&, const Sequence&);
    friend bool operator!=(const Sequence&, const Sequence&);

    friend std::ostream& operator<<(std::ostream&, const Sequence&);

    friend struct std::hash<Sequence>;
};

bool operator==(const Sequence& seq1, const Sequence& seq2) {
    return seq1._seq == seq2._seq;
}

bool operator!=(const Sequence& seq1, const Sequence& seq2) {
    return !(seq1 == seq2);
}

std::ostream& operator<<(std::ostream& out, const Sequence& seq) {
    for (std::size_t pos = 0; pos != seq.size(); ++pos) {
        out << seq[pos];
    }
    return out;
}

std::size_t hamming_distance(const Sequence& seq1, const Sequence& seq2) {
    return seq1.distance(seq2);
}
}

namespace std {
template <>
struct hash<adaptive::Sequence> {
    std::size_t operator()(const ::adaptive::Sequence& seq) const {
        return std::hash<std::vector<bool>>{}(seq._seq);
    }
};
}
