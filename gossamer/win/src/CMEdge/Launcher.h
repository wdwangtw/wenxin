
#ifndef _LAUNCHER_H_
#define _LAUNCHER_H_

class CLauncher
{
public:
    void Run();

private:
    void RunAsCmdLine();
    void RunAsService();
};

#endif //!_LAUNCHER_H_
