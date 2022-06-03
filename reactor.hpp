#include <vector>
#include <pthread.h>
#include <cstdlib>

using std::vector;

struct fd_handlers {

    int fd;
    void *runner;
    pthread_t thread;

};

class Reactor {
    public:
    vector<fd_handlers *> _fds;
    Reactor() = default;
    ~Reactor() = default;

    friend void *newReactor() {
        auto *newReactor = new Reactor();
        return newReactor;
    }

    friend void InstallHandler(Reactor *rec, void *_runner(void *), int *fd) {

        for (auto & _fd : rec->_fds) {
            if (_fd->fd == *fd) {
                _fd->runner = &_runner;
            }
        }

        auto *new_fd_handler = (struct fd_handlers *) malloc(sizeof(struct fd_handlers));
        new_fd_handler->fd = *fd;
        new_fd_handler->runner = &_runner;
        new_fd_handler->thread = 0;
        pthread_create(&new_fd_handler->thread, nullptr, _runner, (void *) fd);
        rec->_fds.push_back(new_fd_handler);
    }

    static void RemoveHandler(Reactor *reactor, int *fd) {
        for (auto & _fd : reactor->_fds) {
            if (_fd->fd == *fd) {
                _fd->runner = nullptr;
                pthread_cancel(_fd->thread);
            }
        }
    }
};