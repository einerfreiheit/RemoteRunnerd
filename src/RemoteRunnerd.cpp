#include "Configuration.hpp"

#include "Communication/Server.hpp"

int main(int argc, char** argv) {
    using namespace remote_runnerd;
    Configuration configuration(argc, argv);
    Server server(configuration);
    server.start();
    return 0;
}
