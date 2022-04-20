#include <csignal>
#include <boost/program_options.hpp>

#include "cross.h"
#include "solver.h"

namespace po = boost::program_options;
using namespace jsolver;

int main(int argc, char **argv)
{
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help,h", "produce this help message")
            ("input,in,i", po::value<std::string>(), "Load picture and convert to cross")
            ("output,out,o", po::value<std::string>(), "Saving path")
            ("size,s", po::value<size_t>(), "Resize loaded picture")
            ;

    po::positional_options_description p;

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    po::notify(vm);

    if (vm.count("help") || argc < 2)
    {
        std::cout << desc << std::endl;
        return 0;
    }

    if (vm.count("input"))
    {
        size_t size = vm.count("size") ? vm["size"].as<size_t>() : 20;
        Cross cross;
        cross.load(vm["input"].as<std::string>(), size);

        if (vm.count("output"))
        {
            cross.write(vm["output"].as<std::string>());
        }

        Solver solver(cross);
        solver.solve();
    }




    // signal(SIGCHLD, SIG_DFL);
    // while (true)
    // {
    //     sleep(1);
    // }

    return 0;
}