#include "InstructionProcessor.h"
#include "Messaging/ZMQServer.h"
#include "TaskRunner.h"
#include <csignal>
#include <iostream>
#include <unistd.h>

static int receivedSignal = 0;
void signalHander(int i)
{
    receivedSignal = 1;
};

struct Options 
{
    size_t timeout {0};
    std::string addr {"tcp://127.0.0.1:12345"};
};

Options readOptions(int argc, char** argv)
{
    int token;
    Options result;
    while ((token = getopt(argc, argv, "a:t:")) != -1) {
        switch (token) {
        case 'a':
            result.addr = std::string(optarg);
            break;
        case 't':
        {
            std::stringstream ss(optarg);
            ss >> result.timeout;
            break;
        }
        default:
            std::cerr<<"usage: [-t timeout] [-a protocol://end-point]\n";
            exit(1);
        }
    }
    return result;
}

int main(int argc, char** argv)
{
    Options options = readOptions(argc, argv);
    ZMQServer server(options.addr);
    server.connect();
    TaskRunner runner;
    runner.timeout = options.timeout;
    InstructionProcessor processor("/etc/remote-runnerd.conf");
    processor.readPermissions();
    signal(SIGHUP, signalHander);
    while (1) {
        auto res = server.receive();
        if (!res.content.empty()) {
            std::vector<std::string> splitted = processor.split(res.content);
            if (processor.isCorrect(splitted))
                runner.runTask(splitted, res.identifier);
        }
        std::vector<TaskPtr> results = runner.getResults();
        for (const auto& res : results) {
            server.send({res->id, res->output});
        }
        if (receivedSignal) {
            processor.readPermissions();
            receivedSignal = 0;
        }
        usleep(100 * 1000);
    }
    return 0;
}
