#ifndef EVENTS_H
#define EVENTS_H
#include <cstddef>
class Events
{
  private:
  public:
    size_t worker_connections;
    Events();
    Events(size_t worker_connections);
    ~Events();
};

Events::Events() : worker_connections(1024)
{
}

Events::Events(size_t worker_connections) : worker_connections(worker_connections)
{
}

Events::~Events()
{
}

#endif /* EVENTS_H */
