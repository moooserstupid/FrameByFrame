#ifndef THREADSTREAM
#define THREADSTREAM

#include <iostream>
#include <sstream>
#include <mutex>

//#define terr ThreadStream(std::cerr)
//#define tout ThreadStream(std::cout)

/**
 * Thread-safe std::ostream class.
 *
 * Usage:
 *    tout << "Hello world!" << std::endl;
 *    terr << "Hello world!" << std::endl;
 */
class ThreadStream : public std::ostringstream
{
public:
    ThreadStream(std::ostream& os);

    ~ThreadStream();
private:
    static std::mutex _mutex_threadstream;
    std::ostream& os_;
};

#endif