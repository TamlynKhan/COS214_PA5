#ifndef OPERATORCONSOLE_H
#define OPERATORCONSOLE_H

#include <string>
#include <vector>

class OperatorCommand;

// TODO(Facade): facade workflows should queue commands here and call DispatchCentre::setStrategy rather than calling receivers directly
class OperatorConsole
{
    public:
        explicit OperatorConsole(const std::string& operatorName);
        ~OperatorConsole();

        OperatorConsole(const OperatorConsole&) = delete;
        OperatorConsole& operator=(const OperatorConsole&) = delete;

        void queueCommand(OperatorCommand* command);
        void executePending();
        bool cancelLast();

    private:
        std::string operatorName;
        std::vector<OperatorCommand*> pending;
        std::vector<OperatorCommand*> history;
};

#endif
