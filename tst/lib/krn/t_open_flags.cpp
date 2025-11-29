// ---------------------------------------------------------------------------
// - t_open_flags.cpp                                                         -
// - regression test for Linux open flags mapped on macOS                     -
// ---------------------------------------------------------------------------

#include "KrnFcntl.hxx"

#include <cassert>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>

int main(int, char**) {
  using namespace iato;

  const char* path = "t_flag_open_file";
  int fd = ::open(path, O_CREAT | O_WRONLY, 0644);
  assert(fd >= 0);
  ::close(fd);

  const t_octa linux_flags = KRN_O_RDONLY | KRN_O_DIRECTORY;
  const t_octa host_flags = map_open_flags(linux_flags);

#ifdef O_DIRECTORY
  assert((host_flags & O_DIRECTORY) == O_DIRECTORY);
#endif

  errno = 0;
  int dir_fd = ::open(path, static_cast<int>(host_flags));
  assert(dir_fd == -1);
  assert(errno == ENOTDIR);
  ::unlink(path);

  return 0;
}


