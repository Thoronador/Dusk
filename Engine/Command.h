#ifndef COMMAND_H
#define COMMAND_H

namespace Dusk
{
    class Scene;
    class Command
    {
    public:
        Command();
        virtual ~Command();
        void execute();

        virtual bool execute(Dusk::Scene* scene, int count = 1) = 0;
    protected:
    private:
        int m_Count; // contains the number how many times the Command shall be executed
    };
}


#endif // COMMAND_H
