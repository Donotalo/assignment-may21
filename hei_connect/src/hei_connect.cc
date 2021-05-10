#include <napi.h>
#include <string>
#include <iostream>
#include <thread>
#include <random>
#include <chrono>

/// <summary>
/// This class represents a Hei Connect device. It exposes APIs to communicate with the device.
/// </summary>
class HeiConnectDevice
{
	// Stirrer speed in RPM
	int32_t stirrer_speed_rpm;

	// Following variabels are used to generate random number to simulate device's communication deley
	std::mt19937 eng;
	std::uniform_int_distribution<std::mt19937::result_type> dist;

	/// <summary>
	/// Sends a command to the device. This is a dummy function prepared for the purpose of the assignment.
	/// </summary>
	/// <param name="command">Command to send.</param>
	/// <param name="input">Parameter to send.</param>
	/// /// <param name="output">Device response is stored in this parameter.</param>
	/// <returns>Returns true if command execution is successful, false otherwise.</returns>
	bool SendCommand(const std::string& command, int32_t input, int32_t& output)
	{
		// Random wait to simulate device operation
		std::this_thread::sleep_for(std::chrono::milliseconds(dist(eng)));
		return true;
	}

public:

	/// <summary>
	/// Constructor
	/// </summary>
	HeiConnectDevice() : stirrer_speed_rpm(0), eng(std::mt19937(std::random_device()())), dist(250, 1500)
	{ }

	/// <summary>
	/// This function will retrieve current stirrer speed in RPM.
	/// </summary>
	/// <returns>Stirrer speed in RPM. Returns -1 if command execution is failed.</returns>
	int32_t GetStirrerSpeed()
	{
		int32_t dummy = -1;
		if (SendCommand("get-speed", 0, dummy))
		{
			return stirrer_speed_rpm;
		}

		return -1;
	}

	/// <summary>
	/// Sets stirrer speed with the given parameter.
	/// </summary>
	/// <param name="speed_rpm">Stirrer speed to set.</param>
	/// <returns>Returns true if stirrer speed is successfully set, false otherwise.</returns>
	bool SetStirrerSpeed(int32_t speed_rpm)
	{
		bool success = false;
		int32_t dummy = -1;

		if (speed_rpm >= 0 && speed_rpm <= 1400 &&
			SendCommand("set-speed", speed_rpm, dummy))
		{
			stirrer_speed_rpm = speed_rpm;
			success = true;
		}

		return success;
	}
};

/// <summary>
/// This is a response class. All addon responses should be wrapped by an instance of this class.
/// 
/// (I couldn't make it work within the allocated time.)
/// </summary>
class Response
{
	bool success;
	int32_t param;

public:

	/// <summary>
	/// Constructor.
	/// </summary>
	/// <param name="s">Represents success status.</param>
	/// <param name="p">Used to send any parameter to Node.js environment.</param>
	Response(bool s = false, int32_t p = -1) : success(s), param(p)
	{}
};

/// <summary>
/// Method to call that will handle all commands from Node.js environment.
/// </summary>
/// <param name="info">An object containing Node.js parameters.</param>
/// <returns>Returns true if command execution is successful, false otherwise.</returns>
Napi::Boolean Method(const Napi::CallbackInfo& info)
{
	static HeiConnectDevice hc_device;

	bool success = false;
	int32_t param = -1;
	Napi::Env env = info.Env();

	if (info.Length() > 0)
	{
		std::string command = info[0].ToString().Utf8Value();

		if (command == "get-speed")
		{
			std::cout << hc_device.GetStirrerSpeed() << std::endl;
			success = true;
		}
		else if (command == "set-speed")
		{
			if (info.Length() > 1)
			{
				int32_t param = info[1].ToNumber().Int32Value();
				if (hc_device.SetStirrerSpeed(param))
				{
					success = true;
				}
				else
				{
					std::cout << "Device isn\'t accepting value" << std::endl;
				}
			}
			else
			{
				std::cout << "Not enough parameter" << std::endl;
			}
		}
		else
		{
			std::cout << "Unsupported command: " << command << std::endl;
		}
	}
	else
	{
		std::cout << "Zero paramter" << std::endl;
	}

	return Napi::Boolean::New(env, success);
}

/// <summary>
/// Node-API entry point.
/// </summary>
Napi::Object Init(Napi::Env env, Napi::Object exports)
{
	exports.Set(Napi::String::New(env, "HeiConnect"),
		Napi::Function::New(env, Method));
	return exports;
}

NODE_API_MODULE(addon, Init)
