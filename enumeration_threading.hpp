#pragma once

#include <vector>
#include <thread>
#include <atomic>

namespace matsulib
{
  namespace enumerable_threading
  {
    template <class Container, class Function>
    auto for_each(
      Container &&values,
      std::size_t number_of_values,
      unsigned int number_of_thread,
      Function &&function)
      -> void
    {
      // check number of thread
      if (number_of_thread <= 0)
      {
        number_of_thread = std::max <unsigned int>({ std::thread::hardware_concurrency(), 1 });
      }

      // create workers
      std::vector <std::thread> workers;
      workers.resize(number_of_thread);

      // loop counter
      std::atomic_size_t task_id = 0;

      // assign work
      for (auto &worker : workers)
      {
        worker = std::thread{ [&]() {
          for (auto i = task_id.fetch_add(1); i < number_of_values; i = task_id.fetch_add(1))
          {
            function(values[i]);
          }
        } };
      }

      // execute parallel
      for (auto &worker : workers)
      {
        worker.join();
      }
    }

    template <class Container, class Function>
    inline auto for_each(Container &&values, unsigned int number_of_thread, Function &&function) -> void
    {
      for_each(
        std::forward <Container>(values),
        values.size(),
        number_of_thread,
        std::forward <Function>(function));
    }

    template <class Container, class Function>
    inline auto for_each(Container &&values, Function &&function) -> void
    {
      for_each(
        std::forward <Container>(values),
        0,
        std::forward <Function>(function));
    }
  }
}