#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <tuple>

using namespace std;

namespace Utils {

namespace Process {

	tuple<bool, string> Exec(const string& cmd);

}
}

#endif // PROCESS_H
