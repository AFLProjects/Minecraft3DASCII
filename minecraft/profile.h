/* /*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2013 CERN
 * @author Tomasz Wlostowski <tomasz.wlostowski@cern.ch>
 * 2019 KiCad Developers, see AUTHORS.txt for contributors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

/**
 * @file profile.h:
 * @brief Simple profiling functions for measuring code execution time.
 */

#ifndef TPROFILE_H
#define TPROFILE_H

#include <chrono>
#include <string>
#include <iostream>
#include <iomanip>
using namespace std;

/**
 * The class PROF_COUNTER is a small class to help profiling.
 * It allows the calculation of the elapsed time (in milliseconds) between
 * its creation (or the last call to Start() ) and the last call to Stop()
 */
class PROF_COUNTER
{
public:
    /**
     * Creates a PROF_COUNTER for measuring an elapsed time in milliseconds
     * @param aName = a string that will be printed in message.
     * @param aAutostart = true (default) to immediately start the timer
     */
    PROF_COUNTER( const std::string& aName, bool aAutostart = true ) :
        m_name( aName ), m_running( false )
    {
        if( aAutostart )
            Start();
    }

    /**
     * Creates a PROF_COUNTER for measuring an elapsed time in milliseconds
     * The counter is started and the string to print in message is left empty.
     */
    PROF_COUNTER()
    {
        Start();
    }

    /**
     * Starts or restarts the counter
     */
    void Start()
    {
        m_running = true;
        m_starttime = CLOCK::now();
        m_lasttime = m_starttime;
    }


    /**
     * save the time when this function was called, and set the counter stane to stop
     */
    void Stop()
    {
        if( !m_running )
            return;

        m_stoptime = CLOCK::now();
        m_running = false;
    }

    /**
     * Print the elapsed time (in a suitable unit) to a stream.
     *
     * The unit is automatically chosen from ns, us, ms and s, depending on the
     * size of the current count.
     *
     * @param the stream to print to.
     */
    string Show()
    {
		string out = "";
        using DURATION = std::chrono::duration<double, std::nano>;

        const auto   duration = SinceStart<DURATION>();
        const double cnt = duration.count();

        if( m_name.size() )
        {
            out += m_name + " took ";
        }

        if( cnt < 1e3 )
			out += std::to_string(cnt) + "ns";
        else if( cnt < 1e6 )
			out += std::to_string(cnt / 1e3) + "us";
        else if( cnt < 1e9 )
			out += std::to_string(cnt / 1e6) + "ms";
        else
			out += std::to_string(cnt / 1e9) + "s";

		out += "\n";
		return out;
    }

    string fps() 
    {
        using DURATION = std::chrono::duration<double, std::nano>;
        return (std::to_string(round(1.0 / (this->SinceStart<DURATION>().count() / 1e9))) + " FPS");
    }

    int time() 
    {

    }

    /**
     * @return the time since the timer was started. If the timer is stopped,
     * the duration is from the start time to the time it was stopped, else it
     * is to the current time.
     */
    template <typename DURATION>
    DURATION SinceStart( bool aSinceLast = false )
    {
        const TIME_POINT stoptime = m_running ? CLOCK::now() : m_stoptime;
        const TIME_POINT starttime = aSinceLast ? m_lasttime : m_starttime;

        m_lasttime = stoptime;

        return std::chrono::duration_cast<DURATION>( stoptime - starttime );
    }

    /**
     * @param aSinceLast: only get the time since the last time the time was read
     * @return the elapsed time in ms since the timer was started.
     */
    double msecs( bool aSinceLast = false )
    {
        using DUR_MS = std::chrono::duration<double, std::milli>;
        return SinceStart<DUR_MS>( aSinceLast ).count();
    }

private:
    std::string m_name;     // a string printed in message
    bool m_running;

    using CLOCK = std::chrono::high_resolution_clock;
    using TIME_POINT = std::chrono::time_point<CLOCK>;

    TIME_POINT m_starttime, m_lasttime, m_stoptime;
};


/**
 * A simple RAII class to measure the time of an operation.
 *
 * On construction, a timer is started, and on destruction, the timer is
 * ended, and the time difference is written into the given duration.
 *
 * For example:
 *
 * DURATION duration; // select a duration type as needed
 * {
 *     SCOPED_PROF_COUNTER<DURATION> timer( duration );
 *     timed_activity();
 * }
 * // duration is now the time timed activity took
 *
 * From C++17, with class template argument deduction, you should be able to
 * omit the <DURATION>.
 */
template <typename DURATION>
class SCOPED_PROF_COUNTER
{
public:
    SCOPED_PROF_COUNTER( DURATION& aDuration ) : m_counter(), m_duration( aDuration )
    {
    }

    ~SCOPED_PROF_COUNTER()
    {
        // update the output
        m_duration = m_counter.SinceStart<DURATION>();
    }

private:
    ///< The counter to use to do the profiling
    PROF_COUNTER m_counter;

    ///< The duration to update at the end of the scope
    DURATION& m_duration;
};


/**
 * Function GetRunningMicroSecs
 * An alternate way to calculate an elapset time (in microsecondes) to class PROF_COUNTER
 * @return an ever increasing indication of elapsed microseconds.
 * Use this by computing differences between two calls.
 * @author Dick Hollenbeck
 */
unsigned GetRunningMicroSecs();

#endif  // TPROFILE_H