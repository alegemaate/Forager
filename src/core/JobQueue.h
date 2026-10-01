/// @file JobQueue.h
///
/// Runs background work. Native builds use a thread pool. Web builds have no threads, so the main thread runs
/// queued jobs for a short time each frame.
///
#pragma once

#include <chrono>
#include <deque>
#include <functional>
#include <thread>

#ifndef __EMSCRIPTEN__
#include "./ThreadPool.h"
#endif

class JobQueue
{
  public:
    /// @brief Number of jobs that can run at the same time
    static size_t workerCount()
    {
#ifdef __EMSCRIPTEN__
        return 1;
#else
        // Leave a core for the main thread
        const size_t cores = std::thread::hardware_concurrency();
        return cores > 2 ? cores - 1 : 1;
#endif
    }

    JobQueue()
#ifndef __EMSCRIPTEN__
        : pool(workerCount())
#endif
    {
    }

    void submit(std::function<void()> job)
    {
#ifdef __EMSCRIPTEN__
        queue.push_back(std::move(job));
#else
        pool.enqueue(std::move(job));
#endif
    }

    /// @brief Web builds run queued jobs here until the budget is spent. At least one job runs, so work always
    /// moves forward. Native builds do nothing, the workers run jobs.
    void runOnMainThread([[maybe_unused]] std::chrono::milliseconds budget)
    {
#ifdef __EMSCRIPTEN__
        const auto start = std::chrono::steady_clock::now();
        while (!queue.empty())
        {
            auto job = std::move(queue.front());
            queue.pop_front();
            job();

            if (std::chrono::steady_clock::now() - start >= budget)
            {
                break;
            }
        }
#endif
    }

    /// @brief Drop jobs that have not started
    void clear()
    {
#ifdef __EMSCRIPTEN__
        queue.clear();
#else
        pool.clear();
#endif
    }

    /// @brief Block until every started and queued job is done
    void wait()
    {
#ifdef __EMSCRIPTEN__
        runOnMainThread(std::chrono::hours(1));
#else
        pool.wait();
#endif
    }

  private:
#ifdef __EMSCRIPTEN__
    std::deque<std::function<void()>> queue;
#else
    ThreadPool pool;
#endif
};
