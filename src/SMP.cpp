/*
    This file is part of Yuki.
    Copyright (C) 2017 Guofeng Dai

    Yuki is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Yuki is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Yuki.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "config.h"
#include "SMP.h"

#include <thread>

SMP::Mutex::Mutex() {
    m_lock = false;
}

bool SMP::Mutex::is_held() {
    return m_lock.load(std::memory_order_acquire);
}

SMP::Lock::Lock(Mutex & m) {
    m_mutex = &m;
    lock();
}

void SMP::Lock::lock() {
    while (m_mutex->m_lock.exchange(true, std::memory_order_acquire) == true);
}

void SMP::Lock::unlock() {
    m_mutex->m_lock.store(false, std::memory_order_release);
}

SMP::Lock::~Lock() {
    unlock();
}

int SMP::get_num_cpus() {
    return std::thread::hardware_concurrency();
}
