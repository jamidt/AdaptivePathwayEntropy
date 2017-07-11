#include "../../adaptive.hpp"

#include <fstream>
#include <sstream>

#include "boost/program_options.hpp"

namespace po = boost::program_options;
using namespace adaptive;

int main(int argc, char** argv) {
    try {
        std::size_t loci;
        long long runs;
        double start;

        po::options_description desc("House of Cards Entropy for the undirected case");
        desc.add_options()("help,h", "Print help")
            ("loci,l", po::value<std::size_t>(&loci)->required(), "Loci")
            ("runs,r", po::value<long long>(&runs)->default_value(1000), "Runs")
            ("start,s", po::value<double>(&start)->required(), "Set start value for (00...0)");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);

        if (vm.count("help")) {
            std::cout << desc << '\n';
            return 0;
        }

        po::notify(vm);

        UniformGeneratorStart fgen(start);
        RandomLandscape landscape(loci, &fgen);
        PathProbabilities pathprob(&landscape);

        std::stringstream filename;
        filename << "data-" << loci << '-' << start << ".txt";
        std::ofstream data(filename.str());

        data << "# House of Cards model\n"
             << "# loci = " << loci << '\n'
             << "# start = " << start << '\n'
             << "# runs = " << runs << '\n'
             << "# path_entropy endpoint_entropy conditional_entropy\n";

        for(long long count = 0; count != runs; ++count) {
            landscape.generateLandscape();
            pathprob.calcPathProb();
            data << path_entropy(pathprob) << ' ' << endpoint_entropy(pathprob) << ' ' << conditional_entropy(pathprob) << '\n';
        }
        data.close();

    } catch (po::required_option& error) {
        std::cerr << error.what() << '\n';
    }
    return 0;
}
