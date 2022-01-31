#include "gps.h"
#include "mqtt/client.h"
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

/////////////////////////////////////////////////////////////////////////////

int main() {
  const std::string SERVER_ADDRESS{"tcp://localhost:8883"};
  // const string SERVER_ADDRESS{"tcp://test.mosquitto.org"};
  const std::string CLIENT_ID{"paho_cpp_sync_consume"};

  mqtt::client cli(SERVER_ADDRESS, CLIENT_ID);

  auto connOpts = mqtt::connect_options_builder()
                      .user_name("user")
                      .password("passwd")
                      .keep_alive_interval(std::chrono::seconds(30))
                      .automatic_reconnect(std::chrono::seconds(2),
                                           std::chrono::seconds(30))
                      .clean_session(false)
                      .finalize();

  const std::vector<std::string> TOPICS{"data/#", "gps"};
  const std::vector<int> QOS{0, 1};

  try {
    std::cout << "Connecting to the MQTT server..." << std::flush;
    mqtt::connect_response rsp = cli.connect(connOpts);
    std::cout << "OK\n" << std::endl;

    std::cout << "Subscribing to topics..." << std::flush;
    cli.subscribe(TOPICS, QOS);
    std::cout << "OK" << std::endl;

    // Consume messages

    while (true) {
      auto res = cli.consume_message();

      if (res) {
        // std::cout << msg->get_topic() << ": " << msg->to_string() <<
        // std::endl;

        const std::string msg = res->to_string();

        auto pos = msg.find(',');
        auto uuid = msg.substr(0, pos);
        auto rest = msg.substr(pos + 1, msg.length());

        pos = rest.find(',');
        double lat = std::stod(rest.substr(0, pos));
        double lon = std::stod(rest.substr(pos + 1, res.length()));

        std::cout << lat << '\n';
        std::cout << lon << '\n';
        auto dist = distanceBetween(45.0, -81.0, lat, lon);

        std::cout << dist << std::endl;

      } else if (!cli.is_connected()) {
        std::cout << "Lost connection" << std::endl;
        while (!cli.is_connected()) {
          std::this_thread::sleep_for(std::chrono::milliseconds(250));
        }
        std::cout << "Re-established connection" << std::endl;
      }
    }

    // Disconnect

    std::cout << "\nDisconnecting from the MQTT server..." << std::flush;
    cli.disconnect();
    std::cout << "OK" << std::endl;
  } catch (const mqtt::exception &exc) {
    std::cerr << exc.what() << std::endl;
    return 1;
  }

  return 0;
}
