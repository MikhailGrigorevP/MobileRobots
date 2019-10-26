#include "Module.h"

namespace Modules_N {
	//! turn module on
	void Module::on() {
		state = 1;
	};
	//! turn module off
	void Module::off() {
		state = 0;
	};
}