#ifndef TIMER_H
#define TIMER_H

#include "SDL2/SDL.h"


class Timer
{

    private:

        //if the timer has started
        bool started;

        //if the timer is paused
        bool paused;

        //Ticks when the timer began
        int startTicks;

        //Ticks when the timer was paused
        int pausedTicks;


    public:

        Timer( void );

        virtual ~Timer( void );

        //Start the timer
        void start( void );

        //Stop the timer
        void stop( void );

        //Pause the timer
        void pause( void );

        //Unpause the timer
        void unpause( void );

        //get the current ticks
        Uint32 getTicks( void );

        //Check if the timer is paused
        bool isPaused( void );

        //Check if the timer has started
        bool isStarted( void );
};

#endif // TIMER_H
