#ifndef PACP_BACKEND
#define PACP_BACKEND

class CPACP
{
public:
    CPACP() {};
    ~CPACP() {};

public:
    bool init();
    bool initLog();
    bool initDlopen();
    bool initSocketMgr();

private:
    
};




#endif