// OneNotifier

#include <string>
#include <cstdlib>
#include <unistd.h>
#include <mutex>
#include <thread>
#include <sys/stat.h>
#include "OneService.hpp"
#include "OneNotifier.hpp"

namespace ZeroTier {

static std::mutex notifier_mutex;

bool OneNotifier::isExec(std::string script, bool forced) {
	return !script.empty() && ( forced || this -> _usable ) ? !access(script.c_str(), X_OK) : false;
}

std::string OneNotifier::cmd(bool state) {
	return this -> _script + " " + ( this -> _ifd.empty() ? "" : ( this -> _ifd + " " )) +
		( state ? "up" : "down" ) + " &";
}

void OneNotifier::reload(std::string script) {
	std::lock_guard<std::mutex> guard(notifier_mutex);
	this -> _up = false;
	this -> _usable = this -> isExec(script, true);
	this -> _script = this -> _usable ? script : "";
}

void OneNotifier::setIfd(std::string ifd) {
	std::lock_guard<std::mutex> guard(notifier_mutex);
	this -> _ifd = ifd;
}

bool OneNotifier::isUp() {
	std::lock_guard<std::mutex> guard(notifier_mutex);
	return this -> _up;
}

void OneNotifier::ifUp(bool forced) {
	std::lock_guard<std::mutex> guard(notifier_mutex);
	if (( !forced && this -> _up ) || this -> _exiting ) return;
	this -> _up = true;
	this -> execute(true);
}

void OneNotifier::ifDown(bool forced) {
	std::lock_guard<std::mutex> guard(notifier_mutex);
	if (( !forced && !this -> _up ) || this -> _exiting ) return;
	this -> _up = false;
	this -> execute(false);
}

void OneNotifier::exit() {
	std::lock_guard<std::mutex> guard(notifier_mutex);
	if ( this -> _exiting ) return;
	this -> _exiting = true;
	this -> _up = false;
	this -> execute(false);
}

void OneNotifier::execute(bool state) {
	if ( this -> _usable && isExec(this -> _script) && std::system(NULL) != 0 )
		std::system(this -> cmd(state).c_str());
}

} // end of namespace ZeroTier

// global
ZeroTier::OneNotifier ZeroTier::notifier;
