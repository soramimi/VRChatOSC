#ifndef OSC_H
#define OSC_H

#include <cstdint>
#include <functional>
#include <string>

namespace osc {

class Value {
	friend class Receiver;
public:
	enum class Type {
		Void,
		Bool,
		Int,
		Float,
	};
private:
	Type type_ = Type::Void;
	union {
		bool b;
		int32_t i;
		float f;
	} v;
public:
	Type type() const
	{
		return type_;
	}
	bool bool_value() const
	{
		return v.b;
	}

	int32_t int_value() const
	{
		return v.i;
	}

	float float_value() const
	{
		return v.f;
	}
};

struct Listener {
	std::function<void(char const *, int)> received;
	std::function<void(std::string const &addr, Value const &)> value;
};

class Transmitter {
private:
	struct Private;
	Private *m;
public:
	Transmitter();
	~Transmitter();

	void open(char const *hostname);
	void close();

	void send_bool(const std::string &addr, bool val);
	void send_int(const std::string &addr, int32_t val);
	void send_float(const std::string &addr, float val);
};

class Receiver {
private:
	struct Private;
	Private *m;

	void run();
	bool isInterruptionRequested() const;
	void start();
	void stop();
public:
	Receiver();
	~Receiver();
	Listener *set_listener(Listener *listener);
	void open(char const *hostname);
	void close();
};

} // namespace osc

#endif // OSC_H
