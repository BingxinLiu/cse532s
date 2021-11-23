#include "Services.h"

　

int TimeService::init(int argc, char *argv[])

{

ACE_DEBUG((LM_DEBUG,"(%t)Starting up the time Service\n"));

mutex_ = new ACE_Thread_Mutex;

cancel_cond_ = new ACE_Condition<ACE_Thread_Mutex>(*mutex_);

activate(THR_NEW_LWP|THR_DETACHED);

return 0;

}

　

int TimeService::fini(void)

{

ACE_DEBUG((LM_DEBUG,

"(%t)FINISH!Closing down the Time Service\n"));

　

//All of the following code is here to make sure that the

//thread in the task is destroyed before the service configurator

//deletes this object.

canceled_=1;

mutex_->acquire();

while(canceled_)

cancel_cond_->wait();

mutex_->release();

ACE_DEBUG((LM_DEBUG,"(%t)Time Service is exiting \n"));

　

return 0;

}

　

//Suspend the Time Service.

int TimeService::suspend(void)

{

ACE_DEBUG((LM_DEBUG,"(%t)Time Service has been suspended\n"));

int result=ACE_Task_Base::suspend();

return result;

}

　

//Resume the Time Service.

int TimeService::resume(void)

{

ACE_DEBUG((LM_DEBUG,"(%t)Resuming Time Service\n"));

int result=ACE_Task_Base::resume();

return result;

}

　

//The entry function for the thread. The tasks underlying thread

//starts here and keeps sending out messages. It stops when:

// a) it is suspeneded

// b) it is removed by fini(). This happens when the fini() method

// sets the cancelled_ flag to true. Thus causes the TimeService

// thread to fall through the while loop and die. Before dying it

// informs the main thread of its imminent death. The main task

// that was previously blocked in fini() can then continue into the

// framework and destroy the TimeService object.

int TimeService::svc(void)

{

char *time = new char[36];

while(!canceled_)

{

ACE::timestamp(time,36);

ACE_DEBUG((LM_DEBUG,"(%t)Current time is %s\n",time));

ACE_OS::fflush(stdout);

ACE_OS::sleep(1);

}

　

//Signal the Service Configurator informing it that the task is now

//exiting so it can delete it.

canceled_=0;

cancel_cond_->signal();

ACE_DEBUG((LM_DEBUG,

"Signalled main task that Time Service is exiting \n"));

return 0;

}

　

//Define the object here

TimeService time_service;