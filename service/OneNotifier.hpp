#ifndef ZT_ONENOTIFIER_HPP
#define ZT_ONENOTIFIER_HPP

#include <string>
#include <vector>

namespace ZeroTier {

/**
 * Service state notifier
 */
class OneNotifier
{
public:
        bool isUp(); // return currently set value
        void ifUp(bool forced = false); // call script with arg up
        void ifDown(bool forced = false); // call script with arg down
        void exit(); // ifDown and keep it that way
        void reload(std::string script); // reload
	void setIfd(std::string ifd);

        OneNotifier() : _ifd(""), _script(""), _usable(false), _exiting(false), _up(false) {};

private:
        std::string _ifd, _script;
        bool _usable, _exiting, _up;

	bool isExec(std::string script, bool forced = false);
	std::string cmd(bool state);
        void execute(bool state);
};

extern OneNotifier notifier;

} // end of ZeroTier ns

#endif
