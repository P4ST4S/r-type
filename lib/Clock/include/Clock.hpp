/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** Clock
*/

#pragma once

#include <iostream>
#include <chrono>
#include <thread>

/**
 ** @brief This class is used to cap the number of
 ** iteration person second in a loop
 **
 */
class Clock {
public:
    Clock();
    ~Clock();

    /**
     ** @brief This method will cap the number of iteration
     ** The method will sleep if the time
     ** between the last call and this one is too short.
     ** It will otherwise do nothing.
     **
     ** @param tickPerSec The number of iteration per second. Or
     ** -1 to disable the cap.
     */
    void cap(double tickPerSec);

    /**
     ** @brief This method will return the delta time coefficient.
     ** 1 is the normal speed, 0.5 is twice the normal speed, 2 is half the normal speed.
     ** The delta time is equal to 1 when there's 1 tick per second.
     **
     ** @return double Delta time coefficient
     */
    double getDeltaTime(void) const;

    /**
     ** @brief Return the number of nanoseconds between the last two calls of cap.
     **
     ** @return size_t
     */
    size_t getNsTickDuration(void) const;

    /**
     ** @brief Return the number of nanoseconds between the last two calls of cap.
     ** This value is not affected by the cap.
     **
     ** @return size_t
     */
    size_t getRealNsTickDuration(void) const;

protected:
    void updateDeltaTime(void);

    std::chrono::_V2::system_clock::time_point _gap = std::chrono::high_resolution_clock::now();
    std::chrono::_V2::system_clock::time_point _prevGap = std::chrono::high_resolution_clock::now();
    std::chrono::duration<size_t, std::nano> _duration = this->_gap - this->_prevGap;
    std::chrono::duration<size_t, std::nano> _realDuration = this->_gap - this->_prevGap;
    double _dt = 1;
private:
};
