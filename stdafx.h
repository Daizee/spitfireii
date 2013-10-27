// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdint.h>
#include <cstdlib>
#include <lua/lua.hpp>
#include <lua/lauxlib.h>
#include <signal.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <tchar.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <thread>
#ifndef WIN32
#include <pthread.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <dlfcn.h>
#include <glob.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/epoll.h>
//#include <sys/mman.h>
#include <sys/param.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <unistd.h>
#include <inttypes.h>
#else
#include <intrin.h>
#endif
#include <assert.h>
#include <algorithm>
#include <cctype>
#include <errno.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <string>
#include <sstream>
#include <sys/stat.h>
#include <sys/timeb.h>
#include <vector>
#include <sys/types.h>
#include <memory.h>
#include <math.h>
#include <mutex>






// TODO: reference additional headers your program requires here
