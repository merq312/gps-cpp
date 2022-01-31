#include "mqtt/client.h"
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

using namespace std;
using namespace std::chrono;

/////////////////////////////////////////////////////////////////////////////

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
    const string SERVER_ADDRESS{"tcp://localhost:8883"};
    // const string SERVER_ADDRESS{"tcp://test.mosquitto.org"};
    const string CLIENT_ID{"paho_cpp_sync_consume"};

    mqtt::client cli(SERVER_ADDRESS, CLIENT_ID);

    auto connOpts = mqtt::connect_options_builder()
            .user_name("user")
            .password("passwd")
            .keep_alive_interval(seconds(30))
            .automatic_reconnect(seconds(2), seconds(30))
            .clean_session(false)
            .finalize();

    // You can install a callback to change some connection data
    // on auto reconnect attempts. To make a change, update the
    // `connect_data` and return 'true'.
    cli.set_update_connection_handler([](mqtt::connect_data &connData) {
        string newUserName{"new_user"};
        if (connData.get_user_name() == newUserName)
            return false;

        cout << "Previous user: '" << connData.get_user_name() << "'" << endl;
        connData.set_user_name(newUserName);
        cout << "New user name: '" << connData.get_user_name() << "'" << endl;
        return true;
    });

    // const vector<string> TOPICS{"data/#", "command"};
    const vector<string> TOPICS{"data/#", "gps"};
    const vector<int> QOS{0, 1};

    try {
        cout << "Connecting to the MQTT server..." << flush;
        mqtt::connect_response rsp = cli.connect(connOpts);
        cout << "OK\n" << endl;

        std::cout << "Subscribing to topics..." << std::flush;
        cli.subscribe(TOPICS, QOS);
        std::cout << "OK" << std::endl;

        // Consume messages

        while (true) {
            auto msg = cli.consume_message();

            if (msg) {
                if (msg->get_topic() == "command" && msg->to_string() == "exit") {
                    cout << "Exit command received" << endl;
                    break;
                }

                cout << msg->get_topic() << ": " << msg->to_string() << endl;
            } else if (!cli.is_connected()) {
                cout << "Lost connection" << endl;
                while (!cli.is_connected()) {
                    this_thread::sleep_for(milliseconds(250));
                }
                cout << "Re-established connection" << endl;
            }
        }

        // Disconnect

        cout << "\nDisconnecting from the MQTT server..." << flush;
        cli.disconnect();
        cout << "OK" << endl;
    } catch (const mqtt::exception &exc) {
        cerr << exc.what() << endl;
        return 1;
    }

    return 0;
}
