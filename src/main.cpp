#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wpedantic"
#include "mqtt/client.h"
#pragma GCC diagnostic pop

#include "gps.h"
#include "uuid.h"
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

int main() {
  const std::string server_address{"tcp://localhost:8883"};
  // const string SERVER_ADDRESS{"tcp://test.mosquitto.org"};

  const uuids::uuid _uuid = uuids::uuid_system_generator{}();
  const std::string client_id = uuids::to_string(_uuid).substr(0, 8);

  mqtt::client cli(server_address, client_id);

  auto connOpts = mqtt::connect_options_builder()
                      .keep_alive_interval(std::chrono::seconds(30))
                      .automatic_reconnect(std::chrono::seconds(2),
                                           std::chrono::seconds(30))
                      .clean_session(false)
                      .finalize();

  const std::vector<std::string> topics{"data/#", "gps"};
  const std::vector<int> qos{0, 1};

  try {
    std::cout << "Connecting to the MQTT server...\n";
    cli.connect(connOpts);

    std::cout << "Subscribing to topics...\n";
    cli.subscribe(topics, qos);

    std::cout << "client id: " << client_id << "\n\n";

    MockGPS gps = MockGPS();

    std::thread publisher([&]() {
      while (true) {
        gps.update_pos();
        const auto payload = client_id + ',' + std::to_string(gps.lat) + ',' +
                             std::to_string(gps.lon);
        const auto pubmsg = mqtt::make_message(topics[1], payload, 1, 0);
        cli.publish(pubmsg);

        if (!cli.is_connected()) {
          break;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
      }
    });

    std::thread subscriber([&]() {
      while (true) {
        const auto res = cli.consume_message();

        if (res) {
          const auto msg = res->to_string();

          auto pos = msg.find(',');
          const auto sender_id = msg.substr(0, pos);

          if (sender_id == client_id) {
            continue;
          }

          const auto rest = msg.substr(pos + 1, msg.length());

          pos = rest.find(',');
          const double lat = std::stod(rest.substr(0, pos));
          const double lon = std::stod(rest.substr(pos + 1, rest.length()));

          const auto dist = distanceBetween(gps.lat, gps.lon, lat, lon);

          std::cout << sender_id << ": " << std::setprecision(3) << dist
                    << "km\n";

        } else if (!cli.is_connected()) {
          break;
        }
      }
    });

    if (publisher.joinable() && subscriber.joinable()) {
      publisher.join();
      subscriber.join();
      std::cout << "Lost connection\n";
    }
  } catch (const mqtt::exception &exc) {
    std::cerr << exc.what() << '\n';
    return 1;
  }

  return 0;
}
