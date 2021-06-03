#include "Communication/Server.hpp"
#include "Configuration.hpp"

int main(int argc, char** argv) {
    using namespace remote_runnerd;
    Configuration configuration(argc, argv);
    Server server(configuration);
    server.start();
    return 0;
}
