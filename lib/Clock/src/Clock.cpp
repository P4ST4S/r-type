/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** Clock
*/

#include "Clock.hpp"

Clock::Clock()
{
}

Clock::~Clock()
{
}

void Clock::cap(double tickPerSec)
{
    std::chrono::nanoseconds nsInterval(static_cast<size_t>(1e9 / tickPerSec));
    this->_prevGap = this->_gap;
    this->_gap = std::chrono::high_resolution_clock::now();
    this->_duration = this->_gap - this->_prevGap;
    if (tickPerSec <= 0) {
        this->updateDeltaTime();
        return;
    }
    if (this->_duration < nsInterval) {
        std::this_thread::sleep_for(nsInterval - this->_duration);
        this->_gap = std::chrono::high_resolution_clock::now();
        this->_realDuration = this->_duration;
        this->_duration = this->_gap - this->_prevGap;
    } else {
        this->_realDuration = this->_duration;
    }
    this->updateDeltaTime();
}

double Clock::getDeltaTime(void) const
{
    return (this->_dt);
}

size_t Clock::getNsTickDuration(void) const
{
    return (this->_duration.count());
}

size_t Clock::getRealNsTickDuration(void) const
{
    return (this->_realDuration.count());
}

void Clock::updateDeltaTime(void)
{
    this->_dt = static_cast<double>(this->_duration.count()) / 1e9;
}
