# Component Configurator Studio

## Q1:

Bingxin Liu

## Q2:

```c++
// ACE_Task_Base is directly inheritating from ACE_Service_Object
class service_wrapper : public ACE_Task_Base
```

## Q3:

```shell
# OUTPUT:
[bingxin.liu@linuxlab002 ACESimpleClient]$ ./ACESimpleClient
[client1|0x7ffc68ca48f0]
[client1|0x7ffc68ca48f0][client2|0x7ffc68ca4830]
Error: No command line arguments.
handle signal
handle close with SIGNAL_MASK
handle close with TIMER_MASK
[bingxin.liu@linuxlab002 ACESimpleClient]$
```

We allocate two service handler object and register them into singleton data structure seperately as client1 and client2

## Q4:

We redirect the standard ouput to a file named output.log and in another window use tail -f to follow the ouput, so that we can seperate input and ouput. The result is shown below:

```shell
#INPUT
[bingxin.liu@linuxlab002 ACESimpleClient]$ ./ACESimpleClient 1> output.log
rename 7ffc52289f80 newname

[bingxin.liu@linuxlab002 ACESimpleClient]$

#OUTPUT
[bingxin.liu@linuxlab002 ACESimpleClient]$ tail -f output.log
[client1|0x7ffc52289f80]
[client1|0x7ffc52289f80][client2|0x7ffc52289ec0]
Error: No command line arguments.
Error: No command line arguments.
Error: No command line arguments.
Error: No command line arguments.
Error: No command line arguments.
Error: No command line arguments.
Error: No command line arguments.
Error: No command line arguments.
[newname|0x7ffc52289f80][client2|0x7ffc52289ec0]
Error: No command line arguments.
handle signal
handle close with SIGNAL_MASK
handle close with TIMER_MASK
release ui

[bingxin.liu@linuxlab002 ACESimpleClient]$
```

We can find that the client1 is renamed as newname

## Q5:

1. Summarize how you implemented each of the commands:

    init() function:
    new a event handler and initialize it (not register it).
    new an ACE_Thread_Mutex and an ACE_Condition, which will be used to control the behavour of porgram in the state of finish.
    Then use ACE's activate function to run the service in a new thread.

    fini() function:
    Set the finish flag and end the reactor's busy loop with end_reator_event_loop() function.
    Wait until service release its dynamically allocated resources.
    Then release mutex and candition variable.

    suspend() function:
    first cancel the timer, which is used as shoting a connection request to server ever few seconds.
    Then handle suspend by reactor's suspend_handler() function and ACE_Task_Base's suspend static function. Now the service has been suspend.
    Set the suspend flag.

    resume() function:
    Reset the timer and resume the reactor via reactor's resume_handler and ACE_Task_Base's resume function.
    clear suspend flag.

    svc() function:
    Basically just the logic of client. Except that after break from the reactor's busy loop, we stop the reactor by its reactor_event_loop_done() function and reset the reactor via its reset_reactor_event_loop() function.
    Then we clear finish flag and notice fini() function via condition variable, so that fini() function can return.

    info() function:
    Just print out the suspend flag.

2. show representative output demonstrating the use of each command.
    ```shell
    # STDIN:
    [bingxin.liu@linuxlab002 ACESimpleClient]$ ./ACESimpleClient 1> output.log
    aaa         # error command
    init        # init service
    suspend     # suspend
    info        # print state
    resume      # resume service
    fini        # stop service
    init        # restart service
    fini        # stop service
    quit        # quit from UI
    [bingxin.liu@linuxlab002 ACESimpleClient]$
    
    # STDOUT:
    [bingxin.liu@linuxlab002 ACESimpleClient]$ tail -f output.log
    # error command
    Can not parse the command [aaa]
    # init service
    Starting up the time Service at 47040827105920
    start svc at at 47040854353664      # svc()
    [client service|0x7ffe14096e90]     # register singleton
    # connect periodically
    Error: No command line arguments.   
    Error: No command line arguments.
    Error: No command line arguments.
    # suspend
    Time Service has been suspended
    # info()
    SUSPEND
    # resume
    Resuming Time Service
    # connect periodically
    Error: No command line arguments.
    Error: No command line arguments.
    Error: No command line arguments.
    # stop service
    FINISH!Closing down the Time Service
    # restart service
    Starting up the time Service at 47040827105920
    start svc at at 47040854353664
    [client service|0x7ffe14096e90]
    Error: No command line arguments.
    Error: No command line arguments.
    Error: No command line arguments.
    # stop service
    FINISH!Closing down the Time Service
    # quit from UI
    ui released
    release service
    release client
    [bingxin.liu@linuxlab002 ACESimpleClient]$
    ```

