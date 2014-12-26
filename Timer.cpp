#include "Timer.h"

//Initialises the variables
Timer::Timer()
{
    //ctor
}

Timer::~Timer()
{
    //dtor
}

/*Check if the timer hasnt already been started
if so do nothing else set the start ticks to the current
ticks and set started to true and paused to false*/
void Timer::start( void )
{
    startTicks = SDL_GetTicks();
    started = true;
    paused = false;

}

//Set startTicks to zero started to false and paused to false
void Timer::stop( void )
{
    if( started == true )
    {
        startTicks = 0;
        started = false;
        paused = false;
    }

}

//check if not paused and if started if so Set paused ticks to the current ticks subtract startTicks then set the started ticks to zero
//paused to true and started to true
void Timer::pause( void )
{
    if( (started == true) && (paused == false) )
    {
            paused = true;

            //gets the difference between when the timer started
            //and when it got paused so we can "reset" the startTicks to the
            //same distance from the current ticks
            pausedTicks = SDL_GetTicks() - startTicks;
    }
}

//Check the timer has started and is paused set the start ticks to
//current ticks subtract paused ticks set paused to false and pausedTicks to 0
void Timer::unpause( void )
{
    if( (started == true) && (paused == true) )
    {
        paused = false;

        startTicks = SDL_GetTicks() - pausedTicks;

        pausedTicks = 0;
    }
}

Uint32 Timer::getTicks( void )
{
    if( started == true )
    {
        if( paused == true )
        {
            return pausedTicks;
        }
        else
        {
            return SDL_GetTicks() - startTicks;
        }
    }

    return 0;
}

//Check if the timer is paused
bool Timer::isPaused( void )
{
    return paused;
}

//Check if the timer has started
bool Timer::isStarted( void )
{
    return started;
}
