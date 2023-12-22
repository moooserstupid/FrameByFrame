#include "threadstream.h"

ThreadStream::ThreadStream(std::ostream& os) : os_(os)
{
    // copyfmt causes odd problems with lost output
    // probably some specific flag
//            copyfmt(os);
            // copy whatever properties are relevant
    imbue(os.getloc());
    precision(os.precision());
    width(os.width());
    setf(std::ios::fixed, std::ios::floatfield);

}
ThreadStream::~ThreadStream()
{
    std::lock_guard<std::mutex> guard(_mutex_threadstream);
    os_ << this->str();
}

std::mutex ThreadStream::_mutex_threadstream{};
